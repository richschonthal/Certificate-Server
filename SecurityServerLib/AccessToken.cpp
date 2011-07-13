//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "AccessToken.h"
#include <AllocString.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//--------------------------------------------------------------------------------
#define _TOKENVERSION 3

//--------------------------------------------------------------------------------
CAccessToken::CAccessToken()
		: m_nId(0)
		, m_pName(NULL)
		, m_nVersion(_TOKENVERSION)
		, m_nExtraLen(0)
		, m_pExtra(NULL)
	{
	memset(&m_nData, 0, sizeof(DWORD) * DWordCount);
	}

//--------------------------------------------------------------------------------
CAccessToken::CAccessToken(const DWORD nData[DWordCount])
		: m_nId(0)
		, m_pName(NULL)
		, m_nVersion(_TOKENVERSION)
		, m_nExtraLen(0)
		, m_pExtra(NULL)
	{
	memcpy(&m_nData, nData, DWordCount * sizeof(DWORD));
	}

//--------------------------------------------------------------------------------
CAccessToken::CAccessToken(const CAccessToken& rVal)
		: m_nId(0)
		, m_pName(NULL)
		, m_nVersion(_TOKENVERSION)
		, m_nExtraLen(0)
		, m_pExtra(NULL)
	{
	if(&rVal == this)
		return;
	*this = rVal;
	}

//--------------------------------------------------------------------------------
CAccessToken::~CAccessToken()
	{
	delete[] m_pName;
	delete[] m_pExtra;
	}

//--------------------------------------------------------------------------------
unsigned long CAccessToken::GetId() const
	{
	return m_nId;
	}

//--------------------------------------------------------------------------------
const char* CAccessToken::GetName() const
	{
	return m_pName;
	}

//--------------------------------------------------------------------------------
CAccessToken& CAccessToken::operator=(const CAccessToken& rVal)
	{
	if(&rVal != this)
		{
		delete[] m_pName;
		delete[] m_pExtra;
		if(rVal.m_pName != NULL)
			AllocString(m_pName, rVal.m_pName);
		else
			m_pName = NULL;
		if(rVal.m_pExtra != NULL)
			{
			m_nExtraLen = rVal.m_nExtraLen;
			memcpy(m_pExtra, rVal.m_pExtra, m_nExtraLen);
			}
		else
			{
			m_nExtraLen = 0;
			m_pExtra = NULL;
			}

		memcpy(&m_nData, rVal.m_nData, sizeof(DWORD) * DWordCount);

		m_nVersion = rVal.m_nVersion;
		m_nId = rVal.m_nId;
		}

	return *this;
	}

//--------------------------------------------------------------------------------
CAccessToken CAccessToken::operator&(const CAccessToken& rVal) const
	{
	if(m_nVersion != _TOKENVERSION)
		return NULL;

	CAccessToken lVal;

	for(int i = 0; i < DWordCount; i++)
		lVal.m_nData[i] &= rVal.m_nData[i];
	return lVal;
	}

//--------------------------------------------------------------------------------
CAccessToken CAccessToken::operator|(const CAccessToken& rVal) const
	{
	if(m_nVersion != _TOKENVERSION)
		return NULL;

	CAccessToken lVal;

	for(int i = 0; i < DWordCount; i++)
		lVal.m_nData[i] |= rVal.m_nData[i];
	return lVal;
	}

//--------------------------------------------------------------------------------
bool CAccessToken::operator==(const CAccessToken& rVal) const
	{
	if(m_nVersion != _TOKENVERSION)
		return false;

	for(int i = 0; i < DWordCount; i++)
		if(m_nData[i] != rVal.m_nData[i])
			return false;
	return true;
	}

//--------------------------------------------------------------------------------
bool CAccessToken::operator!=(const CAccessToken& rVal) const
	{
	if(m_nVersion != _TOKENVERSION)
		return false;

	for(int i = 0; i < DWordCount; i++)
		if(m_nData[i] != rVal.m_nData[i])
			return true;
	return false;
	}

// sets bit #nBitNo - 0 based count - ie 32bits are numbered 0 thru 31
//--------------------------------------------------------------------------------
void CAccessToken::SetBit(UINT nBitNo, bool bVal)
	{
	if(m_nVersion != _TOKENVERSION)
		return;

	const UINT nBits = sizeof(DWORD) * 8;
	UINT nIndex = nBitNo / nBits;
	if(nIndex >= DWordCount)
		return;
	DWORD nMask = 1;
	nMask = nMask << (nBitNo - (nIndex * nBits));
	if(bVal)
		m_nData[nIndex] |= nMask;
	else
		m_nData[nIndex] &= ~nMask;
	}

//--------------------------------------------------------------------------------
bool CAccessToken::IsBitSet(UINT nBitNo)
	{
	if(m_nVersion != _TOKENVERSION)
		return false;

	const UINT nBits = sizeof(DWORD) * 8;
	UINT nIndex = nBitNo / nBits;
	if(nIndex >= DWordCount)
		return false;
	DWORD nMask = 1;
	nMask = nMask << (nBitNo - (nIndex * nBits));
	return (m_nData[nIndex] & nMask) == nMask;
	}

