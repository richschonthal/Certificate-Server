//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "SSConfig.h"
#include "SSConfigBackup.h"

#include <ReadLock.h>
#include <WriteLock.h>

//--------------------------------------------------------------------------------
CSSConfigBackup::CSSConfigBackup(CSystemControlObject* pParent)
		: CSSConfig(pParent)
	{
	// get a read lock on the static vars
	CReadLock lock(&CSSConfig::m_readWriteObj);

	m_bIsBackup = CSSConfig::m_bIsBackup;
	m_sIP = CSSConfig::m_sBackupIP;
	m_nPort = CSSConfig::m_nBackupPort;
	m_nMinutesToWaitForDongle = CSSConfig::m_nMinutesToWaitForDongle;
	}
