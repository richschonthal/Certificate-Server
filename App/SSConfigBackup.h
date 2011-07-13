//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _SSCONFIGBACKUP_CPP_
#define _SSCONFIGBACKUP_CPP_

//--------------------------------------------------------------------------------
#include "SSConfig.h"

//--------------------------------------------------------------------------------
class CSSConfigBackup : public CSSConfig
	{
	public:
		bool m_bIsBackup;
		CString m_sIP;
		DWORD m_nPort;
		int m_nMinutesToWaitForDongle;

	public:
		CSSConfigBackup(CSystemControlObject* = NULL);
	};

#endif // _SSCONFIGBACKUP_CPP_
