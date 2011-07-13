//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#include "stdafx.h"

#include "MCEncrypt.h"
#include "EncryptTables.hpp"

//--------------------------------------------------------------------------------
static const BYTE g_key[] = { 0x73, 0x0E, 0x3B, 0xA1, 0xB9, 0xD3, 0x1C, 0x68, 0x00};

//--------------------------------------------------------------------------------
CMCEncrypt::CMCEncrypt(const BYTE* pData, int nLen, const BYTE* pKey)
		: m_pKey(NULL)
		, m_nLen(0)
		, m_pData(NULL)
	{
	Encrypt(pData, nLen, pKey);
	}

//--------------------------------------------------------------------------------
CMCEncrypt::~CMCEncrypt()
	{
	delete[] m_pData;
	delete[] m_pKey;
	}

//--------------------------------------------------------------------------------
bool CMCEncrypt::Encrypt(const BYTE* pData, int nLen, const BYTE* pKey)
	{
	delete[] m_pData;
	m_pData = NULL;
	m_nLen = 0;
	delete[] m_pKey;
	m_pKey = NULL;

	// must be at least 4 bytes long to encrypt
	if(nLen < 4)
		return false;

	if(pData == NULL)
		return false;

	const BYTE* pSrcKey = (pKey == NULL) ? g_key : pKey;

	// keys are null terminated so they look like strings
	m_pKey = new BYTE[strlen((LPCTSTR) pSrcKey) + 1];

	strcpy((LPTSTR) m_pKey, (LPCTSTR) pSrcKey);

	// copy the length
	m_nLen = nLen;
	// allocate buffer
	m_pData = new BYTE[m_nLen];
	// always encrypt the 1st byte with 7F
	m_pData[0] = pData[0] ^ 0x7F;
	// get the "shift" table using the 1st byte to determine which
	// table to use
	const BYTE* pTable = g_encryptTblTbl[(int) m_pData[0]];

	int nCurKey = 0;
	for(int i = 1; i < m_nLen; i++)
		{
		//encrypt with key and shift it
		m_pData[i] = pTable[(int) (pData[i] ^ m_pKey[nCurKey++])];
		// wrap the key index when null is reached
		if(m_pKey[nCurKey] == 0)
			nCurKey = 0;
		}

	return true;
	}

