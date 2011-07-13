//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "SecuritySystem.h"
#include "MonitorSubSystem.h"
#include "SystemMonitorThread.h"
#include "SystemMonitorHandlerThread.h"
#include "SSConfigPerformance.h"

//--------------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CMonitorSubSystem, CThreadPoolSubSystem);

//--------------------------------------------------------------------------------
CMonitorSubSystem::CMonitorSubSystem(CSecuritySystem* pParent)
		: CSSSubSystem<CThreadPoolSubSystem>(pParent)
	{
	m_pHandler = new CSystemMonitorThread(this);
	m_pHandler->StartThread();

	CSSConfigPerformance config(this);

	for(UINT i = 0; i < config.m_nMonitorPoolSize; i++)
		AddThread(new CSystemMonitorHandlerThread(this));
	}

//--------------------------------------------------------------------------------
CMonitorSubSystem::~CMonitorSubSystem()
	{
	delete m_pHandler;
	}
