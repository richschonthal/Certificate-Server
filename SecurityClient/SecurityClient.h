//--------------------------------------------------------------------------------
//
// Copyright (c) 2001 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _SECURITYCLIENT_H_
#define _SECURITYCLIENT_H_

//--------------------------------------------------------------------------------
#ifndef __AFXMT_H__
#include <afxmt.h>
#endif

#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
#endif

#include "SecurityTarget.h"
#include "SecurityConnection.h"

// from ServerLib
#include <ThreadObject.h>

// from stdobj
#include <Timer.h>

//--------------------------------------------------------------------------------
class CDongle;

//--------------------------------------------------------------------------------
class CSecurityClient : public CThreadObject, public CSecurityTarget
	{
	public:
		static CEvent m_evtMustExit;

	public:
		enum eDongleMode
			{
			// do not use the dongle at all
			DongleNone,
			// use the dongle if the security server is down
			// DongleFallback is the default
			DongleFallback,
			// use only the dongle
			DongleOnly
			};

	protected:
		enum
			{
			MsgIdCertificate = WM_USER + 10
			};

		CSecurityConnection* m_pConnection;
		CCertificateContainer m_container;

		DWORD m_nUser;
		DWORD m_nToken;
		
		eDongleMode m_nDongleMode;

		CEvent m_evtConnectComplete;
		CEvent m_evtWaitingForObtained;
		
		CPulseTimer* m_pConnectDeadlineTimer;

	public:
		// construction/destruction
		CSecurityClient();
		virtual ~CSecurityClient();

		// connect to server and configure for optional backup server
		// also optionally change the dongle mode
		bool Connect(DWORD nUser, DWORD nToken, LPCTSTR pIP, DWORD nPort,
			LPCTSTR pBIP = NULL, DWORD nBPort = 0, eDongleMode = DongleFallback);

		// dongle-only connect
		bool Connect();

		// check on certificate and/or dongle status
		bool IsLicensed();

		// token management
		bool AquireToken(DWORD);
		bool HasToken(DWORD);
		bool ReleaseToken(DWORD);
		bool HasPermission(DWORD nTokenId, UINT nBit);

		// saved parameter access
		DWORD GetUserLevel() const	{ return m_nUser; }
		DWORD GetTokenId() const	{ return m_nToken; }

		eDongleMode GetDongleMode() const	{ return m_nDongleMode; }

		bool IsInDongleMode();
		bool IsObtained();
		bool IsWaitingForObtained();
		bool HasObtainedInitial();
		bool IsConnectComplete();
		bool IsMustExit();

	private:
		CSecurityConnection* GetConnection() const { return m_pConnection; }

		virtual DWORD GetThreadLoopSleepInterval() const	{ return 2000; }

		virtual bool MainLoop();
		virtual bool Init();

		virtual void OnMessage(UINT, WPARAM, LPARAM);

	protected:
		void SetDongleMode(eDongleMode n)	{ m_nDongleMode = n; }

		bool MainLoopCertificateClientMode();
		bool MainLoopLostCertificateClientMode();
		bool MainLoopDongleOnlyClientMode();

		virtual bool DoSecurityMessage(LONG);
		virtual void SetTerminate();

		void SetDeadlineTimer();
		void CancelDeadlineTimer();

		bool IsConnectDeadline() const;
		// dongle management
		// override to provide your own dongle class
		virtual CDongle* GetDongle();
		// override to do further dongle validation
		virtual bool IsValidDongle();

		void SwitchToDongleOnlyMode();

		// called when a communication error occurs with the server
		virtual void OnCommError();
		// called when m_evtMustExit is signaled
		virtual void OnMustExit();
		// called when IsValidDongle is called
		// bIsValid == true when the dongle is ok
		virtual void OnCheckDongle(bool bIsValid);
		//returns the # of miliseconds client will wait to reconnect
		//before reverting to dongle or exiting
		virtual DWORD GetMaxReconnectWaitTime() const;
	};

//--------------------------------------------------------------------------------
inline bool CSecurityClient::IsConnectDeadline() const { return m_pConnectDeadlineTimer != NULL && m_pConnectDeadlineTimer->IsSignaled(); }
inline bool CSecurityClient::IsConnectComplete() { return ::WaitForSingleObject(m_evtConnectComplete, 0) == WAIT_OBJECT_0; }
inline bool CSecurityClient::HasObtainedInitial() { return m_pConnection != NULL && m_pConnection->HasObtainedInitial(); }
inline bool CSecurityClient::IsMustExit() { return ::WaitForSingleObject(m_evtMustExit, 0) == WAIT_OBJECT_0; }


#ifdef _SECCLI_AUTOLIBS_
#ifndef _SECCLIENTBUILD_
#ifdef _DEBUG
#define LIBSTUB "D.lib"
#else
#define LIBSTUB ".lib"
#endif

#pragma comment(lib, "SecurityServerLib" LIBSTUB)
#pragma comment(lib, "ServerLib" LIBSTUB)
#pragma comment(lib, "stdobj" LIBSTUB)
#pragma comment(lib, "stdobj" LIBSTUB)

#undef LIBSTUB

#endif //_SECCLIENTBUILD_

#ifndef _SECCON_NODONGLE
// add libc to your linker's "ignore libraries" option if you get errors like this:
// libc.lib(crt0dat.obj) : error LNK2005: _exit already defined in msvcrtd.lib(MSVCRTD.dll)
#pragma comment(lib, "hlw32lmc.lib")
#endif //_SECCON_NODONGLE

#endif //_SECCON_AUTOLIBS_

#endif // _SECURITYCLIENT_H_
