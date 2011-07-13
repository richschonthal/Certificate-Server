//--------------------------------------------------------------------------------
//
// Copyright (c) 2001 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"

#include <timer.h>

#include <AccessToken.h>
#include <certificate.h>

#include "SecurityConnection.h"
#include "SecurityClient.h"

#include "dongle.h"

#include <ReadLock.h>
#include <WriteLock.h>
#include <Timer.h>

//--------------------------------------------------------------------------------
CEvent CSecurityClient::m_evtMustExit;

//--------------------------------------------------------------------------------
CSecurityClient::CSecurityClient()
		: m_nUser(0)
		, m_nToken(0)
		, m_nDongleMode(DongleNone)
		, m_pConnection(NULL)
		, m_evtConnectComplete(FALSE, TRUE)
		, m_evtWaitingForObtained(FALSE, TRUE)
		, m_pConnectDeadlineTimer(NULL)
		, CThreadObject(NULL)
	{
	}

//--------------------------------------------------------------------------------
CSecurityClient::~CSecurityClient()
	{
	SetTargetNone();
	EndThread();
	if(m_pConnection != NULL)
		{
		m_pConnection->EndConnection();
		delete m_pConnection;
		}
	
	CancelDeadlineTimer();
	}

//--------------------------------------------------------------------------------
bool CSecurityClient::Init()
	{
	SetThreadPriority(THREAD_PRIORITY_ABOVE_NORMAL);
	return true;
	}

//--------------------------------------------------------------------------------
bool CSecurityClient::Connect(DWORD nUser, DWORD nToken, LPCTSTR pIP, DWORD nPort,
								 LPCTSTR pBIP, DWORD nBPort, eDongleMode nDongleMode)
	{
	if(m_pConnection != NULL)
		return true;

	// save params
	SetDongleMode(nDongleMode);
	if(nDongleMode == DongleOnly)
		return false;

	m_nUser = nUser;
	m_nToken = nToken;

	// we are waiting...
	m_evtWaitingForObtained.SetEvent();

	// create our low-level connection
	m_pConnection = new CSecurityConnection(pIP, nPort, pBIP, nBPort);

	// lock the certificate
	CWriteLock lock(&m_container);

	// try and connect - GetNewCertificate will automatically take care of switching servers etc
	if(m_pConnection->GetNewCertificate(m_nUser, m_nToken, m_container.m_pCert) == INVALID_CERT_ID)
		{
		// no connections made so we're in dongle mode
		m_evtWaitingForObtained.ResetEvent();
		// no more connection needed
		m_pConnection->EndConnection();
		delete m_pConnection;
		m_pConnection = NULL;
		// use the dongle instead
		if(nDongleMode != DongleNone)
			return Connect();
		return false;
		}

	// get the MainLoop cranking
	StartThread();
	// no longer waiting for a certificate
	m_evtWaitingForObtained.ResetEvent();
	// we've made a connection
	m_evtConnectComplete.SetEvent();
	// got a certificate - so set us as a target for receiving message from the connection
	m_pConnection->SetTarget(this, MsgIdCertificate);
	return IsLicensed();
	}

//--------------------------------------------------------------------------------
bool CSecurityClient::Connect()
	{
	SetDongleMode(DongleOnly);
	m_evtWaitingForObtained.ResetEvent();
	if(IsValidDongle())
		{
		StartThread();
		m_evtConnectComplete.SetEvent();
		return true;
		}
	return false;
	}

//--------------------------------------------------------------------------------
bool CSecurityClient::AquireToken(DWORD nToken)
	{
	if(! IsObtained() || m_pConnection == NULL)
		return false;

	if(m_pConnection->AquireToken(nToken))
		{
		CWriteLock lock(&m_container);
		m_pConnection->GetCertificateCopy(m_container.m_pCert);
		return true;
		}

	return false;
	}

//--------------------------------------------------------------------------------
bool CSecurityClient::HasToken(DWORD nToken)
	{
	CWriteLock lock(&m_container);
	if(m_container.m_pCert == NULL)
		return false;

	return m_container.m_pCert->HasToken(nToken);
	}

