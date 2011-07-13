//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _SYSTEMMONITORTHREAD_H_
#define _SYSTEMMONITORTHREAD_H_

//--------------------------------------------------------------------------------
#include <LockedSocket.h>
#include "SSThread.h"

class CMonitorSubSystem;
class CSystemMonitorHandlerThread;

//--------------------------------------------------------------------------------
class CSystemMonitorThread : public CSSThread
	{
	private:
		CLockedSocket* m_pSocket;

	public:
		CSystemMonitorThread(CMonitorSubSystem*);
		~CSystemMonitorThread();

		virtual bool Init();
		virtual bool MainLoop();

		virtual CMonitorSubSystem* GetSubSystem() { return (CMonitorSubSystem*) CSSThread::GetSubSystem(); }
	};

#endif //_SYSTEMMONITORTHREAD_H_
