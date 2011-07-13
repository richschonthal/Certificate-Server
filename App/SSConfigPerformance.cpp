//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "SSConfigPerformance.h"

#include <ReadLock.h>

//--------------------------------------------------------------------------------
CSSConfigPerformance::CSSConfigPerformance(CSystemControlObject* pParent)
		: CSSConfig(pParent)
	{
	CReadLock lock(&CSSConfig::m_readWriteObj);

	m_nSecondsToLive = CSSConfig::m_nSecondsToLive;
	m_nTraceLevel = CSSConfig::m_nTraceLevel;
	m_nDBCleanupInterval = CSSConfig::m_nDBCleanupInterval;
	m_nConnectionPoolSize = CSSConfig::m_nConnectionPoolSize;
	m_nMonitorPoolSize = CSSConfig::m_nMonitorPoolSize;
	}
