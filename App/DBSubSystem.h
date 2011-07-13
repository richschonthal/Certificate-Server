//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_DBSUBSYSTEM_H__0BCE52F7_D26F_11D3_AF12_005004A1C5F3__INCLUDED_)
#define AFX_DBSUBSYSTEM_H__0BCE52F7_D26F_11D3_AF12_005004A1C5F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
#include "SecurityServer.h"
#include "SSSubSystem.h"
#include "CertificateMaster.h"
#include <Certificate.h>

class CDBTokenInfoMap;
class CDBCleanUpThread;
class CMagicNumber;

//--------------------------------------------------------------------------------
class CDBSubSystem : public CSSSubSystem<CSubSystem>
	{
	public:
		DECLARE_DYNAMIC(CDBSubSystem);

	friend class CBackupThread;
	friend class CDBCleanUpThread;

	private:
		CDBCertificateMasters* m_pDB;
		CDBTokenInfoMap* m_pTokenInfoMap;
		DWORD m_nNextCertId;
		CDBCleanUpThread* m_pCleanUpThread;
		// used to generate and track "magic" numbers
		CMagicNumber* m_pMagic;

		// used for limiting the num of emails sent within a certain time period
		// ie we dont want to send 1 status 4 email every second
		// we'd rather send 1 every 10 minutes or every m_nEmailInterval minutes
		static CTime m_ctLastSent[2];
		const int m_nEmailInterval;

		CEvent m_evtLicenseOk;

	private:
		bool CanSendEmailNow(UCHAR nIndex);

		bool LicenseFileFailed(int);

		bool IsLicenseOk();

	public:
		enum
			{
			MsgCleanUp = WM_USER
			};

		CDBSubSystem(CSecuritySystem*);
		virtual ~CDBSubSystem();

		CMagicNumber* GetMagNumGen() const;
		CDBCertificateMasters* GetCertMasters() const;
		CDBTokenInfoMap* GetTokenInfoMap() const;

		DWORD GetNextCertificateId();

		CCertificateMaster* FindCertMaster(const CCertificate&);

		CCertificateMaster* IssueCertificate(const CCertificate&, LPCTSTR ipaddress);
		CCertificateMaster* RefreshCertificate(const CCertificate&, LPCTSTR ipaddress);
		bool ReleaseCertificate(const CCertificate&, LPCTSTR ipaddress);
		bool ReleaseCertificate(int);
		int ReleaseCertificate(LPCTSTR);
		int ReleaseAllCertificates();

		bool IssueToken(CCertificateMaster*&, ULONG);
		bool ReleaseToken(const CCertificate&, ULONG);

		bool LoadFromFile();

	public:
		virtual bool PostMessage(UINT, WPARAM = 0, LPARAM = 0);
	};

//--------------------------------------------------------------------------------
inline bool CDBSubSystem::IsLicenseOk() { return ::WaitForSingleObject(m_evtLicenseOk, 0) == WAIT_OBJECT_0; }

#endif // !defined(AFX_DBSUBSYSTEM_H__0BCE52F7_D26F_11D3_AF12_005004A1C5F3__INCLUDED_)
