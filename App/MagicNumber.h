//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _MAGICNUMBER_H_
#define _MAGICNUMBER_H_

//--------------------------------------------------------------------------------
#include <ReadWriteObject.h>

//--------------------------------------------------------------------------------
class CMagicNumber : public CReadWriteObject
	{
	private:
		ULONG m_nPrev;
		ULONG m_nCurrent;
		ULONG m_nNext;

		CTime m_ctLastAdvance;

	public:
		const int m_nInterval;	// # of seconds between advances

		CMagicNumber();
		static ULONG Generate();
		ULONG GetNext();
		ULONG GetCurrent();
		ULONG GetPrevious();

		bool IsValid(ULONG);
	};

#endif // _MAGICNUMBER_H_
