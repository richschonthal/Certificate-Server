//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"

#include <allocstring.h>
#include <LockedSocket.h>

#include "SecurityConnection.h"

#include <Certificate.h>
#include <AccessToken.h>
#include <WriteLock.h>
#include <ReadLock.h>

#if 0
#define RECEIVEWAIT1	INFINITE
#define RECEIVEWAIT2	INFINITE
#define RECEIVEWAIT3	INFINITE
#define SENDWAIT1		INFINITE
#define CONNECTWAIT		20000
#else
#define RECEIVEWAIT1	10000
#define RECEIVEWAIT2	10000
#define RECEIVEWAIT3	10000
#define SENDWAIT1		10000
#define CONNECTWAIT		6000
#endif

#define CONTAINER_LOCK_WAIT 3000

//ServerLibD.lib stdobjD.lib SecurityServerLibD.lib 
//--------------------------------------------------------------------------------
using namespace CCertificateCommands;

//--------------------------------------------------------------------------------
CSecurityConnection::CSecurityConnection(LPCTSTR pIP, UINT nPort, LPCTSTR pBackupIP, UINT nBackupPort)
		: CThreadObject(NULL)
		, m_pIP(NULL)
		, m_pBackupIP(NULL)
		, m_nSleepInterval(30003)
		, m_bObtained(false)
		, m_nPort(nPort)
		, m_nBackupPort(nBackupPort)
		, m_bAutoConnectToBackup(true)
		, m_nUserClass(0)
		, m_evtObtainedInitial(FALSE, TRUE)
	{
	ALLOC_STRING(m_pIP, pIP);
	ALLOC_STRING(m_pBackupIP, pBackupIP);
	m_pContainer = new CCertificateContainer;
	}

//--------------------------------------------------------------------------------
CSecurityConnection::~CSecurityConnection()
	{
	EndThread();
	if(m_pContainer != NULL)
		{
		ReleaseCertificate();
		delete m_pContainer;
		}

	delete[] m_pIP;
	delete[] m_pBackupIP;
	}

//--------------------------------------------------------------------------------
void CSecurityConnection::EndConnection()
	{
	CWriteLock lock(this);
	EndThread();
	ReleaseCertificate();
	delete m_pContainer;
	m_pContainer = NULL;
	}

//--------------------------------------------------------------------------------
bool CSecurityConnection::Init()
	{
	SetThreadPriority(THREAD_PRIORITY_ABOVE_NORMAL);
	return true;
	}

//--------------------------------------------------------------------------------
void CSecurityConnection::SetObtained(bool bObtained)
	{
	if(bObtained)
		m_evtObtained.SetEvent();
	else
		m_evtObtained.ResetEvent();

	m_bObtained = bObtained;
	}

//--------------------------------------------------------------------------------
bool CSecurityConnection::GetCertificateCopy(CCertificate*& pCopy)
	{
	CWriteLock lock(m_pContainer, false);
	if(! lock.Lock(500))
		return false;

	if(m_pContainer->m_pCert == NULL)
		return false;

	if(pCopy == NULL)
		{
		pCopy = new CCertificate(*m_pContainer->m_pCert);
		return true;
		}

	*pCopy = *m_pContainer->m_pCert;
	return true;
	}

//--------------------------------------------------------------------------------
bool CSecurityConnection::MainLoop()
	{
	CWriteLock lock1(this, false);
	if(! lock1.Lock(20))
		return true;

	CWriteLock lock2(m_pContainer, false);
	if(! lock2.Lock(20))
		return true;

	// this happens when we've lost a certificate
	if(m_pContainer->m_pCert == NULL && HasObtainedInitial())
		if(! ReGetCertificate())
			return true;

	if(m_pContainer->m_pCert == NULL)
		return true;

	// try to refresh
	if(! RefreshCertificate())
		if(! ReGetCertificate())
			return true;

	return true;
	}

