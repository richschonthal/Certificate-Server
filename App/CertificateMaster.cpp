//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "CertificateMaster.h"
#include <Certificate.h>

#include "DBTokens.h"

#include <ReadLock.h>
#include <WriteLock.h>
#include <AllocString.h>
#include <atomic.h>

//--------------------------------------------------------------------------------
CCertificateMaster::CCertificateMaster()
		: m_pIP(NULL)
		, m_pTokenInfoMap(new CDBTokenInfoMap)
		, m_ctLastRefresh(0)
	{

	}

//--------------------------------------------------------------------------------
CCertificateMaster::~CCertificateMaster()
	{
	delete m_pTokenInfoMap;
	delete[] m_pIP;
	}

//--------------------------------------------------------------------------------
LPCTSTR CCertificateMaster::GetIP() const
	{
	return m_pIP;
	}

//--------------------------------------------------------------------------------
void CCertificateMaster::SetIP(LPCTSTR pIP)
	{
	CWriteLock lock(this, false);
	if(! lock.Lock(30000))
		return;

	delete[] m_pIP;
	m_pIP = NULL;
	AllocString(m_pIP, pIP);
	}

//--------------------------------------------------------------------------------
CTime CCertificateMaster::GetLastRefresh()
	{
	return m_ctLastRefresh;
	}

//--------------------------------------------------------------------------------
void CCertificateMaster::SetLastRefresh(CTime ctTime)
	{
	Atomic::Exchange(reinterpret_cast<long*>(&m_ctLastRefresh), ctTime.GetTime());
	}

//--------------------------------------------------------------------------------
CDBTokenInfoMap* CCertificateMaster::GetTokenInfoMap()
	{
	return m_pTokenInfoMap;
	}
