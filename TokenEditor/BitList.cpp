//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "BitList.h"

//--------------------------------------------------------------------------------
CBitList::CBitList()
	{
	for(int i = 0; i < MAX; i++)
		m_cells[i].m_bIsSet = false;
	}

//--------------------------------------------------------------------------------
void CBitList::ConfigureScrollBar(CScrollBar* pBar)
	{
	pBar->SetScrollRange(0, MAXPOS);
	pBar->SetScrollPos(0);
	m_nTopLine = 0;
	}

//--------------------------------------------------------------------------------
void CBitList::SetScrollBarPos(CScrollBar* pScrollBar, int nPos, bool bSetRelative)
	{
	if(bSetRelative)
		nPos += pScrollBar->GetScrollPos();

	if(nPos < 0)
		nPos = 0;
	else
		if(nPos > MAXPOS)
			nPos = MAXPOS;

	pScrollBar->SetScrollPos(nPos);
	m_nTopLine = nPos;
	}

//--------------------------------------------------------------------------------
void CBitList::DoLineDown(int nPos, CScrollBar* pScrollBar)
	{
	SetScrollBarPos(pScrollBar, 1);
	}

//--------------------------------------------------------------------------------
void CBitList::DoLineUp(int nPos, CScrollBar* pScrollBar)
	{
	SetScrollBarPos(pScrollBar, -1);
	}

//--------------------------------------------------------------------------------
void CBitList::DoPageDown(int nPos, CScrollBar* pScrollBar)
	{
	SetScrollBarPos(pScrollBar, NUM_OF_CELLS);
	}

//--------------------------------------------------------------------------------
void CBitList::DoPageUp(int nPos, CScrollBar* pScrollBar)
	{
	SetScrollBarPos(pScrollBar, -NUM_OF_CELLS);
	}

//--------------------------------------------------------------------------------
void CBitList::DoThumbTrack(int nPos, CScrollBar* pScrollBar)
	{
	SetScrollBarPos(pScrollBar, nPos, false);
	}

//--------------------------------------------------------------------------------
void CBitList::DoThumbPos(int nPos, CScrollBar* pScrollBar)
	{
	SetScrollBarPos(pScrollBar, nPos, false);
	}