//--------------------------------------------------------------------------------
bool CSecurityConnection::ReGetCertificate()
	{
	DWORD nTokenId = 0;
	int nUpper = m_nTokenIdList.GetUpperBound();
	if(nUpper >= 0)
		nTokenId = m_nTokenIdList[0];

	// get a certificate
	if(GetNewCertificate(m_nUserClass, nTokenId) == (ULONG) -1)
		return false;

	::Sleep(1000);

	if(nUpper > 0)
		for(int i = nUpper; i > 0; i--)
			{
			nTokenId = m_nTokenIdList[i];
			ReleaseToken(nTokenId);
			::Sleep(1000);
			if(AquireToken(nTokenId))
				TRACE("re-Aquired %d\n", nTokenId);
			else
				TRACE("DID NOT re-Aquired %d\n", nTokenId);
			}

	return true;
	}

//--------------------------------------------------------------------------------
bool CSecurityConnection::SwitchToBackup()
	{
	// if the refresh failed, and the result code is set (there was an error)
	// and auto-connect-to-backup is set and there's a backup IP and port # set...
	if(m_bAutoConnectToBackup && m_pBackupIP != NULL && m_nBackupPort != 0)
		{
		// swap the backup and main ip/port
		LPTSTR pIP = m_pBackupIP;
		m_pBackupIP = m_pIP;
		m_pIP = pIP;
		int nPort = m_nBackupPort;
		m_nBackupPort = m_nPort;
		m_nPort = nPort;
		TRACE("Switching to server at %s %d\n", pIP, nPort);
		return true;
		}

	return false;
	}

//--------------------------------------------------------------------------------
DWORD CSecurityConnection::GetThreadLoopSleepInterval() const
	{
	return m_nSleepInterval;
	}

//--------------------------------------------------------------------------------
LPCTSTR CSecurityConnection::GetIP() const
	{
	return m_pIP;
	}

//--------------------------------------------------------------------------------
UINT CSecurityConnection::GetPort() const
	{
	return m_nPort;
	}

//--------------------------------------------------------------------------------
LPCTSTR CSecurityConnection::GetBackupIP() const
	{
	return m_pBackupIP;
	}

//--------------------------------------------------------------------------------
UINT CSecurityConnection::GetBackupPort() const
	{
	return m_nBackupPort;
	}

//--------------------------------------------------------------------------------
CCertificate* CSecurityConnection::GetCertificate() const
	{
	return m_pContainer->m_pCert;
	}

//--------------------------------------------------------------------------------
DWORD CSecurityConnection::GetNewCertificate(DWORD nUserClass, DWORD nTokenId)
	{
	if(m_hThread == NULL)
		StartThread();

	CWriteLock lock(m_pContainer, false);
	if(! lock.Lock(CONTAINER_LOCK_WAIT))
		{
		TRACE("Lock failure (13)\n");
		return INVALID_CERT_ID;
		}

	DWORD nRv = DoGetNewCertificate(nUserClass, nTokenId);
	if(nRv == INVALID_CERT_ID)
		if(SwitchToBackup())
			nRv = DoGetNewCertificate(nUserClass, nTokenId);

	if(HasObtainedInitial() || nRv != INVALID_CERT_ID)
		m_evtObtainedInitial.SetEvent();
	else
		m_evtObtainedInitial.ResetEvent();
	return nRv;
	}

//--------------------------------------------------------------------------------
DWORD CSecurityConnection::GetNewCertificate(DWORD nUserClass, DWORD nTokenId, CCertificate*& pCopy)
	{
	if(m_hThread == NULL)
		StartThread();

	CWriteLock lock(m_pContainer, false);
	if(! lock.Lock(CONTAINER_LOCK_WAIT))
		{
		TRACE("Lock failure (13)\n");
		return INVALID_CERT_ID;
		}

	DWORD nRv = DoGetNewCertificate(nUserClass, nTokenId);
	if(nRv == INVALID_CERT_ID)
		if(SwitchToBackup())
			nRv = DoGetNewCertificate(nUserClass, nTokenId);

	if(HasObtainedInitial() || nRv != INVALID_CERT_ID)
		m_evtObtainedInitial.SetEvent();
	else
		m_evtObtainedInitial.ResetEvent();

	if(HasObtainedInitial())
		pCopy = new CCertificate(*m_pContainer->m_pCert);
	else
		pCopy = NULL;
	return nRv;
	}

