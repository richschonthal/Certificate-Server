//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_SECURITYSERVER_H__76AE78F5_C46F_11D3_AF0C_005004A1C5F3__INCLUDED_)
#define AFX_SECURITYSERVER_H__76AE78F5_C46F_11D3_AF0C_005004A1C5F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include <NTService.h>
#include "SecuritySystem.h"
#include "SSIOSubSystem.h"

//--------------------------------------------------------------------------------
class CSecurityServerApp : public CWinApp
	{
	public:
		CSecurityServerApp();

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CSecurityServerApp)
	public:
		virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	// Implementation

		//{{AFX_MSG(CSecurityServerApp)
			// NOTE - the ClassWizard will add and remove member functions here.
			//    DO NOT EDIT what you see in these blocks of generated code !
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

	private:
		CWinThread* m_pServiceThread;
		static UINT ServiceThread(LPVOID);
		CSecuritySystem* m_pSystem;
		CReadWriteObject m_lock;

		void DoServicePause(WPARAM, LPARAM);
		void DoServiceContinue(WPARAM, LPARAM);
		void DoServiceStop(WPARAM, LPARAM);
	public:
		void AddToMessageLog(LPCTSTR Message, WORD EventType = EVENTLOG_ERROR_TYPE, DWORD dwEventID = -1);
	};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SECURITYSERVER_H__76AE78F5_C46F_11D3_AF0C_005004A1C5F3__INCLUDED_)
