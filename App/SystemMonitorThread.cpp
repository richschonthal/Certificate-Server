//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "SystemMonitorThread.h"
#include "SystemMonitorHandlerThread.h"
#include "SSIOSubSystem.h"
#include "SecuritySystem.h"
#include "MonitorSubSystem.h"
#include "SSConfigGeneral.h"

//--------------------------------------------------------------------------------
CSystemMonitorThread::CSystemMonitorThread(CMonitorSubSystem* pParent)
		: CSSThread(pParent)
		, m_pSocket(new CLockedSocket)
	{

	}

//--------------------------------------------------------------------------------
CSystemMonitorThread::~CSystemMonitorThread()
	{
	if(::GetCurrentThreadId() != m_nThreadId)
		::WaitForSingleObject(m_hThread, INFINITE);
	delete m_pSocket;
	}

//--------------------------------------------------------------------------------
bool CSystemMonitorThread::Init()
	{
	SetThreadPriority(THREAD_PRIORITY_BELOW_NORMAL);

	CSSConfigGeneral config;

	if(m_pSocket->Create(config.m_nStatusPort) == 0)
		{
		GetIO()->FormatOutput(IOMASK_ERR, "Failed to create monitor socket (%ld)", config.m_nStatusPort);
		return false;
		}

	return true;
	}

//--------------------------------------------------------------------------------
bool CSystemMonitorThread::MainLoop()
	{
	if(m_pSocket == NULL)
		return false;

	if(m_pSocket->Listen(5) && GetState() == STATE_RUN)
		{
		CLockedSocket socket;
		if(m_pSocket->Accept(socket))
			{
			SOCKET sock = socket.Detach();
			if(! GetSubSystem()->PostNextThreadMessage(CMonitorSubSystem::MSG_NEWCONNECTION, (WPARAM) sock))
				{
				socket.Attach(sock);
				CString sTemp("Sorry, no more connection handlers available\r\nTry again later\r\n");
				socket.Send(sTemp);
				}
			}
		}

	return CheckState();
	}
