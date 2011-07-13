//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "SecurityServer.h"
#include "SecurityServerService.h"
#include "SecuritySystem.h"
#include "LicenseFile.h"
#include "SSIOSubSystem.h"
#include "SSConfigGeneral.h"
#include "SSConfigPerformance.h"
#include "SSConfigBackup.h"
#include "dongle.h"

#include <CmdLine.h>
#include <ReadLock.h>
#include <WriteLock.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//--------------------------------------------------------------------------------
CSecurityServerApp g_application;
#ifndef _NOT_A_SERVICE
CSecurityServerService g_service;
#endif

//--------------------------------------------------------------------------------
static HANDLE g_hSingletonMutex = NULL;

//--------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CSecurityServerApp, CWinApp)
	//{{AFX_MSG_MAP(CSecurityServerApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
	ON_THREAD_MESSAGE(CNTService::MSG_PAUSE, DoServicePause)
	ON_THREAD_MESSAGE(CNTService::MSG_CONTINUE, DoServiceContinue)
	ON_THREAD_MESSAGE(CNTService::MSG_STOP, DoServiceStop)
	ON_THREAD_MESSAGE(CNTService::MSG_SHUTDOWN, DoServiceStop)
END_MESSAGE_MAP()

//--------------------------------------------------------------------------------
CSecurityServerApp::CSecurityServerApp()
		: m_pSystem(NULL)
	{
//	_crtBreakAlloc = 101;
	}

//--------------------------------------------------------------------------------
BOOL CSecurityServerApp::InitInstance()
	{
	BOOL b = ::SetPriorityClass(::GetCurrentProcess(), HIGH_PRIORITY_CLASS);

	if (!AfxSocketInit())
		{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
		}

	g_hSingletonMutex = ::CreateMutex(NULL, FALSE, "MCMSSecurityServerSingletonMutex");
	if(g_hSingletonMutex == NULL || GetLastError() == ERROR_ALREADY_EXISTS)
		{
#ifndef _NOT_A_SERVICE
		g_service.AddToMessageLog(_T("SecurityServer already running."));
#endif
		TRACE("SecurityServer already running\n");
		return FALSE;
		}

	// create an invisible window
	m_pMainWnd = new CWnd;
	m_pMainWnd->CreateEx(0, AfxRegisterWndClass(NULL), NULL, 0, 0, 0, 0, 0, NULL, NULL);

	AfxEnableControlContainer();

#ifdef _NOT_A_SERVICE

	m_pSystem = new CSecuritySystem;
	m_pSystem->GetIOSubSystem()->CreateLogFile("C:\\SStest.log", false);
	m_pSystem->GetIOSubSystem()->SetTraceLevel(IOMASK_1|IOMASK_2|IOMASK_3|IOMASK_4|IOMASK_5|IOMASK_6);

	m_pServiceThread = ::AfxBeginThread((AFX_THREADPROC) ServiceThread, (LPVOID) this);
	return TRUE;

#else
	CCmdLine cmdLine;

	int nIndex;
	if(cmdLine.HasParam('I', nIndex) || cmdLine.HasParam("-install", nIndex))
		g_service.InstallService();
	else
		if(cmdLine.HasParam('R', nIndex) || cmdLine.HasParam("-remove", nIndex))
			g_service.RemoveService();
		else
			if(cmdLine.HasParam('E', nIndex) || cmdLine.HasParam("-end", nIndex) || cmdLine.HasParam("-stop", nIndex))
				g_service.EndService();
			else
				{
				// must start the system first
				m_pSystem = new CSecuritySystem;
				if(m_pSystem->GetResultCode() != CResult::noErr)
					g_service.AddToMessageLog(_T("Exception occured during SecurityServer startup.  Check license file validity."));

				CSSConfigPerformance config;
				m_pSystem->GetIOSubSystem()->SetTraceLevel(config.m_nTraceLevel);

				m_pServiceThread = ::AfxBeginThread((AFX_THREADPROC) ServiceThread, (LPVOID) this);
				::Sleep(1000);
				g_service.ReportStatus(SERVICE_RUNNING);
				// dont destroy the window cuz we'll need it
				return TRUE;
				}

	// we dont need this window
	delete m_pMainWnd;
	m_pMainWnd = NULL;

	return FALSE;
#endif // _DEBUG
	}

//--------------------------------------------------------------------------------
int CSecurityServerApp::ExitInstance() 
	{
	if(m_pSystem != NULL)
		{
		m_pSystem->SetState(STATE_EXIT);
		m_pSystem->WaitForStateSync();
		}

	delete m_pSystem;
	m_pSystem = NULL;

	delete m_pMainWnd;
	m_pMainWnd = NULL;

	if(m_pServiceThread != NULL)
		::WaitForSingleObject(m_pServiceThread->m_hThread, 15000);

#ifndef _NOT_A_SERVICE
	g_service.ReportStatus(SERVICE_STOPPED);
#endif

	if(g_hSingletonMutex != NULL)
		{
		::CloseHandle(g_hSingletonMutex);
		g_hSingletonMutex = NULL;
		}

	return CWinApp::ExitInstance();
	}

//--------------------------------------------------------------------------------
void CSecurityServerApp::AddToMessageLog(LPCTSTR pMessage, WORD nEventType, DWORD nEventID)
	{
#ifndef _NOT_A_SERVICE
	g_service.AddToMessageLog(pMessage, nEventType, nEventID);
#endif
	}

//--------------------------------------------------------------------------------
UINT CSecurityServerApp::ServiceThread(LPVOID)
	{
#ifdef _NOT_A_SERVICE
	AfxMessageBox("Waiting...", MB_OK);

	AfxGetApp()->PostThreadMessage(WM_QUIT, 0, 0);

#else
	g_service.StartDispatcher();
#endif

	// doesnt return here until we exit
	return 0;
	}

//--------------------------------------------------------------------------------
void CSecurityServerApp::DoServicePause(WPARAM, LPARAM)
	{
#ifdef _NOT_A_SERVICE
	g_service.ReportStatus(SERVICE_PAUSED);
#endif
	}

//--------------------------------------------------------------------------------
void CSecurityServerApp::DoServiceContinue(WPARAM, LPARAM)
	{
#ifdef _NOT_A_SERVICE
	g_service.ReportStatus(SERVICE_RUNNING);
#endif
	}

//--------------------------------------------------------------------------------
void CSecurityServerApp::DoServiceStop(WPARAM, LPARAM)
	{
	PostThreadMessage(WM_QUIT, 0, 0);
	}
