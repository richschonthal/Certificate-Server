//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "SecurityServer.h"
#include "DBTokens.h"
#include "SecServTokenId.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//--------------------------------------------------------------------------------
CDBTokenInfoMap::CDBTokenInfoMap()
	{
	}

//--------------------------------------------------------------------------------
CDBTokenInfoMap::~CDBTokenInfoMap()
	{

	}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
CDBTokenInfo::CDBTokenInfo()
		: m_nInUse(0)
		, m_nCount(0)
		, m_nMax(0)
		, m_nParentId(0)
	{
	m_pCertificates = new CDBCertificateMasters;
	}

//--------------------------------------------------------------------------------
CDBTokenInfo::~CDBTokenInfo()
	{
	delete m_pCertificates;
	}

