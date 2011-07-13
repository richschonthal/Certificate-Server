//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"

#include "Certificate.h"
#include "AccessToken.h"
#include "MCCrypt.h"

//--------------------------------------------------------------------------------
using namespace CCertificateCommands;

//--------------------------------------------------------------------------------
CCertificate::CCertificate()
		: m_nCmd(0)
		, m_nMagicNumber(0)
		, m_pTokenMap(new CAccessTokenMap)
		, m_nSecondsToLive(0)
		, m_tmIssued(0)
		, m_nId(0)
		, m_bLocalTokenAlloc(true)
		, m_nUserClass(0)

	{
// is it just me or is it totally rediculous that i have to do this this way?
// the microsoft c++ compiler doesnt allow 'sizeof' in the if statements
const int x = sizeof(CCertificate*);
const int y = sizeof(ULONG);
#if (x != y)
#error "pointers and longs are assumed to be 32 bits"
#endif
	}

//--------------------------------------------------------------------------------
CCertificate::CCertificate(ULONG nCmd, ULONG nUserClass, ULONG nCmdParam)
		: m_nCmd(0)
		, m_nMagicNumber(0)
		, m_pTokenMap(new CAccessTokenMap)
		, m_nSecondsToLive(0)
		, m_tmIssued(0)
		, m_nId(0)
		, m_bLocalTokenAlloc(true)
		, m_nUserClass(nUserClass)
	{
// is it just me or is it totally rediculous that i have to do this this way?
// the microsoft c++ compiler doesnt allow 'sizeof' in the if statements
const int x = sizeof(CCertificate*);
const int y = sizeof(ULONG);
#if (x != y)
#error "pointers and longs are assumed to be 32 bits"
#endif
	SetCommand(nCmd);
	SetCommandParam(nCmdParam);
	}

//--------------------------------------------------------------------------------
CCertificate::CCertificate(const CCertificate& rVal)
		: m_pTokenMap(new CAccessTokenMap)
	{
	*this = rVal;
	}

//--------------------------------------------------------------------------------
CCertificate::~CCertificate()
	{
	if(! m_bLocalTokenAlloc)
		// dont let the tokens get deleted
		m_pTokenMap->RemoveAll();

	delete m_pTokenMap;
	}

//--------------------------------------------------------------------------------
CCertificate::operator=(const CCertificate& rVal)
	{
	m_nCmd = rVal.m_nCmd;
	m_nMagicNumber = rVal.m_nMagicNumber;
	*m_pTokenMap = *rVal.m_pTokenMap;
	m_nSecondsToLive = rVal.m_nSecondsToLive;
	m_tmIssued = rVal.m_tmIssued;
	m_nId = rVal.m_nId;
	m_bLocalTokenAlloc = rVal.m_bLocalTokenAlloc;
	m_nUserClass = rVal.m_nUserClass;
	}

//--------------------------------------------------------------------------------
bool CCertificate::operator==(const CCertificate& rVal)
	{
	if(m_nCmd == rVal.m_nCmd && m_nMagicNumber == rVal.m_nMagicNumber && m_nSecondsToLive == rVal.m_nSecondsToLive &&
		m_tmIssued == rVal.m_tmIssued && m_bLocalTokenAlloc == rVal.m_bLocalTokenAlloc && m_nUserClass == rVal.m_nUserClass)
		{
		if(m_pTokenMap == NULL)
			{
			if(rVal.m_pTokenMap == NULL)
				return true;

			return rVal.m_pTokenMap->GetCount() == 0;
			}

		if(rVal.m_pTokenMap == NULL)
			return m_pTokenMap->GetCount() == 0;

		return *m_pTokenMap == *rVal.m_pTokenMap;
		}

	return false;
	}

//--------------------------------------------------------------------------------
CAccessTokenMap* CCertificate::GetTokenMap() const
	{
	return m_pTokenMap;
	}

//--------------------------------------------------------------------------------
CAccessToken* CCertificate::GetToken(ULONG nId)
	{
	CAccessToken* pTok;

	// get the first token in the map
	if(nId == (ULONG)-1)
		{
		POSITION pos = m_pTokenMap->GetStartPosition();
		if(pos == NULL)
			return NULL;
		m_pTokenMap->GetNextAssoc(pos, nId, pTok);
		return pTok;
		}

	if(m_pTokenMap->Lookup(nId, pTok))
		return pTok;

	return NULL;
	}

//--------------------------------------------------------------------------------
bool CCertificate::HasToken(ULONG nId)
	{
	if(nId == (ULONG) -1)
		return false;
	return GetToken(nId) != NULL;
	}