//--------------------------------------------------------------------------------
void CAccessToken::SetExtraInfo(void* pExtra, int nLen)
	{
	delete[] m_pExtra;
	m_pExtra = new UCHAR[nLen];
	m_nExtraLen = nLen;
	memcpy(m_pExtra, pExtra, nLen);
	}

//--------------------------------------------------------------------------------
void CAccessToken::GetExtraInfo(void*& pExtra, int& nLen)
	{
	// do nothing if the buffer isnt big enough or
	// there's nothing to copy
	if((nLen > -1 && nLen >= m_nExtraLen) || m_pExtra == NULL)
		pExtra = NULL;
	else
		{
		if(pExtra == NULL)
			pExtra = new UCHAR[m_nExtraLen];
		memcpy(pExtra, m_pExtra, m_nExtraLen);
		}

	// if the buffer specified isnt big enough
	// the caller will need to know how big to make it
	nLen = m_nExtraLen;
	}

//--------------------------------------------------------------------------------
int CAccessToken::Encode(char* pData)
	{
	if(m_nVersion != _TOKENVERSION)
		return 0;

	int nLen = sprintf(pData, " %lx %lx", m_nVersion, m_nId);

	for(int i = 0; i < DWordCount; i++)
		nLen += sprintf(pData+nLen, " %lx", m_nData[i]);
	nLen += sprintf(pData+nLen, " %x", m_nExtraLen);
	if(m_nExtraLen > 0)
		for(i = 0; i < m_nExtraLen; i++)
			nLen += sprintf(pData+nLen, " %lx", (int) m_pExtra[i]);
	return nLen;
	}

//--------------------------------------------------------------------------------
int CAccessToken::Decode(const char* pData, int nLen)
	{
	if(pData == NULL)
		return 0;

	if(*pData == 0 || *pData != ' ')
		return 0;

	// skip the leading space
	pData++;

	char temp[32] = {0};
	char* pBase = temp;
	int nCur = 0;
	const int nItemCount = 7;
	// one element for each member var that was encoded
	unsigned long nData[nItemCount] = {0,0,0,0,0,0,0};

	for(int i = 0;i < nLen;i++)
		{
		char c = *pData++;
		if(isxdigit(c))
			{
			*pBase = c;
			pBase++;
			}
		else
			{
			*pBase = 0;
			nData[nCur] = strtoul(temp, NULL, 16);
			pBase = temp;
			nCur++;
			if(nCur == nItemCount)
				break;
			}
		}

	// keep track of the # of bytes decoded
	nLen = i + 1;

	m_nVersion = nData[0];
	if(m_nVersion != _TOKENVERSION)
		return 0;
	m_nId = nData[1];
	for(i = 0; i < 4; i++)
		m_nData[i] = nData[i+2];
	m_nExtraLen = nData[6];

	if(m_nExtraLen > 0)
		{
		pBase = temp;
		delete[] m_pExtra;
		m_pExtra = new unsigned char[m_nExtraLen];
		for(i = 0; i < m_nExtraLen;)
			{
			char c = *pData;
			pData++;
			if(isxdigit(c))
				{
				*pBase = c;
				pBase++;
				}
			else
				{
				*pBase = 0;
				m_pExtra[i++] = (unsigned char)strtoul(temp, NULL, 16);
				pBase = temp;
				}
			}
		}

	return nLen + m_nExtraLen;
	}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
CAccessTokenMap::~CAccessTokenMap()
	{
	POSITION pos = GetStartPosition();
	while(pos != NULL)
		{
		ULONG nKey;
		CAccessToken* pTok;
		GetNextAssoc(pos, nKey, pTok);
		delete pTok;
		}
	}

//--------------------------------------------------------------------------------
CAccessTokenMap::operator=(const CAccessTokenMap& rVal)
	{
	POSITION pos = rVal.GetStartPosition();
	while(pos != NULL)
		{
		ULONG nKey;
		CAccessToken* pTok;
		rVal.GetNextAssoc(pos, nKey, pTok);
		if(pTok == NULL)
			break;
		CAccessToken* pNewTok;
		if(Lookup(nKey, pNewTok))
			continue;
		pNewTok = new CAccessToken(*pTok);
		SetAt(nKey, pNewTok);
		}
	}

//--------------------------------------------------------------------------------
bool CAccessTokenMap::operator==(const CAccessTokenMap& rVal)
	{
	int nCount = GetCount();

	if(nCount == 0 && rVal.GetCount() == 0)
		return true;

	if(nCount != rVal.GetCount())
		return false;

	POSITION posR = rVal.GetStartPosition();
	POSITION posL = GetStartPosition();
	while(posR != NULL && posL != NULL)
		{
		ULONG nKeyR;
		CAccessToken* pTokR;
		rVal.GetNextAssoc(posR, nKeyR, pTokR);
		ULONG nKeyL;
		CAccessToken* pTokL;
		GetNextAssoc(posL, nKeyL, pTokL);
		if(pTokR == NULL)
			{
			if(pTokL == NULL)
				continue;
			}

		if(pTokL == NULL)
			return false;

		if(! (*pTokL == *pTokR))
			return false;
		}

	return true;
	}
