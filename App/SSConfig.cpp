//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "SSConfig.h"
#include "LicenseFile.h"

#include <WriteLock.h>
#include <ReadLock.h>

// needed for min and max certificate life values
#include <Certificate.h>

//--------------------------------------------------------------------------------
// global variable access control
int CSSConfig::m_nRefCount = 0;
CReadWriteObject CSSConfig::m_readWriteObj;

// data that's read from the registry
CString CSSConfig::m_sLogFilePath;
CString CSSConfig::m_sLicenseFileDefault("mcmssecserv.dat");
CString CSSConfig::m_sMainIP;
int CSSConfig::m_nSecondsToLive = MIN_CERT_LIFE;
DWORD CSSConfig::m_nStatusPort = 2525;
DWORD CSSConfig::m_nMainPort = 2112;
DWORD CSSConfig::m_nTraceLevel = 0;

// on the main server, this is the IP/port of the backup server
// on the backup server, this is the IP/port of the main server
CString CSSConfig::m_sBackupIP;
DWORD CSSConfig::m_nBackupPort = 0;

// optional tuning stuff - may or may not exist in the registry
DWORD CSSConfig::m_nDBCleanupInterval = 1000;

// data thats read from the license file
CTime CSSConfig::m_ctExpiration = (time_t) 0;
bool CSSConfig::m_bIsBackup = false;
CString CSSConfig::m_sMonitorUser;
CString CSSConfig::m_sMonitorPass;

CString CSSConfig::m_sEmailIP;
CString CSSConfig::m_sEmailFrom;
CString CSSConfig::m_sEmailTo;
CString CSSConfig::m_sDongle;
CString CSSConfig::m_sP3Serial;

DWORD CSSConfig::m_nConnectionPoolSize = 8;
DWORD CSSConfig::m_nMonitorPoolSize = 4;

// purely a global variables - ie they're only used at runtime - they're not stored
CSSConfigThread* CSSConfig::m_pThread = NULL;
CRegKey CSSConfig::regRoot_(HKEY_LOCAL_MACHINE, "Software\\Markcare Medical Systems\\SecurityServer\\1.0");
bool CSSConfig::m_bBackupIsServing = false;
DWORD CSSConfig::m_nReadCount = 0;
DWORD CSSConfig::m_nMinutesToWaitForDongle = 30;

//--------------------------------------------------------------------------------
CSSConfigThread::CSSConfigThread(CSystemObject* pParent)
		: CThreadObject(pParent)
	{

	}

//--------------------------------------------------------------------------------
bool CSSConfigThread::Init()
	{
	ReadData();
//	CSSConfig::m_regRoot.NotifyChangeKeyValue(FALSE, REG_NOTIFY_CHANGE_LAST_SET, m_evtChange, TRUE);
	return true;
	}

#ifdef _DEBUG
//--------------------------------------------------------------------------------
void CSSConfigThread::Exit(bool)
	{
	}
#endif

//--------------------------------------------------------------------------------
bool CSSConfigThread::MainLoop()
	{
	if(::WaitForSingleObject(m_evtChange, 10) == WAIT_OBJECT_0)
		{
//		CSSConfig::m_regRoot.NotifyChangeKeyValue(FALSE, REG_NOTIFY_CHANGE_LAST_SET, m_evtChange, TRUE);
		ReadData();
		}

	return CThreadObject::MainLoop();
	}

