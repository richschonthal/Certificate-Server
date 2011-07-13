// TokenEditor.h : main header file for the TOKENEDITOR application
//

#if !defined(AFX_TOKENEDITOR_H__499C63AD_DD70_11D3_AF12_005004A1C5F3__INCLUDED_)
#define AFX_TOKENEDITOR_H__499C63AD_DD70_11D3_AF12_005004A1C5F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTokenEditorApp:
// See TokenEditor.cpp for the implementation of this class
//

class CTokenEditorApp : public CWinApp
{
public:
	CTokenEditorApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTokenEditorApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTokenEditorApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOKENEDITOR_H__499C63AD_DD70_11D3_AF12_005004A1C5F3__INCLUDED_)
