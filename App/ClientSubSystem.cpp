//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "SecuritySystem.h"
#include "ClientSubSystem.h"
#include "ClientHandlerThread.h"
#include "ClientConnectThread.h"
#include "SSIOSubSystem.h"
#include "CertificateMaster.h"

#include "SSConfigPerformance.h"

#include <WriteLock.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//--------------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CClientSubSystem, CSubSystem);

//--------------------------------------------------------------------------------
CClientSubSystem::CClientSubSystem(CSecuritySystem* pParent)
		: CSSSubSystem<CThreadPoolSubSystem>(pParent)
	{
	m_pConfig = NULL;

	CSSConfigPerformance config(this);
	for(UINT i = 0; i < config.m_nConnectionPoolSize; i++)
		AddThread(new CClientHandlerThread(this));

	// start our 'main' thread
	m_pConnectionThread = new CClientConnectThread(this);
	m_pConnectionThread->StartThread();
	}

//--------------------------------------------------------------------------------
CClientSubSystem::~CClientSubSystem()
	{
	m_pConnectionThread->EndThread();
	m_pConnectionThread->WaitForThreadExit();
	delete m_pConnectionThread;
	}

//--------------------------------------------------------------------------------
CSSIOSubSystem* CClientSubSystem::GetIO()
	{
	return ((CSecuritySystem*) GetParent())->GetIOSubSystem();
	}