//--------------------------------------------------------------------------------
bool CSecurityConnection::TransactCertificate(CCertificate& cert)
	{
	CSmallSocket socket;

	if(! socket.Create())
		{
		TRACE("CSecurityConnection::TransactCertificate cant create socket\n");

		SetResultCode(ErrCantCreateSocket);
		DoPostMessage(MsgCommError);
		OnError();
		return false;
		}

	char sTemp[4096];

	TRACE("TransactCertificate 1\n");
	// open a connection to the server
	if(! socket.Connect(GetIP(), GetPort(), CONNECTWAIT))
		{
		TRACE("CSecurityConnection::TransactCertificate cant connect %s %d\n", GetIP(), GetPort());
		SetResultCode(ErrCantConnectToServer);
		DoPostMessage(MsgCommError);
		OnError();
		return false;
		}

	TRACE("TransactCertificate 2\n");
	// get the server hello string
	if(socket.Receive(sTemp, 128, CSmallSocket::WAITFORDATA, RECEIVEWAIT1) < 1)
		{
		TRACE("CSecurityConnection::TransactCertificate receive err 1\n");
		SetResultCode(ErrCantConnectToServer);
		DoPostMessage(MsgCommError);
		OnError();
		return false;
		}

	TRACE("TransactCertificate 3\n");
	// the server isnt running so exit
	if(memcmp(sTemp, "000", 3) != 0)
		{
		TRACE("CSecurityConnection::TransactCertificate server not running\n");
		SetResultCode(ErrCantConnectToServer);
		DoPostMessage(MsgServerDown);
		OnError();
		return false;
		}

	int nLen = cert.Encode(sTemp, false);

	TRACE("TransactCertificate 4\n");
	// send the request
	if(socket.Send(sTemp, nLen, CSmallSocket::WAITFORWOULDBLOCK, SENDWAIT1) != nLen)
		{
		TRACE("CSecurityConnection::TransactCertificate send err 1\n");
		SetResultCode(ErrSocketSend);
		DoPostMessage(MsgCommError);
		OnError();
		return false;
		}

	nLen = socket.Receive(sTemp, 4096, CSmallSocket::WAITFORDATA, RECEIVEWAIT1);
	TRACE("TransactCertificate 5\n");
	// get our new certificate
	if(nLen < 1)
		{
		TRACE("CSecurityConnection::TransactCertificate receive err 2\n");
		SetResultCode(ErrSocketReceive);
		DoPostMessage(MsgCommError);
		OnError();
		return false;
		}

	TRACE("TransactCertificate 6 done\n");

	cert.Decode(sTemp, nLen);	
	return cert.GetCommandParam() == INFO_OK;
	}

//--------------------------------------------------------------------------------
DWORD CSecurityConnection::DoGetNewCertificate(DWORD nUserClass, DWORD nTokenId)
	{
	m_pContainer->DeleteCertificate();

	SetObtained(false);

	CCertificate* pCert = new CCertificate(CMD_NEW, nUserClass, nTokenId);

	m_nUserClass = nUserClass;
	m_nTokenIdList.SetAtGrow(0, nTokenId);

	if(! TransactCertificate(*pCert))
		{
		delete pCert;
		return INVALID_CERT_ID;
		}

	TRACE("GetNewCertificate 10\n");
	int nLife = pCert->GetSecondsToLive();
	if(nLife < MIN_CERT_LIFE || nLife > MAX_CERT_LIFE)
		m_nSleepInterval = 30000;
	else
		// convert to miliseconds divided by 2
		m_nSleepInterval = (nLife * 500);

	TRACE("GetNewCertificate 13\n");

	DoPostMessage(MsgNew);
	OnNewCertificate();

	TRACE("GetNewCertificate 14\n");

	m_pContainer->m_pCert = pCert;

	SetObtained();

	return pCert->GetId();
	}

