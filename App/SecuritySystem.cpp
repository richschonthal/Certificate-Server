//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "SecuritySystem.h"
#include "ClientSubSystem.h"
#include "SSIOSubSystem.h"
#include "DBSubSystem.h"
#include "MonitorSubSystem.h"
#include "SSConfig.h"
#include "SSConfigGeneral.h"
#include "SSConfigBackup.h"
#include "SSConfigPerformance.h"
#include "LicenseCheckThread.h"
#include "BackupThread.h"
#include "DBTokens.h"
#include "SecServTokenId.h"
#include "SSQuickMail.h"

#include "Dongle.h"

#ifdef _DEBUG
#include <PlatformInfo.h>
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//--------------------------------------------------------------------------------
CSecuritySystem::CSecuritySystem()
		: m_pBackupThread(NULL)
		, m_nServerMode(SERVERMODE_UNKNOWN)
		, m_evtLicenseOk(FALSE, TRUE)
		, m_evtDongleOk(FALSE, TRUE)
		, m_ttBackupCheckIn(0)
	{
	SetState(STATE_PAUSE);

	// assume the dongle's ok until it's time to start checking it
	m_evtDongleOk.SetEvent();

	SetObjectConfiguration(new CSSConfig(this));

	AddSubSystem(m_pIOSubSystem = new CSSIOSubSystem(this));
	AddSubSystem(m_pDBSubSystem = new CDBSubSystem(this));
	AddSubSystem((CSubSystem*) (m_pClientSubSystem = new CClientSubSystem(this)));
	
	CMonitorSubSystem* pMonSub = new CMonitorSubSystem(this);
	
	AddSubSystem(pMonSub);

	::Sleep(1000);

	m_ctSystemStart = CTime::GetCurrentTime();

	if(! m_pDBSubSystem->LoadFromFile())
		SetResultCode(errBadLicenseFile);

	m_pLicCheckThread = new CLicenseCheckThread(this);
	m_pLicCheckThread->StartThread();

	SetState(STATE_RUN);

	// sleep to allow everything to start up
	::Sleep(6000);
	}

//--------------------------------------------------------------------------------
CSecuritySystem::~CSecuritySystem()
	{
	CSSQuickMail mail;
	mail.m_sSubject = "IMP: Security Server Shutting Down " + CTime::GetCurrentTime().Format("%c");
	mail.m_sMsg = mail.m_sSubject + "\r\n";
	mail.Send();
	m_pLicCheckThread->EndThread();
	delete m_pLicCheckThread;
	m_pBackupThread->EndThread();
	delete m_pBackupThread;
	DeleteConfiguration();
	}

//--------------------------------------------------------------------------------
void CSecuritySystem::CreateStartupEmailMessage(CString& sMsg) const
	{
	CStringArray sArray;
	GetSystemConfigStringMessage(sArray);
	for(int i = 0; i < sArray.GetSize(); i++)
		sMsg += sArray[i] + "\r\n";
	}

//--------------------------------------------------------------------------------
void CSecuritySystem::StartBackupMode()
	{
	if(m_nServerMode == SERVERMODE_BACKUP)
		return;

	if(m_pBackupThread != NULL)
		{
		m_pBackupThread->EndThread();
		delete m_pBackupThread;
		m_pBackupThread = NULL;
		}

	m_nServerMode = SERVERMODE_BACKUP;

	SetIsServing(false);
	
	m_pBackupThread = new CBackupThread(this);
	m_pBackupThread->StartThread();
	
	CSSConfigBackup config;
	CTime ctDeadline(CTime::GetCurrentTime() + CTimeSpan(0, 0, config.m_nMinutesToWaitForDongle, 0));
	::InterlockedExchange((LONG*) &m_ctDongleDeadline, (LONG) ctDeadline.GetTime());

	CSSQuickMail* pMail = new CSSQuickMail;
	pMail->m_sSubject = "FYI: Backup started " + CTime::GetCurrentTime().Format("%c\r\n");
	CreateStartupEmailMessage(pMail->m_sMsg);
	m_pIOSubSystem->SendQuickMail(pMail);
	}

//--------------------------------------------------------------------------------
void CSecuritySystem::StartMainserverMode()
	{
	if(m_nServerMode == SERVERMODE_MAIN)
		return;

	if(m_pBackupThread != NULL)
		{
		m_pBackupThread->EndThread();
		delete m_pBackupThread;
		m_pBackupThread = NULL;
		}

	m_nServerMode = SERVERMODE_MAIN;

	SetIsServing();
	CTime ctDeadline(CTime::GetCurrentTime() + CTimeSpan(0, 0, nMinutesToWaitForDongle, 0));
	::InterlockedExchange((LONG*) &m_ctDongleDeadline, (LONG) ctDeadline.GetTime());
	CSSQuickMail* pMail = new CSSQuickMail;
	pMail->m_sSubject = "FYI: Security Server started " + CTime::GetCurrentTime().Format("%c\r\n");
	CreateStartupEmailMessage(pMail->m_sMsg);
	m_pIOSubSystem->SendQuickMail(pMail);
	}

