//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_SECURITYSYSTEM_H__3FFF6998_C933_11D3_AF10_005004A1C5F3__INCLUDED_)
#define AFX_SECURITYSYSTEM_H__3FFF6998_C933_11D3_AF10_005004A1C5F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
#include <System.h>

class CSSIOSubSystem;
class CClientSubSystem;
class CDBSubSystem;
class CSystemMonitorThread;
class CLicenseCheckThread;
class CBackupThread;

//--------------------------------------------------------------------------------
class CSecuritySystem : public CSystem  
	{
	private:
		CClientSubSystem* m_pClientSubSystem;
		CSSIOSubSystem* m_pIOSubSystem;
		CDBSubSystem* m_pDBSubSystem;
		CLicenseCheckThread* m_pLicCheckThread;
		CBackupThread* m_pBackupThread;

		time_t m_ttBackupCheckIn;

		CEvent m_evtDongleOk;
		CEvent m_evtLicenseOk;

		CString m_sLicenseFilename;

	public:
		CSecuritySystem();
		virtual ~CSecuritySystem();

		CClientSubSystem* GetClientSubSystem() { return m_pClientSubSystem; }
		CSSIOSubSystem* GetIOSubSystem() { return m_pIOSubSystem; }
		CDBSubSystem* GetDBSubSystem() { return m_pDBSubSystem; }

	public:
		enum
			{
			errBadLicenseFile = BeginErrorRange
			};

		enum
			{
			// number of minutes after system start before checking for dongle
			// this is only for the main server - if this is a backup server
			// then this wait period comes from the license file
			nMinutesToWaitForDongle = 5
			};

		// the time the system was started - this is always set
		CTime m_ctSystemStart;
		// the grace period before the dongle is required to continue running
		// on a main server this is set as m_ctSystemStart+MinutesToWaitForDongle
		// on a backup server, this is not set until SetIsServing is called
		// the value of CSSConfigBackup::m_nMinutesToWaitForDongle is used to calculate
		// the dongle deadline
		CTime m_ctDongleDeadline;

	private:
		// true when this server is serving
		// (ie not true when running as backup and the main server is operational)
		bool m_bIsServing;
		enum
			{
			SERVERMODE_UNKNOWN,
			SERVERMODE_MAIN,
			SERVERMODE_BACKUP
			} m_nServerMode;

	private:
		CBackupThread* GetBackupThread();

		friend class CLicenseCheckThread;
		void SetDongleStatus(bool);

		friend class CDBSubSystem;
		void SetLicenseStatus(bool);

		void CreateStartupEmailMessage(CString&) const;

	public:
		void SetIsServing(bool = true);
		bool IsServing() const;
		bool IsLicensed();

		// these must never be virtual since they're called from LoadFromFile during CSecuritySystem::CSecuritySystem
		void StartBackupMode();
		void StartMainserverMode();

		bool IsBackupServer() const;

		bool IsBackupRunning();
		void SetBackupIsRunning();

		CTime GetDongleDeadlineTime() const;

		void GetSystemConfigStringMessage(CStringArray&) const;

		void SetLicenseFilename(const CString&);
		CString GetLicenseFilename();
	};

//--------------------------------------------------------------------------------
inline CBackupThread* CSecuritySystem::GetBackupThread() { return m_pBackupThread; }
inline bool CSecuritySystem::IsBackupServer() const { return m_nServerMode == SERVERMODE_BACKUP; }
inline CTime CSecuritySystem::GetDongleDeadlineTime() const { return m_ctDongleDeadline; }
inline bool CSecuritySystem::IsServing() const { return m_bIsServing; }

#endif // !defined(AFX_SECURITYSYSTEM_H__3FFF6998_C933_11D3_AF10_005004A1C5F3__INCLUDED_)