//--------------------------------------------------------------------------------
bool CSecurityConnection::RefreshCertificate()
	{
	CWriteLock lock(m_pContainer, false);
	if(! lock.Lock(CONTAINER_LOCK_WAIT))
		{
		TRACE("Lock failure (14)\n");
		return false;
		}

	SetObtained(false);
	if(m_pContainer->m_pCert == NULL)
		{ 
		SetResultCode(ErrNULLCertificate);
		DoPostMessage(MsgGeneralError);
		OnError();
		return false;
		}

	m_pContainer->m_pCert->SetCommand(CMD_REFRESH);
	if(! TransactCertificate(*m_pContainer->m_pCert))
		{
		m_pContainer->DeleteCertificate();
		SetResultCode(ErrSocketSend);
		DoPostMessage(MsgCommError);
		OnError();
		return false;
		}

	if(m_pContainer->m_pCert->GetSecondsToLive() < 30)
		m_nSleepInterval = m_pContainer->m_pCert->GetSecondsToLive() * 1000;

	SetResultCode(0);
	SetOSResult(0);

	SetObtained();
	DoPostMessage(MsgRefreshed);
	OnRefresh();
	return true;
	}

//--------------------------------------------------------------------------------
bool CSecurityConnection::ReleaseCertificate()
	{
	CWriteLock lock(m_pContainer, false);
	if(! lock.Lock(CONTAINER_LOCK_WAIT))
		{
		TRACE("Lock failure (15)\n");
		return false;
		}

	SetObtained(false);
	m_evtObtainedInitial.ResetEvent();

	if(m_pContainer->m_pCert == NULL)
		{
		SetResultCode(ErrNULLCertificate);
		DoPostMessage(MsgGeneralError);
		OnError();
		return false;
		}

	// form our request
	m_pContainer->m_pCert->SetCommand(CMD_RELEASE);
	m_pContainer->m_pCert->SetCommandParam(0);

	TransactCertificate(*m_pContainer->m_pCert);

	DoPostMessage(MsgReleased);

	m_pContainer->DeleteCertificate();

	SetResultCode(0);
	return true;
	}

//--------------------------------------------------------------------------------
bool CSecurityConnection::AquireToken(ULONG nTokenId)
	{
	CWriteLock lock(m_pContainer, false);
	if(! lock.Lock(CONTAINER_LOCK_WAIT))
		{
		TRACE("Lock failure (14)\n");
		return false;
		}

	if(m_pContainer->m_pCert == NULL)
		{ 
		SetResultCode(ErrNULLCertificate);
		DoPostMessage(MsgGeneralError);
		OnError();
		return false;
		}

	// form our request
	m_pContainer->m_pCert->SetCommand(CMD_NEW);
	m_pContainer->m_pCert->SetCommandParam(nTokenId);

	if(! TransactCertificate(*m_pContainer->m_pCert))
		{
		m_pContainer->DeleteCertificate();
		SetResultCode(ErrSocketSend);
		DoPostMessage(MsgCommError);
		OnError();
		return false;
		}

	if(m_pContainer->m_pCert->GetSecondsToLive() < 30)
		m_nSleepInterval = m_pContainer->m_pCert->GetSecondsToLive() * 1000;

	SetResultCode(0);
	SetOSResult(0);

	if(m_pContainer->m_pCert->GetCommandParam() != INFO_OK)
		{
		m_pContainer->DeleteCertificate();
		DoPostMessage(MsgRevoked);
		OnRevoked();
		return false;
		}

	m_nTokenIdList.Add(nTokenId);

	SetObtained();
	DoPostMessage(MsgNew);
	return true;
	}