//--------------------------------------------------------------------------------
void CSSConfigThread::ReadData()
	{
	CWriteLock lock(&CSSConfig::m_readWriteObj);

	CRegValueSz valStr(CSSConfig::getRegRoot(), "LogFilePath");
	valStr.ReadValue(CSSConfig::m_sLogFilePath);

	valStr.SetValueName("DefaultLicFile");
	valStr.ReadValue(CSSConfig::m_sLicenseFileDefault);

	CRegValueDWord valNum(CSSConfig::getRegRoot(), "TraceLevel");
	valNum.ReadValue(&CSSConfig::m_nTraceLevel);

	DWORD nTemp;
	valNum.SetValueName("SecondsToLive");
	valNum.ReadValue(&nTemp);

	if(nTemp > MAX_CERT_LIFE)
		CSSConfig::m_nSecondsToLive = MAX_CERT_LIFE;
	else
		if(nTemp < MIN_CERT_LIFE)
			CSSConfig::m_nSecondsToLive = MIN_CERT_LIFE;
		else
			// round to the nearest 30 second interval
			CSSConfig::m_nSecondsToLive = (nTemp / 30) * 30;

	valNum.SetValueName("MainPort");
	valNum.ReadValue(&CSSConfig::m_nMainPort);

	valStr.SetValueName("MainIP");
	valStr.ReadValue(CSSConfig::m_sMainIP);

	valNum.SetValueName("StatusPort");
	valNum.ReadValue(&CSSConfig::m_nStatusPort);

	valNum.SetValueName("BackupPort");
	valNum.ReadValue(&CSSConfig::m_nBackupPort);

	valNum.SetValueName("MonitorPoolSize");
	valNum.ReadValue(&CSSConfig::m_nMonitorPoolSize);

	valNum.SetValueName("ConnectionPoolSize");
	valNum.ReadValue(&CSSConfig::m_nConnectionPoolSize);

	valStr.SetValueName("BackupIP");
	valStr.ReadValue(CSSConfig::m_sBackupIP);

	valNum.SetValueName("DBCleanupInterval");
	if(valNum.ReadValue(&nTemp))
		CSSConfig::m_nDBCleanupInterval = nTemp;

	CSSConfig::m_nReadCount++;
	}

//--------------------------------------------------------------------------------
DWORD CSSConfigThread::GetThreadLoopSleepInterval() const
	{
	// waiting is done in MainLoop
	return 100;
	}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
CSSConfig::CSSConfig(CSystemControlObject* pParent)
		: CSystemConfig(pParent)
	{
	if(m_nRefCount == 0)
		{
		if(! getRegRoot().CreateKey())
			return;

		m_pThread = new CSSConfigThread(pParent);
		m_pThread->StartThread();
		}

	m_nRefCount++;
	}

//--------------------------------------------------------------------------------
CSSConfig::~CSSConfig()
	{
	m_nRefCount--;

	if(m_nRefCount == 0)
		{
		getRegRoot() .CloseKey();
		delete m_pThread;
		m_pThread = NULL;
		}
	}

// should only be called from CDBSubSystem::LoadFromFile()
//--------------------------------------------------------------------------------
bool CSSConfig::SetDataFromLicenseFile(const CLicenseFile* pLicFile)
	{
	if(pLicFile == NULL)
		return false;

	// get a write lock on the static vars
	CWriteLock lock(&m_readWriteObj);

	m_ctExpiration = pLicFile->m_ctExpiration;
	m_sP3Serial = pLicFile->m_sP3Serial;
	m_sDongle = pLicFile->m_sDongle;
	if(pLicFile->m_sEmailIP == "0.0.0.0")
		{
		m_sEmailIP.Empty();
		m_sEmailFrom.Empty();
		m_sEmailTo.Empty();
		}
	else
		{
		m_sEmailIP = pLicFile->m_sEmailIP;
		m_sEmailFrom = pLicFile->m_sEmailFrom;
		m_sEmailTo = pLicFile->m_sEmailTo;
		if(m_sEmailTo.Find('@', 0) == -1)
			m_sEmailTo += "@markcare.com";
		}

	m_bIsBackup = pLicFile->m_bIsBackup;
	m_nMinutesToWaitForDongle = pLicFile->m_nMinutesToWaitForDongle;

	m_sMonitorUser = pLicFile->m_sMonitorUser;
	m_sMonitorPass = pLicFile->m_sMonitorPass;
	return true;
	}
