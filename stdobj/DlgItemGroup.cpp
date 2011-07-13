//--------------------------------------------------------------------------------
//
// Copyright (c) 2001 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#include "stdafx.h"
#include "DlgItemGroup.h"

//--------------------------------------------------------------------------------
class CControlInfo
	{
	public:
		CPoint m_ptRelative;
		int m_nControlId;
		UCHAR m_nFlags;
	public:
		CControlInfo(int nId, int x, int y, UCHAR nFlags) : m_nControlId(nId) , m_ptRelative(x,y), m_nFlags(nFlags) {}
	};

//--------------------------------------------------------------------------------
class CControlList : public CList<CControlInfo*, CControlInfo*>
	{
	public:
		~CControlList()
			{
			for(POSITION pos = GetTailPosition(); pos != NULL; GetPrev(pos))
				delete GetAt(pos);
			}
	};

//--------------------------------------------------------------------------------
CDlgItemGroup::CDlgItemGroup(HWND& hWnd, int nAnchor)
		: m_hWnd(hWnd)
		, m_nAnchorCtrlId(nAnchor)
		, m_pControls(new CControlList)
	{
	}

//--------------------------------------------------------------------------------
CDlgItemGroup::~CDlgItemGroup()
	{
	delete m_pControls;
	}

//--------------------------------------------------------------------------------
HWND CDlgItemGroup::GetDlgItemRect(HWND hDlg, int nId, CRect& rect)
	{
	HWND hWnd = ::GetDlgItem(hDlg, nId);
	if(hWnd == NULL)
		return NULL;
	::GetWindowRect(hWnd, &rect);	
	::ScreenToClient(hDlg, (POINT*)&(rect.TopLeft()));
	::ScreenToClient(hDlg, (POINT*)&(rect.BottomRight()));
	return hWnd;
	}

//--------------------------------------------------------------------------------
HWND CDlgItemGroup::GetDlgItemRect(int nId, CRect& rect) const
	{
	HWND hWnd = ::GetDlgItem(m_hWnd, nId);
	if(hWnd == NULL)
		return NULL;
	::GetWindowRect(hWnd, &rect);	
	::ScreenToClient(m_hWnd, (POINT*)&(rect.TopLeft()));
	::ScreenToClient(m_hWnd, (POINT*)&(rect.BottomRight()));
	return hWnd;
	}

//--------------------------------------------------------------------------------
void CDlgItemGroup::UpdatePositions() const
	{
	long nBase = ::GetDialogBaseUnits();
	int nBaseX = LOWORD(nBase);
	int nBaseY = HIWORD(nBase);
	CRect rect;
	CRect rctAnchor;

	if(m_nAnchorCtrlId != 0)
		GetDlgItemRect(m_nAnchorCtrlId, rctAnchor);
	else
		::GetClientRect(m_hWnd, &rctAnchor);

	for(POSITION pos = m_pControls->GetHeadPosition(); pos != NULL; m_pControls->GetNext(pos))
		{
		CControlInfo* pInfo = m_pControls->GetAt(pos);
		HWND hwnd = GetDlgItemRect(pInfo->m_nControlId, rect);
		if(hwnd == NULL)
			continue;
		int nTop;
		
		if(pInfo->m_nFlags & CDlgItemGroup::RelYToBottom)
			nTop = rctAnchor.bottom + ::MulDiv(pInfo->m_ptRelative.y, nBaseY, 8);
		else
			if(pInfo->m_nFlags & CDlgItemGroup::RelYToTop)
				nTop = rctAnchor.top + ::MulDiv(pInfo->m_ptRelative.y, nBaseY, 8);
			else
				nTop = rect.top;

		int nLeft;
		
		if(pInfo->m_nFlags & CDlgItemGroup::RelXToRight)
			nLeft = rctAnchor.right + ::MulDiv(pInfo->m_ptRelative.x, nBaseX, 4);
		else
			if(pInfo->m_nFlags & CDlgItemGroup::RelXToLeft)
				nLeft = rctAnchor.left + ::MulDiv(pInfo->m_ptRelative.x, nBaseX, 4);
			else
				nLeft = rect.left;

		if(pInfo->m_nFlags & CDlgItemGroup::RelSameWidth)
			rect.right = rctAnchor.right;

		::MoveWindow(hwnd, nLeft, nTop, rect.Width(), rect.Height(), FALSE);
		}
	}

//--------------------------------------------------------------------------------
void CDlgItemGroup::AddItem(int nId, int x, int y, int nFlags)
	{
	m_pControls->AddTail(new CControlInfo(nId, x, y, nFlags));
	}

//--------------------------------------------------------------------------------
void CDlgItemGroup::RemoveItem(int nId)
	{
	for(POSITION pos = m_pControls->GetHeadPosition(); pos != NULL; m_pControls->GetNext(pos))
		if(m_pControls->GetAt(pos)->m_nControlId == nId)
			{
			m_pControls->RemoveAt(pos);
			return;
			}
	}