//--------------------------------------------------------------------------------
bool CSecurityClient::ReleaseToken(DWORD nToken)
	{
	if(! IsObtained())
		return false;

	if(m_pConnection->ReleaseToken(nToken))
		{
		CWriteLock lock(&m_container);
		m_pConnection->GetCertificateCopy(m_container.m_pCert);
		return true;
		}

	return false;
	}

//--------------------------------------------------------------------------------
bool CSecurityClient::DoSecurityMessage(LONG nMsg)
	{
	switch(nMsg)
		{
		case CSecurityConnection::MsgCommError:
		case CSecurityConnection::MsgGeneralError:
			// suspend msg sending
			m_pConnection->SetTargetNone();
			// we're now wating to obtain a new cert
			m_evtWaitingForObtained.SetEvent();
			// we have to have it by a certain time
			SetDeadlineTimer();
			OnCommError();
			break;
		case CSecurityConnection::MsgRevoked:
		case CSecurityConnection::MsgReleased:
			// suspend msg sending
			m_pConnection->SetTargetNone();
			m_evtWaitingForObtained.SetEvent();
			break;
		}

	return true;
	}

//--------------------------------------------------------------------------------
void CSecurityClient::OnMessage(UINT nMsg, WPARAM wparam, LPARAM lparam)
	{
	switch(nMsg)
		{
		case MsgIdCertificate:
			if(! DoSecurityMessage(lparam))
				SetTerminate();
			break;
		}
	}

//--------------------------------------------------------------------------------
void CSecurityClient::SetDeadlineTimer()
	{
	if(m_pConnectDeadlineTimer != NULL)
		return;

	m_pConnectDeadlineTimer = new CPulseTimer;
	// set this one with 'eat the first pulse'
	m_pConnectDeadlineTimer->Set(GetMaxReconnectWaitTime());
	}

//--------------------------------------------------------------------------------
void CSecurityClient::CancelDeadlineTimer()
	{
	delete m_pConnectDeadlineTimer;
	m_pConnectDeadlineTimer = NULL;
	}

//--------------------------------------------------------------------------------
bool CSecurityClient::IsObtained()
	{
	return ::WaitForSingleObject(m_pConnection->m_evtObtained, 0) == WAIT_OBJECT_0;
	}

//--------------------------------------------------------------------------------
bool CSecurityClient::IsWaitingForObtained()
	{
	return ::WaitForSingleObject(m_evtWaitingForObtained, 0) == WAIT_OBJECT_0;
	}

//--------------------------------------------------------------------------------
bool CSecurityClient::MainLoop()
	{
	if(! IsConnectComplete())
		return true;

	// we've lost the certificate and now we're waiting for a new one
	// but we're still using our old copy
	if(IsWaitingForObtained())
		{
		if(IsObtained())
			{
			// no more deadline
			CancelDeadlineTimer();
			// no longer waiting
			m_evtWaitingForObtained.ResetEvent();
			// certificate obtained so reinstate msg sending
			m_pConnection->SetTarget(this, MsgIdCertificate);
			CWriteLock lock(&m_container);
			// keep a copy of the new certificate
			m_pConnection->GetCertificateCopy(m_container.m_pCert);
			}
		else
			{
			// set the timer for obtaining a license
			// does nothing if the timer's already set
			SetDeadlineTimer();
			// we have no certificate but maybe we have a dongle or a cert copy
			if(! IsLicensed())
				{
				SetTerminate();
				return false;
				}
			// we should have (re)connected by now so if not then we're done
			if(IsConnectDeadline())
				{
				// we've timed out, there's no dongle, no note from mom, life sucks, app must bail
				SetTerminate();
				return false;
				}
			return true;
			}
		}

	if(IsLicensed())
		return true;

	SetTerminate();
	return false;
	}

//--------------------------------------------------------------------------------
void CSecurityClient::SetTerminate()
	{
	TRACE("********** MUST TERMINATE AND DIE DIE DIE ******************\n");
	if(m_pConnection != NULL)
		{
		m_pConnection->EndConnection();
		delete m_pConnection;
		m_pConnection = NULL;
		}

	DoPostMessage(nMustTerminate);
	m_evtMustExit.SetEvent();
	OnMustExit();
	}

