//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _SSTHREAD_H_
#define _SSTHREAD_H_

//--------------------------------------------------------------------------------
#include <ThreadObject.h>

class CSSIOSubSystem;
class CDBSubSystem;
class CClientSubSystem;
class CSecuritySystem;

//--------------------------------------------------------------------------------
class CSSThread : public CThreadObject
	{
	private:
		CSSIOSubSystem* m_pIO;
		CDBSubSystem* m_pDB;
		CClientSubSystem* m_pClient;

	public:
		CSSThread(CSystemObject*);

		CSSIOSubSystem* GetIO();
		CDBSubSystem* GetDB();
		CClientSubSystem* GetClientSubSystem();
		
		virtual CSecuritySystem* GetSystem();

		virtual DWORD GetThreadLoopSleepInterval() const	{ return 20; }
		virtual void Exit(bool);
	};

#endif // _SSTHREAD_H_
