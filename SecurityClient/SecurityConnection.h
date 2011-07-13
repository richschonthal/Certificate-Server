//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _SECURITYCONNECTION_H_
#define _SECURITYCONNECTION_H_

//--------------------------------------------------------------------------------
#include <ThreadObject.h>
#include <ReadWriteObject.h>

#include <Certificate.h>

#include "SecurityTarget.h"

#ifndef __AFXSOCK_H__
#include <afxsock.h>
#endif //__AFXSOCK_H__

#define INVALID_CERT_ID 0xFFFFFFFF

// a nice macro for creating new CSecurityConnection-derived classes
//--------------------------------------------------------------------------------
#define DECLARE_SECURITY_CONNECTION_CLASS(name) \
	name(LPCTSTR ip, UINT port, LPCTSTR backupip, UINT backupport) \
		: CSecurityConnection(ip, port, backupip, backupport) {}

//--------------------------------------------------------------------------------
class CCertificateContainer;
class CSmallSocket;

//--------------------------------------------------------------------------------
class CSecurityConnection : public CThreadObject, public CReadWriteObject, protected CSecurityTarget
	{
	friend class CSecurityClient;

	public:
		enum
			{
			ErrCantCreateSocket = HighestResultCode,
			ErrCantOpenSocket,
			ErrCantConnectToServer,
			ErrSocketReceive,
			ErrSocketSend,
			ErrNULLCertificate,
			ErrNULLSocket
			};

		enum
			{
			// LPARAM values for target's messages
			MsgNone,
			// new certificate issued
			MsgNew,
			// no certificate issued (server is runnning)
			MsgNotIssued,
			// certificate refreshed
			MsgRefreshed,
			// certificate was issued but has been revoked
			MsgRevoked,
			// certificate released by client app
			MsgReleased,
			// communication error
			MsgCommError,
			// the server is running but not serving
			MsgServerDown,
			// internal error
			MsgGeneralError
			};

	private:
		CCertificateContainer* m_pContainer;
		LPTSTR m_pIP;
		UINT m_nPort;
		LPTSTR m_pBackupIP;
		UINT m_nBackupPort;
		DWORD m_nSleepInterval;
		// true when there is currently a certificate
		bool m_bObtained;
		// set when at least one certificate has been obtained
		CEvent m_evtObtainedInitial;

		DWORD m_nUserClass;
		CDWordArray m_nTokenIdList;

	public:
		// when true, if the connection to the server is lost then
		// this connection tries to connect to the backup server
		// the default is true
		bool m_bAutoConnectToBackup;
		// fired when a certificate is obtained
		CEvent m_evtObtained;

	public:
		CSecurityConnection(LPCTSTR ip, UINT port, LPCTSTR backupip, UINT backupport);
		virtual ~CSecurityConnection();

	protected:
		bool SwitchToBackup();
		DWORD DoGetNewCertificate(DWORD nUserClass, DWORD nTokenId);
		bool ReGetCertificate();
		void SetObtained(bool = true);

		virtual bool Init();

		virtual bool MainLoop();
		virtual DWORD GetThreadLoopSleepInterval() const;

		bool TransactCertificate(CCertificate&);

	public:
		DWORD GetUserClass() const;

		LPCTSTR GetIP() const;
		UINT GetPort() const;

		LPCTSTR GetBackupIP() const;
		UINT GetBackupPort() const;

		CCertificate* GetCertificate() const;
		bool GetCertificateCopy(CCertificate*&);

		DWORD GetNewCertificate(DWORD nUserClass, DWORD nTokenId);
		DWORD GetNewCertificate(DWORD nUserClass, DWORD nTokenId, CCertificate*& pCertCopy);
		bool RefreshCertificate();
		bool ReleaseCertificate();

		bool AquireToken(ULONG);
		bool ReleaseToken(ULONG);

		bool HasObtainedInitial();
		static LPCTSTR GetMessageText(int, int);

		virtual bool OnNewCertificate();
		virtual bool OnRefresh();
		virtual bool OnRevoked();
		virtual bool OnError();

		bool IsValidCertificate();
		bool HasToken(DWORD);

		void EndConnection();

	protected:
		void DoPostMessage(int nMsg);
	};

//--------------------------------------------------------------------------------
inline bool CSecurityConnection::HasObtainedInitial()
	{ return ::WaitForSingleObject(m_evtObtainedInitial, 0) == WAIT_OBJECT_0; }

//--------------------------------------------------------------------------------
class CCertificateContainer : public CReadWriteObject
	{
	public:
		CCertificate* m_pCert;

		CCertificateContainer() : m_pCert(NULL) {}
		~CCertificateContainer() { delete m_pCert; }

		void DeleteCertificate() { delete m_pCert; m_pCert = NULL; }
	};

#endif // _SECURITYCONNECTION_H_
