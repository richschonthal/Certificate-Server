//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include <NTService.h>
#include <Directory.h>
#include "SecuritySystem.h"
#include "SSIOSubSystem.h"
#include "SSIOThread.h"
#include "SSQuickMail.h"
#include "IOThread.h"
#include "SSConfigGeneral.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//--------------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CSSIOSubSystem, CIOSubSystem);

//--------------------------------------------------------------------------------
CSSIOSubSystem::CSSIOSubSystem(CSecuritySystem* pParent)
		: CSSSubSystem<CIOSubSystem>(pParent)
		, m_ctLastLogCreate(0)
	{
	}

//--------------------------------------------------------------------------------
CSSIOSubSystem::~CSSIOSubSystem()
	{
	}

//--------------------------------------------------------------------------------
void CSSIOSubSystem::WriteLogFileHeader()
	{
	CString sLine('-', 80);
	sLine += "\r\n";

	m_pLogFile->WriteString(sLine);
	m_pLogFile->WriteString("MarkCare Medical Systems, Inc.\r\n");
	CString sTemp;
	sTemp.Format("Security Server Log File started: %s\r\n", CTime::GetCurrentTime().Format("%d-%b-%Y @ %H:%M"));
	m_pLogFile->WriteString(sTemp);
	m_pLogFile->WriteString(sLine);
	m_pLogFile->Flush();
	}

//--------------------------------------------------------------------------------
CThreadObject* CSSIOSubSystem::CreateThread()
	{
	return (CThreadObject*) new CSSIOThread(this);
	}

//--------------------------------------------------------------------------------
CStdioFile* CSSIOSubSystem::GetLogFilePtr()
	{
	CSSConfigGeneral config;
	if(config.m_sLogFilePath.IsEmpty())
		{
		delete m_pLogFile;
		m_pLogFile = NULL;
		return NULL;
		}

	// if there's no logfile currently, or its been 24 hours since it was created
	// or the log path has changed then create a new log file
	CTime ctNow(CTime::GetCurrentTime());
	if(m_pLogFile == NULL || (ctNow - m_ctLastLogCreate).GetTotalHours() > 23 || m_sLastLogPath != config.m_sLogFilePath)
		{
		m_sLastLogPath = config.m_sLogFilePath;
		m_ctLastLogCreate = ctNow;
		CString sTemp("SecServLog");
		sTemp += ctNow.Format("%Y%m%d.log");
		if(config.m_sLogFilePath.Right(1) != '\\')
			config.m_sLogFilePath += '\\';
		CDirectory::CreatePath(config.m_sLogFilePath);
		if(! CreateLogFile(config.m_sLogFilePath + sTemp))
			return NULL;
		}

	return m_pLogFile;
	}
