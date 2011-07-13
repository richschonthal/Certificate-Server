//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "ClientSubSystem.h"
#include "ClientConnectThread.h"
#include "ClientHandlerThread.h"
#include "SSIOSubSystem.h"
#include "DBSubSystem.h"
#include "SSConfigGeneral.h"

#include <SmallSocket.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//--------------------------------------------------------------------------------
CClientConnectThread::CClientConnectThread(CClientSubSystem* pSub)
		: CSSThread(pSub)
		, m_pSocket(NULL)
	{
	}

//--------------------------------------------------------------------------------
CClientConnectThread::~CClientConnectThread()
	{
	delete m_pSocket;
	}

//--------------------------------------------------------------------------------
bool CClientConnectThread::MainLoop()
	{
	if(m_pSocket != NULL && GetState() == STATE_RUN)
		{
		// check the socket for a connection
		// got a connection?
		if(m_pSocket->Listen(5))
			{
			CSmallSocket socket;
			if(m_pSocket->Accept(socket))
				{
				GetClientSubSystem()->PostNextThreadMessage(CClientSubSystem::MSG_NEWCONNECTION, (WPARAM) socket.Detach(), NULL);
				::Sleep(10);
				}
			}
		}

	return CheckState();
	}

//--------------------------------------------------------------------------------
DWORD CClientConnectThread::GetThreadLoopSleepInterval() const
	{
	return 100;
	}

//--------------------------------------------------------------------------------
bool CClientConnectThread::Init()
	{
	SetThreadPriority(THREAD_PRIORITY_ABOVE_NORMAL);

	CSSConfigGeneral config;

	m_pSocket = new CSmallSocket;
	if(! m_pSocket->Create(config.m_nMainPort, config.m_sMainIP))
		{
		CString sTemp;
		sTemp.Format("Unable to create main socket - Error = (%ld)", ::WSAGetLastError());
		((CSecurityServerApp*)AfxGetApp())->AddToMessageLog(sTemp);
		return false;
		}

	return true;
	}

//--------------------------------------------------------------------------------
void CClientConnectThread::Exit(bool)
	{
	}
