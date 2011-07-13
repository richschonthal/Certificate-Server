//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _BACKUPTHREAD_CPP_
#define _BACKUPTHREAD_CPP_

//--------------------------------------------------------------------------------
#include "SSThread.h"
#include "SSConfigBackup.h"

class CBackupServerConnection;
class CSecuritySystem;
class CSmallSocket;

//--------------------------------------------------------------------------------
class CBackupThread : public CSSThread
	{
	private:
		CTime m_ctStarted;

	public:
		CBackupThread(CSystemObject*);
		virtual ~CBackupThread();

	private:
		virtual bool MainLoop();
		virtual bool Init();

		virtual DWORD GetThreadLoopSleepInterval() const;

		bool ConnectToMainServer();

		CSecuritySystem* GetParent() { return (CSecuritySystem*) CThreadObject::GetParent(); }
	};

#endif // _BACKUPTHREAD_CPP_
