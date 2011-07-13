//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _SSCONFIG_H_
#define _SSCONFIG_H_

//--------------------------------------------------------------------------------
#include <ThreadObject.h>
#include <SystemConfig.h>
#include <ReadWriteObject.h>
#include <Registry.h>
#include "DBSubSystem.h"

class CLicenseFile;

//--------------------------------------------------------------------------------
class CSSConfigThread : public CThreadObject
	{
	public:
		CSSConfigThread(CSystemObject*);

	private:
		CEvent m_evtChange;

	private:
		virtual bool MainLoop();
		virtual bool Init();
#ifdef _DEBUG
		virtual void Exit(bool);
#endif
		virtual DWORD GetThreadLoopSleepInterval() const;

		void ReadData();
	};

//--------------------------------------------------------------------------------
class CSSConfig : public CSystemConfig
	{
	friend CSSConfigThread;

	protected:
		static int m_nRefCount;
		static CReadWriteObject m_readWriteObj;

		static CString m_sLogFilePath;
		static CString m_sLicenseFileDefault;
		static CString m_sEmailIP;
		static CString m_sEmailFrom;
		static CString m_sEmailTo;
		static CString m_sDongle;
		static CString m_sP3Serial;
		static CString m_sMainIP;

		static int m_nSecondsToLive;
		static DWORD m_nStatusPort;
		static DWORD m_nMainPort;
		static DWORD m_nTraceLevel;

		static DWORD m_nDBCleanupInterval;

		static CTime m_ctExpiration;
		static bool m_bIsBackup;
		static CString m_sMonitorUser;
		static CString m_sMonitorPass;

		static CString m_sBackupIP;
		static DWORD m_nBackupPort;

		static CSSConfigThread* m_pThread;
		static CRegKey regRoot_;
		static bool m_bBackupIsServing;
		static DWORD m_nReadCount;
		static DWORD m_nMinutesToWaitForDongle;

		static DWORD m_nConnectionPoolSize;
		static DWORD m_nMonitorPoolSize;

	public:
		CSSConfig(CSystemControlObject*);
		virtual ~CSSConfig();

		static CRegKey& getRegRoot() { return regRoot_; }

	private:
		friend bool CDBSubSystem::LoadFromFile();
		bool SetDataFromLicenseFile(const CLicenseFile*);
	};

#endif //_SSCONFIG_H_
