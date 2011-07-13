//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#include "stdafx.h"
#include "ThreadObject.h"
#include "SubSystem.h"
#include "System.h"
 
#include <ReadLock.h>

#include <Process.h>

//--------------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(CThreadObject, CObject);

//--------------------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//--------------------------------------------------------------------------------
CThreadObject::CThreadObject(CSystemObject* pParent)
		: CSystemObject(pParent)
		, m_evtThreadBusy(FALSE, TRUE)
		, m_hThread(NULL)
	{
	memset(&m_msgCur, 0, sizeof(MSG));

	if(pParent != NULL)
		m_pState = pParent->m_pState;
	}

//--------------------------------------------------------------------------------
CThreadObject::~CThreadObject()
	{
	if(m_pParent != NULL)
		if(m_pState == m_pParent->m_pState)
			m_pState = NULL;
	}

//--------------------------------------------------------------------------------
bool CThreadObject::StartThread()
	{
	if(m_hThread != NULL || m_hThread == INVALID_HANDLE_VALUE)
		return false;

	m_event.ResetEvent();

	// start our control loop
	m_hThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) StaticMain, this, 0, &m_nThreadId);

	if(m_hThread == INVALID_HANDLE_VALUE)
		return false;

	// wait for StaticMain to start
	DWORD nRv = ::WaitForSingleObject(m_event, INFINITE);

	return nRv == WAIT_OBJECT_0;
	}

//--------------------------------------------------------------------------------
bool CThreadObject::EndThread()
	{
	if(this == NULL)
		return false;
	if(m_hThread == NULL || m_hThread == INVALID_HANDLE_VALUE)
		return false;
	::PostThreadMessage(m_nThreadId, WM_QUIT, 0, 0);
	::WaitForSingleObject(m_hThread, INFINITE);
	::CloseHandle(m_hThread);
	m_hThread = INVALID_HANDLE_VALUE;
	return true;
	}

//--------------------------------------------------------------------------------
UINT CThreadObject::StaticMain(CThreadObject* that)
	{
	HANDLE hTimer = ::CreateWaitableTimer(NULL, FALSE, NULL);

	// if we're part of a system then we add ourselves as a state client
	// so that the subsystem knows how many threads to wait for when
	// changing states (for state sync)
	CSystemObject* pParent = that->GetParent();
	if(pParent != NULL)
		pParent->AddStateClient();

	// set up the timer that we'll use for the main loop interval
	DWORD nSleepTime = that->GetThreadLoopSleepInterval();

	LARGE_INTEGER liTimerDue;
	memset(&liTimerDue, 0, sizeof(liTimerDue));

	// negative values are relative time - this will make the timers first signal
	// happen (almost) immediatly
	liTimerDue.LowPart = -1;

	if(nSleepTime != 0)
		::SetWaitableTimer(hTimer, &liTimerDue, nSleepTime, NULL, NULL, FALSE);

	int nHandCount = 1;
	HANDLE handsToWaitFor[2] = { hTimer };

	if(that->m_pState != NULL)
		{
		nHandCount = 2;
		handsToWaitFor[1] = that->m_pState->m_evtChanged;
		}

	DWORD nLastState = 0xFFFFFFFF;
	DWORD nTimerWaitId = WAIT_OBJECT_0;
	DWORD nStateWaitId = WAIT_OBJECT_0 + nHandCount - 1;

	// signal that the thread is created
	that->m_event.SetEvent();
	bool bInitOk = that->Init();

	if(bInitOk)
		TRY
			{
			for(;;)
				{
				DWORD nWait = ::MsgWaitForMultipleObjects(nHandCount, handsToWaitFor, FALSE, 20, QS_ALLEVENTS);

				// for debugging
				if(nWait == WAIT_FAILED || nWait == WAIT_ABANDONED || nWait == WAIT_TIMEOUT)
					continue;

				if(PeekMessage(&that->m_msgCur, NULL, 0, 0, PM_REMOVE))
					{
					UINT nMsg = that->m_msgCur.message;

					if(nMsg == WM_DESTROY)
						that->OnMessage(WM_QUIT, that->m_msgCur.wParam, that->m_msgCur.lParam);
					else	
						that->OnMessage(that->m_msgCur.message, that->m_msgCur.wParam, 
								that->m_msgCur.lParam);

					if(nMsg == WM_QUIT || nMsg == WM_DESTROY)
						break;
					}

				if(nWait == nTimerWaitId)
					{
					if(nSleepTime != 0)
						if(! that->MainLoop())
							break;

					// did the sleep interval change?
					if(nSleepTime != that->GetThreadLoopSleepInterval())
						{
						::CancelWaitableTimer(hTimer);

						nSleepTime = that->GetThreadLoopSleepInterval();
						liTimerDue.LowPart = -1;
						if(nSleepTime != 0)
							::SetWaitableTimer(hTimer, &liTimerDue, nSleepTime, NULL, NULL, FALSE);
						}
					continue;
					}

				if(nWait == nStateWaitId)
					{
					DWORD nState = that->GetState();
					that->OnStateChange(nState);
					if(! that->CheckState())
						break;
					}
				}
			}
		CATCH_ALL(e)
			{
			}
		END_CATCH_ALL

	::CancelWaitableTimer(hTimer);
	::CloseHandle(hTimer);

	that->Exit(bInitOk);

	that->AcknowledgeStateChange();

	if(pParent != NULL)
		pParent->RemoveStateClient();

	UINT nRv = that->GetResultCode();

	// basically this is 'delete this' which is kinda a no-no
	// but this is the one case i can think of where it's warrented
	// this class provides an easy to use thread encapsulation so
	// the thread should be able to delete itself, no?
	if(that->m_msgCur.message == WM_DESTROY)
		delete that;

	return nRv;
	}

