//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _SSCONFIGMONITOR_H_
#define _SSCONFIGMONITOR_H_

//--------------------------------------------------------------------------------
#include "SSConfig.h"

//--------------------------------------------------------------------------------
class CSSConfigMonitor : public CSSConfig
	{
	public:
		DWORD m_nPort;
		CString m_sUser;
		CString m_sPass;

	public:
		CSSConfigMonitor(CSystemControlObject* = NULL);
	};

#endif //_SSCONFIGMONITOR_H_