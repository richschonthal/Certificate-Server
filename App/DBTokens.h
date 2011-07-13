//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_DBTOKENS_H__F2792D07_D7F3_11D3_AF12_005004A1C5F3__INCLUDED_)
#define AFX_DBTOKENS_H__F2792D07_D7F3_11D3_AF12_005004A1C5F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
#include "AccessToken.h"
#include <ReadWriteObject.h>
#include <DBDatabase.h>

//--------------------------------------------------------------------------------
typedef CDBDatabase CDBCertificateMasters;

//--------------------------------------------------------------------------------
class CDBTokenInfo : public CReadWriteObject, public CObject
	{
	public:
		CDBTokenInfo();
		~CDBTokenInfo();

		DWORD m_nInUse;
		DWORD m_nCount;
		DWORD m_nMax;
		DWORD m_nParentId;
		CAccessToken m_token;
		CDBCertificateMasters* m_pCertificates;
	};

//--------------------------------------------------------------------------------
class CDBTokenInfoMap : public CReadWriteObject, public CMap<DWORD, DWORD, CDBTokenInfo*, CDBTokenInfo*>
	{
	public:
		CDBTokenInfoMap();
		virtual ~CDBTokenInfoMap();
	};

#endif // !defined(AFX_DBTOKENS_H__F2792D07_D7F3_11D3_AF12_005004A1C5F3__INCLUDED_)