//--------------------------------------------------------------------------------
CSubSystem* CThreadObject::GetSubSystem()
	{
	// find the nearest sub system in the parent chain
	for(CSystemObject* pPar = GetParent(); pPar != NULL; pPar = pPar->GetParent())
		{
		if(pPar->GetRuntimeClass()->IsDerivedFrom(RUNTIME_CLASS(CSubSystem)))
			return (CSubSystem*) pPar;
		}

	return NULL;
	}

//--------------------------------------------------------------------------------
CSystem* CThreadObject::GetSystem()
	{
	// find the nearest system in the parent chain
	for(CSystemObject* pPar = GetParent(); pPar != NULL; pPar = pPar->GetParent())
		{
		if(pPar->GetRuntimeClass()->IsDerivedFrom(RUNTIME_CLASS(CSystem)))
			return (CSystem*) pPar;
		}

	return NULL;
	}

//--------------------------------------------------------------------------------
bool CThreadObject::OnActivate()
	{
	return true;
	}

//--------------------------------------------------------------------------------
bool CThreadObject::OnDeactivate()
	{
	GetEvent().SetEvent();
	return true;
	}

//--------------------------------------------------------------------------------
void CThreadObject::OnStateChange(DWORD)
	{
	AcknowledgeStateChange();
	}

//--------------------------------------------------------------------------------
void CThreadObject::OnMessage(UINT, WPARAM, LPARAM)
	{
	return;
	}

//--------------------------------------------------------------------------------
DWORD CThreadObject::GetThreadLoopSleepInterval() const
	{
	// the default is to not have a MainLoop so we return 0
	// see StaticMain (waitable timer stuff) for details
	return 100;
	}

//--------------------------------------------------------------------------------
DWORD CThreadObject::GetPauseWaitSleepInterval() const
	{
	return 0;
	}

//--------------------------------------------------------------------------------
bool CThreadObject::MainLoop()
	{
	return CheckState();
	}

//--------------------------------------------------------------------------------
bool CThreadObject::Init()
	{
	return true;
	}

//--------------------------------------------------------------------------------
void CThreadObject::Exit(bool)
	{
	}

//--------------------------------------------------------------------------------
void CThreadObject::SetThreadBusy(bool bIsBusy)
	{
	if(bIsBusy)
		{
		CSystemObject* pParent = GetParent();
		if(pParent != NULL)
			{
			if(pParent->IsKindOf(RUNTIME_CLASS(CThreadPoolSubSystem)))
				((CThreadPoolSubSystem*) pParent)->IncBusyThreadCount();
			}
		m_evtThreadBusy.SetEvent();
		OnActivate();
		}
	else
		{
		CSystemObject* pParent = GetParent();
		if(pParent != NULL)
			{
			if(pParent->IsKindOf(RUNTIME_CLASS(CThreadPoolSubSystem)))
				((CThreadPoolSubSystem*) pParent)->DecBusyThreadCount();
			}
		m_evtThreadBusy.ResetEvent();
		OnDeactivate();
		}
	}

//--------------------------------------------------------------------------------
bool CThreadObject::CheckState()
	{
	switch(GetState())
		{
		default:
		case STATE_INIT:
		case STATE_RUN:
			return true;
		case STATE_UNKNOWN:
		case STATE_ERROR:
		case STATE_EXIT:
		case STATE_STOP:
			return false;
		case STATE_PAUSE:
			return true;
		}
	}
