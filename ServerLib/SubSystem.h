//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#if !defined(AFX_SUBSYSTEM_H__9F10BA2A_835B_11D3_B0D3_00A0CC271D0D__INCLUDED_)
#define AFX_SUBSYSTEM_H__9F10BA2A_835B_11D3_B0D3_00A0CC271D0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
#include "SystemControlObject.h"
#include <StdobjQueue.h>

//--------------------------------------------------------------------------------
class CSystem;
class CThreadObject;

//--------------------------------------------------------------------------------
class CSubSystem : public CSystemControlObject
	{
	protected:
		DECLARE_DYNAMIC(CSubSystem)
		friend class CThreadObject;

	protected:
		CEvent m_event;

	public:
		CSubSystem(CSystem*);
		virtual ~CSubSystem();

		// returns a ptr to the system this threadgroup is a part of
		CSystem* GetSystem();

		// called by CSystem
		virtual bool Init();
	};

//--------------------------------------------------------------------------------
class CThreadPoolSubSystem : public CSubSystem
	{
	DECLARE_DYNAMIC(CThreadPoolSubSystem)

	friend class CThreadObject;

	protected:
		CQueue<CThreadObject*, CThreadObject*> m_threads;
		LONG m_nBusyThreadCount;

	public:

		CThreadPoolSubSystem(CSystem*);
		virtual ~CThreadPoolSubSystem();

		// posts a message to all threads
		virtual bool PostMessage(UINT, WPARAM = 0, LPARAM = 0);

		// posts a message to the next thread in the pool
		virtual bool PostNextThreadMessage(UINT, WPARAM = 0, LPARAM = 0);

		// adds a thread to the thread pool
		virtual bool AddThread(CRuntimeClass*, int nCount);

		virtual void AddThread(CThreadObject*);

		LONG GetBusyThreadCount();

		bool WaitForAllToExit(DWORD = INFINITE);

		virtual CThreadObject* CreateThread();

	protected:
		void IncBusyThreadCount();
		void DecBusyThreadCount();
	};

//--------------------------------------------------------------------------------
inline LONG CThreadPoolSubSystem::GetBusyThreadCount() { return m_nBusyThreadCount; }
inline void CThreadPoolSubSystem::IncBusyThreadCount() { ::InterlockedIncrement(&m_nBusyThreadCount); }
inline void CThreadPoolSubSystem::DecBusyThreadCount() { ::InterlockedDecrement(&m_nBusyThreadCount); }

#endif // !defined(AFX_SUBSYSTEM_H__9F10BA2A_835B_11D3_B0D3_00A0CC271D0D__INCLUDED_)
