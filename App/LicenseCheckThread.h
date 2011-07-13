//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _LICENSECHECKTHREAD_H_
#define _LICENSECHECKTHREAD_H_

//--------------------------------------------------------------------------------
#include "SSThread.h"

class CSecuritySystem;

//--------------------------------------------------------------------------------
class CLicenseCheckThread : public CSSThread
	{
	private:
		bool m_bRunOk;
		bool m_bFirstRun;
		CTime m_ctLastCheckLicFile;

	public:
		CLicenseCheckThread(CSystemObject*);
	private:
		virtual bool Init();
		virtual bool MainLoop();
		virtual DWORD GetThreadLoopSleepInterval() const;
		void DoChecking();

		CSecuritySystem* GetParent() { return (CSecuritySystem*) CThreadObject::GetParent(); }
	};

#endif // _LICENSECHECKTHREAD_H_
