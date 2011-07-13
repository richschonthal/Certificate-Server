//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#include "stdafx.h"
#include "TokenEditor.h"
#include "MainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//--------------------------------------------------------------------------------
CTokenEditorApp theApp;

//--------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CTokenEditorApp, CWinApp)
	//{{AFX_MSG_MAP(CTokenEditorApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

//--------------------------------------------------------------------------------
CTokenEditorApp::CTokenEditorApp()
	{
	}
#include "GenRandTbl.h"

//--------------------------------------------------------------------------------
BOOL CTokenEditorApp::InitInstance()
	{
	if (!AfxSocketInit())
		{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
		}

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CGenRandomTable tbl;

	CMainDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
	}
