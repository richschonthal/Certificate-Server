//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "LicenseFile.h"
#include "DBTokens.h"
#include "MCEncrypt.h"
#include <allocstring.h>

#define SIGNATURE "MARKCAREMEDICAL"
#define VERSION_FIRST_WITH_PARENT_ID 2

//--------------------------------------------------------------------------------
CLicenseFile::CTokenInfoList::~CTokenInfoList()
	{
	for(int i = 0; i < GetSize(); i++)
		delete GetAt(i);
	}

//--------------------------------------------------------------------------------
CLicenseFile::CLicenseFile()
	{
	m_sP3Serial[0] = 0;
	m_sDongle[0] = 0;
	m_sEmailIP[0] = 0;
	m_sEmailFrom[0] = 0;
	m_sEmailTo[0] = 0;
	m_pBuffer = NULL;
	m_pExtraData = NULL;
	m_nExtraDataLen = 0;
	m_sMonitorUser[0] = 0;
	m_sMonitorPass[0] = 0;
	}

//--------------------------------------------------------------------------------
CLicenseFile::~CLicenseFile()
	{
	delete m_pBuffer;
	}

//--------------------------------------------------------------------------------
bool CLicenseFile::Read()
	{
	int nCurIndex = 0;
	int nSize = 0;
	DWORD nVersion = 0;

	if(m_pBuffer == NULL)
		{
		m_sP3Serial[0] = 0;
		m_sDongle[0] = 0;
		m_sEmailIP[0] = 0;
		m_sEmailFrom[0] = 0;
		m_sEmailTo[0] = 0;
		m_sMonitorUser[0] = 0;
		m_sMonitorPass[0] = 0;

		BYTE* pTemp = new BYTE[4096000];
		ASSERT(pTemp != NULL);
		const int nMinLen = sizeof(m_sP3Serial) + sizeof(m_sDongle) + sizeof(CAccessToken) + 
			sizeof(m_sEmailIP) + sizeof(m_sEmailFrom) + sizeof(m_sEmailTo) + sizeof(long) + 16;

		nSize = CFile::Read(pTemp, 4096000);

		if(nSize < nMinLen)
			{
			delete[] pTemp;
			return false;
			}

		CMCDecrypt crypt(pTemp, nSize);
		m_pBuffer = new char[nSize];
		memcpy(m_pBuffer, crypt.GetData(), nSize);

		nCurIndex = 16;
		if(strcmp((LPCTSTR) m_pBuffer, "MARKCAREMEDICAL") != 0)
			{
			delete[] pTemp;
			return false;
			}

		memcpy(&nVersion, &m_pBuffer[nCurIndex], sizeof(nVersion));
		nCurIndex += sizeof(nVersion);

		memcpy(m_sP3Serial, &m_pBuffer[nCurIndex], sizeof(m_sP3Serial));
		nCurIndex += sizeof(m_sP3Serial);

		memcpy(m_sDongle, &m_pBuffer[nCurIndex], sizeof(m_sDongle));
		nCurIndex += sizeof(m_sDongle);

		time_t nTime = 0;
		memcpy(&nTime, &m_pBuffer[nCurIndex], sizeof(time_t));
		nCurIndex += sizeof(time_t);
		m_ctExpiration = nTime;

		// read the length of the email address
		DWORD nLen = 0;
		memcpy(&nLen, &m_pBuffer[nCurIndex], sizeof(nLen));
		nCurIndex += sizeof(nLen);

		CString sTemp;
		memcpy(sTemp.GetBuffer(nLen), &m_pBuffer[nCurIndex], nLen);
		sTemp.ReleaseBuffer();
		ParseEmailField(sTemp, m_sEmailIP, m_sEmailFrom, m_sEmailTo);
		nCurIndex += nLen;

		// is backup is stored as 1 or 0
		m_bIsBackup = m_pBuffer[nCurIndex] != 0;
		nCurIndex += 1;

		memcpy(&m_nMinutesToWaitForDongle, &m_pBuffer[nCurIndex], sizeof(m_nMinutesToWaitForDongle));
		nCurIndex += sizeof(m_nMinutesToWaitForDongle);

		// read the length of the monitor uname
		nLen = 0;
		memcpy(&nLen, &m_pBuffer[nCurIndex], sizeof(nLen));
		nCurIndex += sizeof(nLen);

		memcpy(m_sMonitorUser, &m_pBuffer[nCurIndex], nLen);
		m_sMonitorUser[nLen] = 0;
		nCurIndex += nLen;

		// read the length of the monitor pwd
		nLen = 0;
		memcpy(&nLen, &m_pBuffer[nCurIndex], sizeof(nLen));
		nCurIndex += sizeof(nLen);

		memcpy(m_sMonitorPass, &m_pBuffer[nCurIndex], nLen);
		m_sMonitorPass[nLen] = 0;
		nCurIndex += nLen;

		// read the "extra" data - this version doesnt support any
		// so if there is some, just skip over it
		memcpy(&nLen, &m_pBuffer[nCurIndex], sizeof(nLen));
		nCurIndex += sizeof(nLen) + nLen;

		delete[] pTemp;
		}

	// we've reached the end of the data
	if(nCurIndex >= nSize - 1)
		return false;

	for(;;)
		{
		CDBTokenInfo* pCurRec = new CDBTokenInfo;

		// read the token id
		memcpy(&pCurRec->m_token.m_nId, &m_pBuffer[nCurIndex], sizeof(pCurRec->m_token.m_nId));
		nCurIndex += sizeof(pCurRec->m_token.m_nId);

		// read the name length
		UINT nLen;
		memcpy(&nLen, &m_pBuffer[nCurIndex], sizeof(nLen));
		nCurIndex += sizeof(nLen);

		ASSERT(nLen < 1024);

		// nLen > 0 then there is a name string so read it
		if(nLen > 0)
			{
			pCurRec->m_token.m_pName = new char[nLen + 1];
			memcpy(pCurRec->m_token.m_pName, &m_pBuffer[nCurIndex], nLen);
			pCurRec->m_token.m_pName[nLen] = 0;
			nCurIndex += nLen;
			}
		else
			{
			pCurRec->m_token.m_pName = new char[1];
			*(pCurRec->m_token.m_pName) = 0;
			}

		// read the length of the encoded token
		memcpy(&nLen, &m_pBuffer[nCurIndex], sizeof(nLen));
		nCurIndex += sizeof(nLen);

		pCurRec->m_token.Decode((LPCTSTR) &m_pBuffer[nCurIndex], nLen);
		nCurIndex += nLen;

		// read the count
		memcpy(&pCurRec->m_nCount, &m_pBuffer[nCurIndex], sizeof(pCurRec->m_nCount));
		nCurIndex += sizeof(pCurRec->m_nCount);

		// read the max count
		memcpy(&pCurRec->m_nMax, &m_pBuffer[nCurIndex], sizeof(pCurRec->m_nMax));
		nCurIndex += sizeof(pCurRec->m_nMax);

		if(nVersion >= VERSION_FIRST_WITH_PARENT_ID)
			{
			// read the parent token id
			memcpy(&pCurRec->m_nParentId, &m_pBuffer[nCurIndex], sizeof(pCurRec->m_nParentId));
			nCurIndex += sizeof(pCurRec->m_nParentId);
			}

		m_tokens.Add(pCurRec);

		if(nCurIndex >= nSize - 1)
			break;
		}

	return true;
	}

//--------------------------------------------------------------------------------
void CLicenseFile::ParseEmailField(const CString& sIn, char* pIP, char* pFrom, char* pTo)
	{
	CString sTemp;
	TRY
		{
		// parse format: ip.ip.ip.ip, from text, to address (minus @ and domain)
		int nIndex = sIn.Find(',');
		if(nIndex == -1)
			return;
		sTemp = sIn.Left(nIndex);
		sTemp.Remove(' ');
		strcpy(pIP, sTemp);
		int nIndex2 = sIn.Find(',', nIndex + 1);
		if(nIndex2 == -1)
			return;
		sTemp = sIn.Mid(nIndex + 1, nIndex2 - nIndex - 1);
		sTemp.TrimLeft();
		strcpy(pFrom, sTemp);
		sTemp = sIn.Mid(nIndex2 + 1);
		sTemp.Remove(' ');
		strcpy(pTo, sTemp);
		}
	CATCH_ALL(e)
		{
		}
	END_CATCH_ALL
	}
