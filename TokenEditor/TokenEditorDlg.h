//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_TOKENEDITORDLG_H__499C63AF_DD70_11D3_AF12_005004A1C5F3__INCLUDED_)
#define AFX_TOKENEDITORDLG_H__499C63AF_DD70_11D3_AF12_005004A1C5F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
#include "BitList.h"

class CDelimFile;

//--------------------------------------------------------------------------------
class CTokenEditorDlg : public CDialog
	{
	// Construction
	public:
		CTokenEditorDlg(CWnd* pParent = NULL);	// standard constructor

	// Dialog Data
		//{{AFX_DATA(CTokenEditorDlg)
		enum { IDD = IDD_TOKENEDITOR_DIALOG };
		CScrollBar	m_scroll;
		//}}AFX_DATA

		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CTokenEditorDlg)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
		virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
	protected:
		// Generated message map functions
		//{{AFX_MSG(CTokenEditorDlg)
		virtual BOOL OnInitDialog();
		afx_msg void OnPaint();
		afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

	private:
		CDelimFile* m_pNameFile;
		CDelimFile* m_pBitFile;

		void PopulateCells();

	public:
		CBitList m_list;
	};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOKENEDITORDLG_H__499C63AF_DD70_11D3_AF12_005004A1C5F3__INCLUDED_)
