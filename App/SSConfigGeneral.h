//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _SSCONFIGGENERAL_H_
#define _SSCONFIGGENERAL_H_

//--------------------------------------------------------------------------------
#include "SSConfig.h"

//--------------------------------------------------------------------------------
class CSSConfigGeneral : public CSSConfig
	{
	public:
		// stuff read from the registry
		CString m_sLogFilePath;
		CString m_sLicenseFileDefault;
		// stuff read from the license file
		CString m_sP3Serial;
		CString m_sDongle;
		CString m_sEmailIP;
		CString m_sEmailFrom;
		CString m_sEmailTo;
		CTime m_ctExpiration;
		DWORD m_nStatusPort;
		DWORD m_nMainPort;
		CString m_sMainIP;

	public:
		CSSConfigGeneral(CSystemControlObject* = NULL);
	};

#endif // _SSCONFIGGENERAL_H_
