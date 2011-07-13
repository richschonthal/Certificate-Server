//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "SSConfigGeneral.h"

#include <ReadLock.h>

//--------------------------------------------------------------------------------
CSSConfigGeneral::CSSConfigGeneral(CSystemControlObject* pParent)
		: CSSConfig(pParent)
	{
	// get a read lock on the static vars
	CReadLock lock(&CSSConfig::m_readWriteObj);

	m_ctExpiration = CSSConfig::m_ctExpiration;

	m_sLogFilePath = CSSConfig::m_sLogFilePath;
	m_sLicenseFileDefault = CSSConfig::m_sLicenseFileDefault;
	m_sP3Serial = CSSConfig::m_sP3Serial;
	m_sDongle = CSSConfig::m_sDongle;
	m_sEmailIP = CSSConfig::m_sEmailIP;
	m_sEmailFrom = CSSConfig::m_sEmailFrom;
	m_sEmailTo = CSSConfig::m_sEmailTo;
	m_nStatusPort = CSSConfig::m_nStatusPort;
	m_nMainPort = CSSConfig::m_nMainPort;
	m_sMainIP = CSSConfig::m_sMainIP;
	}

