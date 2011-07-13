//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _DBCLEANUPTHREAD_H_
#define _DBCLEANUPTHREAD_H_

//--------------------------------------------------------------------------------
#include "SSThread.h"

class CDBSubSystem;

//--------------------------------------------------------------------------------
class CDBCleanUpThread : public CSSThread
	{
	private:
		CDBSubSystem* m_pSub;

	public:
		CDBCleanUpThread(CDBSubSystem*);

		CDBSubSystem* GetSubSystem() const;

		virtual DWORD GetThreadLoopSleepInterval() const;

		virtual bool MainLoop();
		virtual void OnMessage(UINT, WPARAM, LPARAM);

		virtual bool Init();

		bool DoCleanUp();
	};

#endif // _DBCLEANUPTHREAD_H_
