//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _BITLIST_H_
#define _BITLIST_H_

//--------------------------------------------------------------------------------
class CBitCell
	{
	public:
		bool m_bIsSet;
		CString m_sName;
	};

//--------------------------------------------------------------------------------
class CBitList
	{
	public:
		enum
			{
			MIN = 1,
			MAX = 32,
			NUM_OF_CELLS = 6,
			MAXPOS = MAX - NUM_OF_CELLS
			};

	public:
		CBitCell m_cells[MAX];
		int m_nTopLine;

	public:
		CBitList();

		void ConfigureScrollBar(CScrollBar*);
		void SetScrollBarPos(CScrollBar*, int, bool = true);

		void DoLineDown(int, CScrollBar*);
		void DoLineUp(int, CScrollBar*);
		void DoPageDown(int, CScrollBar*);
		void DoPageUp(int, CScrollBar*);
		void DoThumbTrack(int, CScrollBar*);
		void DoThumbPos(int, CScrollBar*);
	};

#endif //_BITLIST_H_