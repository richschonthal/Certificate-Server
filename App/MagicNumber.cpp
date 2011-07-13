//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "MagicNumber.h"

//--------------------------------------------------------------------------------
CMagicNumber::CMagicNumber()
		: m_nInterval(120) // # of seconds to wait before advancing
	{
	srand(CTime::GetCurrentTime().GetTime());
	m_nNext = Generate();
	m_nCurrent = Generate();
	m_nPrev = Generate();
	m_ctLastAdvance = CTime::GetCurrentTime();
	}

//--------------------------------------------------------------------------------
ULONG CMagicNumber::Generate()
	{
	// assume a ULONG is 32bits
	ULONG nTime = CTime::GetCurrentTime().GetTime();
	ULONG nRand = rand();
	// assumes max val return by rand is 7fff
	nTime = (nTime<<16) & 0xFFFF0000;
	return nTime + nRand;
	}

//--------------------------------------------------------------------------------
ULONG CMagicNumber::GetNext()
	{
	CWriteLock lock(this, false);
	if(! lock.Lock(30000))
		return 0;

	CTimeSpan span = m_ctLastAdvance - CTime::GetCurrentTime();
	if(span.GetTotalSeconds() > m_nInterval)
		{
		m_nPrev = m_nCurrent;
		m_nCurrent = m_nNext;
		m_nNext = Generate();
		m_ctLastAdvance = CTime::GetCurrentTime();
		}

	return m_nNext;
	}

//--------------------------------------------------------------------------------
ULONG CMagicNumber::GetCurrent()
	{
	CReadLock lock(this);
	return m_nCurrent;
	}

//--------------------------------------------------------------------------------
ULONG CMagicNumber::GetPrevious()
	{
	CReadLock lock(this);
	return m_nPrev;
	}

//--------------------------------------------------------------------------------
bool CMagicNumber::IsValid(ULONG nWhat)
	{
	CReadLock lock(this);

	return nWhat == m_nPrev || nWhat == m_nCurrent || nWhat == m_nNext;
	}
