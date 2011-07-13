//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _SSCONFIGPERFORMANCE_H_
#define _SSCONFIGPERFORMANCE_H_

//--------------------------------------------------------------------------------
#include "SSConfig.h"

//--------------------------------------------------------------------------------
class CSSConfigPerformance : public CSSConfig
	{
	public:
		int m_nSecondsToLive;
		DWORD m_nTraceLevel;
		DWORD m_nDBCleanupInterval;
		DWORD m_nConnectionPoolSize;
		DWORD m_nMonitorPoolSize;

	public:
		CSSConfigPerformance(CSystemControlObject* = NULL);
	};

#endif // _SSCONFIGPERFORMANCE_H_