//--------------------------------------------------------------------------------
void CSecuritySystem::SetIsServing(bool bServe)
	{
	CSSConfigBackup config;
	if(config.m_bIsBackup)
		{
		if(bServe)
			{
			// a backup system now taking over
			m_ctSystemStart = CTime::GetCurrentTime();
			CTime ctDeadline(CTime::GetCurrentTime() + CTimeSpan(0, 0, config.m_nMinutesToWaitForDongle, 0));
			::InterlockedExchange((LONG*) &m_ctDongleDeadline, (LONG) ctDeadline.GetTime());
			m_evtDongleOk.SetEvent();
			}
		else
			::InterlockedExchange((LONG*) &m_ctDongleDeadline, 0);
		}
	else
		{
		if(bServe)
			{
			m_ctSystemStart = CTime::GetCurrentTime();
			CTime ctDeadline(CTime::GetCurrentTime() + CTimeSpan(0, 0, nMinutesToWaitForDongle, 0));
			::InterlockedExchange((LONG*) &m_ctDongleDeadline, (LONG) ctDeadline.GetTime());
			m_evtDongleOk.SetEvent();
			}
		else
			::InterlockedExchange((LONG*) &m_ctDongleDeadline, 0);
		}

	m_bIsServing = bServe;
	}

//--------------------------------------------------------------------------------
bool CSecuritySystem::IsLicensed()
	{
	HANDLE hands[2] = { m_evtDongleOk, m_evtLicenseOk};

	return ::WaitForMultipleObjects(2, hands, TRUE, 0) < 2;
	}

//--------------------------------------------------------------------------------
bool CSecuritySystem::IsBackupRunning()
	{
	time_t ttTime = m_ttBackupCheckIn;
	CTime ctWhen(ttTime);

	return (CTime::GetCurrentTime() - ctWhen).GetTotalMinutes() == 0;
	}

//--------------------------------------------------------------------------------
void CSecuritySystem::SetBackupIsRunning()
	{
	::InterlockedExchange((LONG*) &m_ttBackupCheckIn, (LONG) CTime::GetCurrentTime().GetTime());
	}

//--------------------------------------------------------------------------------
void CSecuritySystem::SetDongleStatus(bool bIsOk)
	{
	if(bIsOk)
		m_evtDongleOk.SetEvent();
	else
		m_evtDongleOk.ResetEvent();
	}

//--------------------------------------------------------------------------------
void CSecuritySystem::SetLicenseStatus(bool bIsOk)
	{
	if(bIsOk)
		m_evtLicenseOk.SetEvent();
	else
		m_evtLicenseOk.ResetEvent();
	}

//--------------------------------------------------------------------------------
void CSecuritySystem::GetSystemConfigStringMessage(CStringArray& sArray) const
	{
	sArray.RemoveAll();

	CString sTemp;

	CSSConfigGeneral configGeneral;

	sTemp = "-- General --";
	sArray.Add(sTemp);

	sTemp.Format("%16s: %s", "Log path", (LPCTSTR) configGeneral.m_sLogFilePath);
	sArray.Add(sTemp);

	sTemp.Format("%16s: %s", "Dflt Lic file", (LPCTSTR) configGeneral.m_sLicenseFileDefault);
	sArray.Add(sTemp);

	sTemp.Format("%16s: %s", "PIII Serial", (LPCTSTR) configGeneral.m_sP3Serial);
	sArray.Add(sTemp);

	sTemp.Format("%16s: %s", "Dongle", (LPCTSTR) configGeneral.m_sDongle);
	sArray.Add(sTemp);

	sTemp.Format("%16s: %s", "Email IP", (LPCTSTR) configGeneral.m_sEmailIP);
	sArray.Add(sTemp);

	sTemp.Format("%16s: %s", "Email From", (LPCTSTR) configGeneral.m_sEmailFrom);
	sArray.Add(sTemp);

	sTemp.Format("%16s: %s", "Email To", (LPCTSTR) configGeneral.m_sEmailTo);
	sArray.Add(sTemp);

	sTemp.Format("%16s: %s", "Expires", (LPCTSTR) configGeneral.m_ctExpiration.Format("%c"));
	sArray.Add(sTemp);

	sTemp.Format("%16s: %ld", "Main Port", configGeneral.m_nMainPort);
	sArray.Add(sTemp);

	sTemp.Format("%16s: %s", "Main IP", (LPCTSTR) configGeneral.m_sMainIP);
	sArray.Add(sTemp);

	CSSConfigBackup configBackup;

	sTemp = "-- Backup --";
	sArray.Add(sTemp);

	sTemp.Format("%16s: %c", "Is Backup", (LPCTSTR) configBackup.m_bIsBackup ? 'T' : 'F');
	sArray.Add(sTemp);

	sTemp.Format("%16s: %s", "IP", (LPCTSTR) configBackup.m_sIP);
	sArray.Add(sTemp);

	sTemp.Format("%16s: %ld", "Port", configBackup.m_nPort);
	sArray.Add(sTemp);

	sTemp.Format("%16s: %d", "Dongle Wait", configBackup.m_nMinutesToWaitForDongle);
	sArray.Add(sTemp);

	CSSConfigPerformance configPerf;

	sTemp = "-- Performance --";
	sArray.Add(sTemp);

	sTemp.Format("%16s: %d", "Sec to Live", configPerf.m_nSecondsToLive);
	sArray.Add(sTemp);

	sTemp.Format("%16s: 0x%lx", "Trace Level", configPerf.m_nTraceLevel);
	sArray.Add(sTemp);
	
	sTemp.Format("%16s: %ld", "DB Clean Up", configPerf.m_nDBCleanupInterval);
	sArray.Add(sTemp);
	}

//--------------------------------------------------------------------------------
void CSecuritySystem::SetLicenseFilename(const CString& sName)
	{
	m_sLicenseFilename = sName;
	}

//--------------------------------------------------------------------------------
CString CSecuritySystem::GetLicenseFilename()
	{
	if(m_sLicenseFilename.IsEmpty())
		{
		CSSConfigGeneral config;
		return config.m_sLicenseFileDefault;
		}

	return m_sLicenseFilename;
	}
