//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "SSConfig.h"
#include "SSConfigMonitor.h"

#include <ReadLock.h>

//--------------------------------------------------------------------------------
CSSConfigMonitor::CSSConfigMonitor(CSystemControlObject* pParent)
		: CSSConfig(pParent)
	{
	// get a read lock on the static vars
	CReadLock lock(&CSSConfig::m_readWriteObj);

	m_nPort = CSSConfig::m_nStatusPort;
	m_sUser = CSSConfig::m_sMonitorUser;
	m_sPass = CSSConfig::m_sMonitorPass;
	}
