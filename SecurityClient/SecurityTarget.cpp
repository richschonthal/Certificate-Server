//--------------------------------------------------------------------------------
//
// Copyright (c) 2001 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "SecurityTarget.h"
#include "ThreadObject.h"

//--------------------------------------------------------------------------------
void CSecurityTarget::SetTarget(CWinThread* pThread, UINT nMsg)
	{
	m_target.pThrd = pThread;
	m_nMsg = nMsg;
	m_nTargetType = TargetThread;
	}

//--------------------------------------------------------------------------------
void CSecurityTarget::SetTarget(CWnd* pWnd, UINT nMsg)
	{
	m_target.pWnd = pWnd;
	m_nMsg = nMsg;
	m_nTargetType = TargetCWnd;
	}

//--------------------------------------------------------------------------------
void CSecurityTarget::SetTarget(CThreadObject* pThreadObj, UINT nMsg)
	{
	m_target.pThrdObj = pThreadObj;
	m_nMsg = nMsg;
	m_nTargetType = TargetThreadObj;
	}

//--------------------------------------------------------------------------------
void CSecurityTarget::SetTarget(HWND hWnd, UINT nMsg)
	{
	m_target.hWnd = hWnd;
	m_nMsg = nMsg;
	m_nTargetType = TargetHWND;
	}

//--------------------------------------------------------------------------------
void CSecurityTarget::SetTarget(DWORD nThreadId, UINT nMsg)
	{
	m_target.nThreadId = nThreadId;
	m_nMsg = nMsg;
	m_nTargetType = TargetThreadId;
	}

//--------------------------------------------------------------------------------
void CSecurityTarget::SetTargetNone()
	{
	memset(&m_target, 0, sizeof(m_target));
	m_nMsg = 0;
	m_nTargetType = TargetNone;
	}

//--------------------------------------------------------------------------------
void CSecurityTarget::DoPostMessage(WPARAM p1, LPARAM p2)
	{
	switch(m_nTargetType)
		{
		case TargetThreadObj:
			// we dont want to use CThreadObj::PostMessage because this may
			// start the thread and we may be posting to a threads that been killed...
			::PostThreadMessage(m_target.pThrdObj->GetThreadId(), m_nMsg, p1, p2);
			break;
		case TargetThread:
			m_target.pThrd->PostThreadMessage(m_nMsg, p1, p2);
			break;
		case TargetCWnd:
			m_target.pWnd->PostMessage(m_nMsg, p1, p2);
			break;
		case TargetHWND:
			::PostMessage(m_target.hWnd, m_nMsg, p1, p2);
			break;
		case TargetThreadId:
			::PostThreadMessage(m_target.nThreadId, m_nMsg, p1, p2);
			break;
		default:
			break;
		}
	}