//--------------------------------------------------------------------------------
CDongle* CSecurityClient::GetDongle()
	{
	return NULL;
	}

//--------------------------------------------------------------------------------
bool CSecurityClient::IsLicensed()
	{
	// if its dongleonly then only check l'dongle
	if(GetDongleMode() == DongleOnly)
		return IsValidDongle();

	// if we've never gotten a certificate then the best we can do  is check out la dongla
	CWriteLock lock(&m_container);
	if(m_container.m_pCert == NULL)
		return IsValidDongle();

	// we must have lost our secserv connection but maybe we still have a copy of the certificate
	if(::WaitForSingleObject(m_evtWaitingForObtained, 0) == WAIT_OBJECT_0)
		{
		// if we have no certificate
		if(m_container.m_pCert == NULL || IsConnectDeadline())
			{
			delete m_container.m_pCert;
			m_container.m_pCert = NULL;
			// and were not allowed to use a dongle
			if(GetDongleMode() == DongleNone)
				return false;
			// we have no certificate copy so we need a dongle or we must exit
			return IsValidDongle();
			}
		}

	// great scott!  we actually have a valid certificate
	return m_container.m_pCert->HasToken(m_nToken);
	}

//--------------------------------------------------------------------------------
bool CSecurityClient::IsValidDongle()
	{
	// a dongle cannot be valid if were not using them
	if(GetDongleMode() == DongleNone)
		return false;

	CDongle* pDongle = GetDongle();
	if(pDongle == NULL)
		{
		OnCheckDongle(false);
		return false;
		}

	bool bIsValid = pDongle->IsCorrectDongleMemory();
	OnCheckDongle(bIsValid);
	if(bIsValid)
		{
		SwitchToDongleOnlyMode();
		return true;
		}

	return false;
	}

//--------------------------------------------------------------------------------
void CSecurityClient::SwitchToDongleOnlyMode()
	{
	eDongleMode nMode = GetDongleMode();
	if(nMode == DongleOnly || nMode == DongleNone)
		return;
	// no more deadline
	CancelDeadlineTimer();
	// no longer waiting
	m_evtWaitingForObtained.ResetEvent();
	// no longer need a connection
	if(m_pConnection != NULL)
		{
		m_pConnection->EndConnection();
		delete m_pConnection;
		m_pConnection = NULL;
		m_evtWaitingForObtained.ResetEvent();
		}
	SetDongleMode(DongleOnly);
	}

//--------------------------------------------------------------------------------
bool CSecurityClient::IsInDongleMode()
	{
	// we cant be in dongle mode if were not in dongle mode
	if(GetDongleMode() == DongleNone)
		return false;
	// were always in dongle mode if were always in dongle mode
	if(GetDongleMode() == DongleOnly)
		return true;
	// were in dongle mode if we dont have a certificate
	CWriteLock lock(&m_container);
	return m_container.m_pCert == NULL;
	}

//--------------------------------------------------------------------------------
bool CSecurityClient::HasPermission(DWORD nTokenId, UINT nBit)
	{
	if(GetDongleMode() == DongleOnly)
		return IsValidDongle();

	CWriteLock lock(&m_container);
	if(m_container.m_pCert == NULL)
		return false;

	if(m_container.m_pCert == NULL)
		return false;

	CAccessToken* pToken = m_container.m_pCert->GetToken(nTokenId);
	if(pToken == NULL)
		return false;

	return pToken->IsBitSet(nBit);
	}

//--------------------------------------------------------------------------------
void CSecurityClient::OnCommError()
	{
	}

//--------------------------------------------------------------------------------
void CSecurityClient::OnMustExit()
	{
	}

//--------------------------------------------------------------------------------
void CSecurityClient::OnCheckDongle(bool)
	{
	}

//--------------------------------------------------------------------------------
DWORD CSecurityClient::GetMaxReconnectWaitTime() const
	{
	return 90000;
	}