//--------------------------------------------------------------------------------
bool CCertificate::IsExpired()
	{
	return (time(NULL) - m_tmIssued) > (time_t) m_nSecondsToLive;
	}

//--------------------------------------------------------------------------------
int CCertificate::Encode(char* pDest, bool bIncludeToken)
	{
	const int nCount = m_pTokenMap->GetCount();
	int nLen = sprintf(pDest, "%lx %lx %lx %lx %lx %lx %lx ", 
		m_nCmd,
		m_nMagicNumber,
		m_nId,
		m_nUserClass,
		m_nSecondsToLive,
		m_tmIssued,
		bIncludeToken ? nCount : 0);

	if(bIncludeToken && nCount)
		{
		POSITION pos = m_pTokenMap->GetStartPosition();
		while(pos)
			{
			ULONG nKey;
			CAccessToken* pTok;
			m_pTokenMap->GetNextAssoc(pos, nKey, pTok);
			nLen += pTok->Encode(pDest + nLen);
			}
		}

	CMCEncrypt encrypt((const unsigned char*) pDest, nLen);
	memcpy(pDest, encrypt.GetData(), encrypt.GetLength());
	return nLen;
	}

//--------------------------------------------------------------------------------
void CCertificate::Decode(const char* pCypherSrc, int nLen)
	{
	CMCDecrypt decrypt((const unsigned char*) pCypherSrc, nLen);

	const char* pSrc = (const char*) decrypt.GetData();

	if(pSrc == NULL)
		return;

	char temp[32] = {0};
	char* pBase = temp;
	int nCur = 0;
	const int nItemCount = 7;
	// one element for each member var that was encoded
	ULONG nData[nItemCount] = {0,0,0,0,0,0,0};

	for(int i = 0;i < nLen;i++)
		{
		char c = *pSrc++;
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

	m_nCmd = nData[0];
	m_nMagicNumber = nData[1];
	m_nId = nData[2];
	m_nUserClass = nData[3];
	m_nSecondsToLive = nData[4];
	m_tmIssued = nData[5];
	int nCount = nData[6];

	nLen -= i;

	if(nCount > 0)
		{
		m_bLocalTokenAlloc = true;
		int nOffset = 0;
		for(int i = 0; i < nCount; i++)
			{
			CAccessToken* pToken = new CAccessToken;
			// keep track of the offset becuase there may be multiple tokens encoded
			nOffset += pToken->Decode(pSrc + nOffset, nLen - nOffset);
			CAccessToken* pCur = NULL;
			if(m_pTokenMap->Lookup(pToken->GetId(), pCur))
				{
				*pCur = *pToken;
				delete pToken;
				}
			else
				m_pTokenMap->SetAt(pToken->GetId(), pToken);
			}
		}
	}

//--------------------------------------------------------------------------------
UINT CCertificate::GetCommand() const
	{
	return m_nCmd & MASK_CMD;
	}

//--------------------------------------------------------------------------------
void CCertificate::SetCommand(UINT nCmd)
	{
	m_nCmd &= MASK_PARAM;
	m_nCmd += (nCmd & MASK_CMD);
	}

//--------------------------------------------------------------------------------
void CCertificate::SetCommand(UINT nCmd, UINT nParam)
	{
	m_nCmd = (nCmd & MASK_CMD) + ((nParam<<16) & MASK_PARAM);
	}

//--------------------------------------------------------------------------------
UINT CCertificate::GetCommandParam() const
	{
	return (m_nCmd>>16) & MASK_CMD;
	}

//--------------------------------------------------------------------------------
void CCertificate::SetCommandParam(UINT nParam)
	{
	m_nCmd &= MASK_CMD;
	m_nCmd += (nParam<<16) & MASK_PARAM;
	}

//--------------------------------------------------------------------------------
ULONG CCertificate::GetMagicNumber() const
	{
	return m_nMagicNumber;
	}

//--------------------------------------------------------------------------------
ULONG CCertificate::GetUserClass() const
	{
	return m_nUserClass;
	}

//--------------------------------------------------------------------------------
ULONG CCertificate::GetId() const
	{
	return m_nId;
	}

//--------------------------------------------------------------------------------
time_t CCertificate::GetTimeIssued() const
	{
	return m_tmIssued;
	}

//--------------------------------------------------------------------------------
time_t CCertificate::GetSecondsToLive() const
	{
	return m_nSecondsToLive;
	}

//--------------------------------------------------------------------------------
bool CCertificate::OnExpired()
	{
	return false;
	}

//--------------------------------------------------------------------------------
bool CCertificate::OnRevoked()
	{
	return false;
	}
