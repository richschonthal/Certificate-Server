//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _SYSTEMMONITORHANDLERTHREAD_H_
#define _SYSTEMMONITORHANDLERTHREAD_H_

//--------------------------------------------------------------------------------
#include "SSThread.h"

class CMonitorSubSystem;
class CLockedSocket;

//--------------------------------------------------------------------------------
class CSystemMonitorHandlerThread : public CSSThread
	{
	private:
		bool m_bLoggedIn;
		CLockedSocket m_socket;
		CString m_sCurCmd;
		CTime m_ctLastActivity;

		CString m_sUser;
		CString m_sPass;

	public:
		CSystemMonitorHandlerThread(CMonitorSubSystem*);
		~CSystemMonitorHandlerThread();

		virtual void OnMessage(UINT, WPARAM, LPARAM);
		virtual bool MainLoop();

		virtual bool OnActivate();
		virtual bool OnDeactivate();

		bool DoCommand();

		void DoLoad(LPCTSTR);
		void DoList();
		void DoListAll();
		void DoPause();
		void DoContinue();
		void DoRevoke();
		void DoRevokeByIP(const CString&);
		void DoRevokeAll();
		void DoUser(LPCTSTR);
		void DoPass(LPCTSTR);
		void DoDongle();
		void DoConfigDump();

		virtual bool Init();

		virtual CMonitorSubSystem* GetSubSystem() { return (CMonitorSubSystem*) CSSThread::GetSubSystem(); }
	};

#endif //_SYSTEMMONITORHANDLERTHREAD_H_
