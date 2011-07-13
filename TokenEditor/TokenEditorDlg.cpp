//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "TokenEditor.h"
#include "TokenEditorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//--------------------------------------------------------------------------------
CTokenEditorDlg::CTokenEditorDlg(CWnd* pParent /*=NULL*/)
		: CDialog(CTokenEditorDlg::IDD, pParent)
	{
	//{{AFX_DATA_INIT(CTokenEditorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	}

//--------------------------------------------------------------------------------
void CTokenEditorDlg::DoDataExchange(CDataExchange* pDX)
	{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTokenEditorDlg)
	DDX_Control(pDX, IDC_SCROLLBAR1, m_scroll);
	//}}AFX_DATA_MAP
	}

//--------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CTokenEditorDlg, CDialog)
	//{{AFX_MSG_MAP(CTokenEditorDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//--------------------------------------------------------------------------------
BOOL CTokenEditorDlg::OnInitDialog()
	{
	CDialog::OnInitDialog();

	CScrollBar* pBar = (CScrollBar*) GetDlgItem(IDC_SCROLLBAR1);
	m_list.ConfigureScrollBar(pBar);

	return TRUE;  // return TRUE  unless you set the focus to a control
	}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

//--------------------------------------------------------------------------------
void CTokenEditorDlg::OnPaint() 
	{
	if(! IsIconic())
		{
		PopulateCells();
		CDialog::OnPaint();
		}
	}

//--------------------------------------------------------------------------------
void CTokenEditorDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
	{
	int nCurPos = pScrollBar->GetScrollPos();

	switch(nSBCode)
		{
		case SB_BOTTOM:
			break;
		case SB_ENDSCROLL:
			break;
		case SB_LINEDOWN:
			m_list.DoLineDown(nPos, pScrollBar);
			PopulateCells();
			break;
		case SB_PAGEDOWN:
			m_list.DoPageDown(nPos, pScrollBar);
			PopulateCells();
			break;
		case SB_LINEUP:
			m_list.DoLineUp(nPos, pScrollBar);
			PopulateCells();
			break;
		case SB_PAGEUP:
			m_list.DoPageUp(nPos, pScrollBar);
			PopulateCells();
			break;
		case SB_THUMBPOSITION:
			m_list.DoThumbPos(nPos, pScrollBar);
			PopulateCells();
			break;
		case SB_THUMBTRACK:
			m_list.DoThumbTrack(nPos, pScrollBar);
			PopulateCells();
			break;
		case SB_TOP:
			break;
		}

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
	}

//--------------------------------------------------------------------------------
class CCells
	{
	public:
		int m_nLabelId;
		int m_nCheckId;
		int m_nEditId;
	};

//--------------------------------------------------------------------------------
CCells g_cells[6] = 
	{
	IDC_BITNO_1, IDC_ON1, IDC_BITNAME_1,
	IDC_BITNO_2, IDC_ON2, IDC_BITNAME_2,
	IDC_BITNO_3, IDC_ON3, IDC_BITNAME_3,
	IDC_BITNO_4, IDC_ON4, IDC_BITNAME_4,
	IDC_BITNO_5, IDC_ON5, IDC_BITNAME_5,
	IDC_BITNO_6, IDC_ON6, IDC_BITNAME_6
	};

//--------------------------------------------------------------------------------
void CTokenEditorDlg::PopulateCells()
	{
	CString sTemp;

	for(int i = 0; i < CBitList::NUM_OF_CELLS; i++)
		{
		int nCur = m_list.m_nTopLine + i;

		sTemp.Format("%d", nCur + 1);
		GetDlgItem(g_cells[i].m_nLabelId)->SetWindowText(sTemp);
		GetDlgItem(g_cells[i].m_nEditId)->SetWindowText(m_list.m_cells[nCur].m_sName);
		CButton* pBox = (CButton*) GetDlgItem(g_cells[i].m_nCheckId);
		pBox->SetCheck(m_list.m_cells[nCur].m_bIsSet ? 1 : 0);
		}
	}

//--------------------------------------------------------------------------------
BOOL CTokenEditorDlg::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
	{
	switch(nCode)
		{
		case BN_CLICKED:
			{
			for(int i = 0; i < CBitList::NUM_OF_CELLS; i++)
				if(nID == (UINT) g_cells[i].m_nCheckId)
					{
					ASSERT(m_list.m_nTopLine + i < CBitList::MAX);
					CButton* pBtn = (CButton*) GetDlgItem(nID);
					m_list.m_cells[m_list.m_nTopLine + i].m_bIsSet = pBtn->GetCheck() != 0;
					break;
					}
			}
			break;

		case EN_CHANGE:
			{
			for(int i = 0; i < CBitList::NUM_OF_CELLS; i++)
				if(nID == (UINT) g_cells[i].m_nEditId)
					{
					ASSERT(m_list.m_nTopLine + i < CBitList::MAX);
					GetDlgItem(nID)->GetWindowText(m_list.m_cells[m_list.m_nTopLine + i].m_sName);
					break;
					}
			}
			break;

		default:
			break;
		}

	return CDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
	}