//--------------------------------------------------------------------------------
bool CSecurityConnection::ReleaseToken(ULONG nId)
	{
	CWriteLock lock(m_pContainer, false);
	if(! lock.Lock(CONTAINER_LOCK_WAIT))
		{
		TRACE("Lock failure (15)\n");
		return false;
		}

	if(m_pContainer->m_pCert == NULL)
		{
		SetResultCode(ErrNULLCertificate);
		DoPostMessage(MsgGeneralError);
		OnError();
		return false;
		}

	for(int i = 0; i < m_nTokenIdList.GetSize(); i++)
		if(m_nTokenIdList[i] == nId)
			{
			m_nTokenIdList.RemoveAt(i);
			break;
			}

	m_pContainer->m_pCert->SetCommand(CMD_RELEASE);
	m_pContainer->m_pCert->SetCommandParam(nId);

	TransactCertificate(*m_pContainer->m_pCert);

	SetResultCode(0);
	return true;
	}

//--------------------------------------------------------------------------------
bool CSecurityConnection::OnNewCertificate()
	{
	return true;
	}

//--------------------------------------------------------------------------------
bool CSecurityConnection::OnRevoked()
	{
	SetObtained(false);
	return true;
	}

//--------------------------------------------------------------------------------
bool CSecurityConnection::OnRefresh()
	{
	return true;
	}

//--------------------------------------------------------------------------------
bool CSecurityConnection::OnError()
	{
	return true;
	}

//--------------------------------------------------------------------------------
void CSecurityConnection::DoPostMessage(int nMsg)
	{
	CSecurityTarget::DoPostMessage((WPARAM) m_pContainer->m_pCert, (LPARAM) nMsg);
	}

//--------------------------------------------------------------------------------
bool CSecurityConnection::IsValidCertificate()
	{
	if(m_pContainer == NULL)
		return false;

	CReadLock lock(m_pContainer);

	if(m_pContainer->m_pCert == NULL)
		return false;
	
	return (m_pContainer->m_pCert->GetCommand() == CMD_NEW || m_pContainer->m_pCert->GetCommand() == CMD_REFRESH) 
		&& m_pContainer->m_pCert->GetCommandParam() == INFO_OK;
	}

//--------------------------------------------------------------------------------
bool CSecurityConnection::HasToken(DWORD nId)
	{
	if(m_pContainer == NULL)
		return false;

	CReadLock lock(m_pContainer);

	if(m_pContainer->m_pCert == NULL)
		return false;

	return m_pContainer->m_pCert->HasToken(nId);
	}

//--------------------------------------------------------------------------------
LPCTSTR CSecurityConnection::GetMessageText(int n1, int n2)
	{
	static struct
		{
		int nId1;
		int nId2;
		LPCTSTR pMsg;
		} messages[] =

		{
		CMD_NEW,		INFO_OK,				"New certificate issued",
		CMD_NEW,		ERR_NO_SUCH_TOKEN,		"Certificate not issued - no such token",
		CMD_NEW,		ERR_TOKEN_NOT_AVAIL,	"Certificate not issued - token not available",
		CMD_NEW,		0,						"Certificate not issued - unknown error",
		CMD_REFRESH,	INFO_OK,				"Certificate refreshed",
		CMD_REFRESH,	ERR_NO_SUCH_CERT,		"Certificate not refreshed - no such certificate",
		CMD_REFRESH,	INFO_REVOKED,			"Certificate not refreshed - certificate revoked",
		CMD_REFRESH,	0,						"Certificate not refreshed - unknown error",
		CMD_RELEASE,	0,						"Certificate released",
		0,				0,						""
		};

	int nEndIndex = 0;

	// find an exact match
	for(int i = 0; ;i++)
		{
		if(n1 == messages[i].nId1 && n2 == messages[i].nId2)
			return messages[i].pMsg;

		if(messages[i].nId1 == 0 && messages[i].nId2 == 0)
			{
			nEndIndex = i;
			break;
			}
		}

	// ok so then match the first when the second is 0
	for(i = 0; i < nEndIndex;i++)
		{
		if(n1 == messages[i].nId1 && messages[i].nId2 == 0)
			return messages[i].pMsg;
		}

	// still no match so return the empty string
	return messages[nEndIndex].pMsg;
	}
