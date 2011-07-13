//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "SecurityServer.h"
#include "DBSubSystem.h"
#include "CertificateMaster.h"
#include "DBTokens.h"
#include "DBCleanUpThread.h"
#include "MagicNumber.h"
#include "LicenseFile.h"
#include "SSConfigGeneral.h"
#include "SSConfigPerformance.h"
#include "SSConfigBackup.h"
#include "SSQuickMail.h"
#include "Dongle.h"
#include "SecServTokenId.h"

#include <PlatformInfo.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//--------------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CDBSubSystem, CSubSystem);

//--------------------------------------------------------------------------------
// keep track of the last time status emails were sent so that we don't bombard the mail
// server - ie only send 1 status 4 email every 10 minutes
// 2 elements - 1 for for status 4 and 1 for license count exceeded
CTime CDBSubSystem::m_ctLastSent[2] = {0,0};

//--------------------------------------------------------------------------------
bool CDBSubSystem::CanSendEmailNow(UCHAR nIndex)
	{
	if(nIndex > 2)
		return false;

	CTime ctNow(CTime::GetCurrentTime());
	if(m_ctLastSent[nIndex] == 0)
		{
		m_ctLastSent[nIndex] = ctNow;
		return true;
		}

	if( (ctNow - m_ctLastSent[nIndex]).GetMinutes() >= m_nEmailInterval)
		{
		m_ctLastSent[nIndex] = ctNow;
		return true;
		}

	return false;
	}

//--------------------------------------------------------------------------------
CDBSubSystem::CDBSubSystem(CSecuritySystem* pParent)
		: CSSSubSystem<CSubSystem>(pParent)
		, m_nNextCertId(0)
		, m_nEmailInterval(10)
	{
	m_pDB = new CDBCertificateMasters;
	m_pTokenInfoMap = new CDBTokenInfoMap;
	m_pCleanUpThread = new CDBCleanUpThread(this);
	m_pCleanUpThread->StartThread();
	m_pMagic = new CMagicNumber;
	}

//--------------------------------------------------------------------------------
CDBSubSystem::~CDBSubSystem()
	{

	{
	CWriteLock lock(m_pDB);
	for(POSITION pos = m_pDB->m_pList->GetTailPosition(); pos != NULL; )
		delete m_pDB->m_pList->GetPrev(pos);
	}

	{
	CWriteLock lock(m_pTokenInfoMap);
	for(POSITION pos = m_pTokenInfoMap->GetStartPosition(); pos != NULL; )
		{
		DWORD nKey;
		CDBTokenInfo* pData;
		m_pTokenInfoMap->GetNextAssoc(pos, nKey, pData);
		delete pData;
		}
	}

	m_pCleanUpThread->EndThread();
	delete m_pCleanUpThread;
	delete m_pDB;
	delete m_pTokenInfoMap;
	delete m_pMagic;
	}

//--------------------------------------------------------------------------------
bool CDBSubSystem::PostMessage(UINT nMsg, WPARAM, LPARAM)
	{
	if(nMsg == MsgCleanUp)
		m_pCleanUpThread->PostMessage(nMsg);
	return true;
	}

//--------------------------------------------------------------------------------
CMagicNumber* CDBSubSystem::GetMagNumGen() const
	{
	return m_pMagic;
	}

//--------------------------------------------------------------------------------
CDBCertificateMasters* CDBSubSystem::GetCertMasters() const
	{
	return m_pDB;
	}

//--------------------------------------------------------------------------------
CDBTokenInfoMap* CDBSubSystem::GetTokenInfoMap() const
	{
	return m_pTokenInfoMap;
	}

//--------------------------------------------------------------------------------
DWORD CDBSubSystem::GetNextCertificateId()
	{
	// in case of recomilation on a non-32bit platform
	ASSERT(sizeof(long) == sizeof(DWORD));

	return ::InterlockedIncrement((long*) &m_nNextCertId);
	}

//--------------------------------------------------------------------------------
CCertificateMaster* CDBSubSystem::FindCertMaster(const CCertificate& rCert)
	{
	CReadLock lock(GetCertMasters(), false);
	if(! lock.Lock(10000))
		return NULL;

	return (CCertificateMaster*) GetCertMasters()->Find(rCert.GetId());
	}

//--------------------------------------------------------------------------------
CCertificateMaster* CDBSubSystem::IssueCertificate(const CCertificate& rCert, LPCTSTR pIP)
	{
	// no point in any of this if we're not serving
	if(! GetSystem()->IsServing())
		{
		GetIO()->Output(IOMASK_1|IOMASK_DEBUG|IOMASK_CONST, "IssueCertificate (1)");
		return NULL;
		}

	// this function only handles new certificates
	if(rCert.GetCommand() != CCertificateCommands::CMD_NEW)
		{
		GetIO()->Output(IOMASK_1|IOMASK_DEBUG|IOMASK_CONST, "IssueCertificate (2)");
		return NULL;
		}

	// the requested token id should be the command parameter
	DWORD nTokenId = rCert.GetCommandParam();

	// cant have a token with id zero
	if(nTokenId == 0)
		{
		GetIO()->Output(IOMASK_1|IOMASK_DEBUG|IOMASK_CONST, "IssueCertificate (5)");
		return NULL;
		}

	DWORD nStart = ::GetTickCount();
	CWriteLock lock(GetCertMasters(), false);
	if(! lock.Lock(30000))
		{
		GetIO()->Output(IOMASK_1|IOMASK_DEBUG|IOMASK_CONST, "IssueCertificate (3) must be busy or deadlocked");
		return NULL;
		}

	DWORD nTotal = ::GetTickCount() - nStart;

	CCertificateMaster* pCert = new CCertificateMaster;

	// almost there - put data into the certificate
	pCert->SetCommand(CCertificateCommands::CMD_NEW);
	pCert->m_nMagicNumber = GetMagNumGen()->GetNext();
	pCert->m_nId = GetNextCertificateId();
	pCert->m_nElementId = pCert->m_nId;
	pCert->m_tmIssued = CTime::GetCurrentTime().GetTime();
	pCert->m_nUserClass = rCert.GetUserClass();
	pCert->m_ctLastRefresh = CTime::GetCurrentTime();
	pCert->m_bLocalTokenAlloc = false;
	AllocString(pCert->m_pIP, pIP);

	CSSConfigPerformance configPerf(this);
	pCert->m_nSecondsToLive = configPerf.m_nSecondsToLive;

	if(! IssueToken(pCert, nTokenId))
		{
		delete pCert;
		GetIO()->Output(IOMASK_1|IOMASK_DEBUG|IOMASK_CONST, "IssueCertificate (4)");
		return NULL;
		}

	pCert->SetCommandParam(CCertificateCommands::INFO_OK);
	return pCert;
	}

//--------------------------------------------------------------------------------
CCertificateMaster* CDBSubSystem::RefreshCertificate(const CCertificate& rCert, LPCTSTR pIP)
	{
	CReadLock lock(GetCertMasters(), false);
	if(! lock.Lock(10000))
		return NULL;

	CCertificateMaster* pCert = (CCertificateMaster*) GetCertMasters()->Find(rCert.GetId());

	if(pCert == NULL)
		return NULL;

	if( (! GetMagNumGen()->IsValid(pCert->GetMagicNumber()))
			|| (pCert->GetUserClass() != rCert.GetUserClass())
			|| (strcmp(pCert->GetIP(), pIP) != 0)
			|| (pCert->GetSecondsToLive() == 0))
		pCert = NULL;
	else
		{
		pCert->SetLastRefresh(CTime::GetCurrentTime());
		pCert->SetCommand(CCertificateCommands::CMD_REFRESH);
		pCert->SetCommandParam(CCertificateCommands::INFO_OK);
		}

	return pCert;
	}

//--------------------------------------------------------------------------------
bool CDBSubSystem::ReleaseCertificate(const CCertificate& rCert, LPCTSTR pIP)
	{
	CWriteLock lock(GetCertMasters(), false);
	if(! lock.Lock(10000))
		return false;

	CCertificateMaster* pCert = (CCertificateMaster*) GetCertMasters()->Find(rCert.GetId());

	if(pCert == NULL)
		return false;

	if(! GetMagNumGen()->IsValid(pCert->GetMagicNumber()))
		pCert = NULL;
	else
		if(pCert->GetUserClass() != rCert.GetUserClass())
			pCert = NULL;
		else
			if(strcmp(pCert->GetIP(), pIP) != 0)
				pCert = NULL;

	if(pCert != NULL)
		{
		pCert->m_nSecondsToLive = 0;

		GetIO()->FormatOutput(IOMASK_1, "Released certificate %d from %s at %s\n",
			pCert->GetId(), pIP, (LPCTSTR) CTime::GetCurrentTime().Format("%c"));
	
		PostMessage(MsgCleanUp);
		}

	return true;
	}

//--------------------------------------------------------------------------------
bool CDBSubSystem::ReleaseCertificate(int nId)
	{
	CWriteLock lock(GetCertMasters(), false);
	if(! lock.Lock(30000))
		return false;

	CCertificateMaster* pCert = (CCertificateMaster*) GetCertMasters()->Find(nId);

	if(pCert == NULL)
		return false;

	GetIO()->FormatOutput(IOMASK_1, "Released certificate %d from %s at %s\n",
		pCert->GetId(), pCert->GetIP(), (LPCTSTR) CTime::GetCurrentTime().Format("%c"));

	pCert->m_nSecondsToLive = 0;
	PostMessage(MsgCleanUp);
	return true;
	}

//--------------------------------------------------------------------------------
int CDBSubSystem::ReleaseCertificate(LPCTSTR pIP)
	{
	CWriteLock lock(GetCertMasters(), false);
	if(! lock.Lock(30000))
		return 0;

	int nCount = 0;
	for(POSITION pos = GetCertMasters()->GetHeadPosition(); pos != NULL;)
		{
		CCertificateMaster* pCert = (CCertificateMaster*) GetCertMasters()->GetNext(pos);
		if(pCert == NULL)
			return nCount;
		if(strcmp(pCert->GetIP(), pIP) == 0)
			{
			nCount++;

			GetIO()->FormatOutput(IOMASK_1, "Released certificate %d from %s at %s\n",
				pCert->GetId(), pIP, (LPCTSTR) CTime::GetCurrentTime().Format("%c"));

			pCert->m_nSecondsToLive = 0;
			PostMessage(MsgCleanUp);
			}
		}

	return nCount;
	}

//--------------------------------------------------------------------------------
int CDBSubSystem::ReleaseAllCertificates()
	{
	CWriteLock lock(GetCertMasters(), false);
	if(! lock.Lock(30000))
		return 0;

	int nCount = 0;
	for(POSITION pos = GetCertMasters()->GetHeadPosition(); pos != NULL;)
		{
		CCertificateMaster* pCert = (CCertificateMaster*) GetCertMasters()->GetNext(pos);
		if(pCert == NULL)
			return nCount;

		nCount++;

		GetIO()->FormatOutput(IOMASK_1, "Released certificate %d from %s at %s\n",
			pCert->GetId(), pCert->GetIP(), (LPCTSTR) CTime::GetCurrentTime().Format("%c"));

		pCert->m_nSecondsToLive = 0;
		PostMessage(MsgCleanUp);
		}

	return nCount;
	}

//--------------------------------------------------------------------------------
bool CDBSubSystem::IssueToken(CCertificateMaster*& pCert, ULONG nTokenId)
	{
	CDBTokenInfoMap* pDB = GetTokenInfoMap();
	if(pDB == NULL)
		{
		GetIO()->Output(IOMASK_CONST|IOMASK_ERR, "Program error - Token DB == null");
		return false;
		}

	CWriteLock dbLock(pDB, false);
	if(! dbLock.Lock(10000))
		{
		GetIO()->Output(IOMASK_CONST|IOMASK_ERR, "Program error - can't lock the token DB");
		return false;
		}

	CDBTokenInfo* pTokenInfo = NULL;
	pDB->Lookup(nTokenId, pTokenInfo);

	if(pTokenInfo == NULL)
		{
		pCert->SetCommandParam(CCertificateCommands::ERR_NO_SUCH_TOKEN);
		GetIO()->FormatOutput(IOMASK_1|IOMASK_CONST|IOMASK_DEBUG, "IssueCertificate Status 5 (%d)", nTokenId);
		return false;
		}

	// check for parent token
	if(pTokenInfo->m_nParentId > 0)
		{
		// the certificate must have the parent token to have this token allocated to it
		if(! pCert->HasToken(pTokenInfo->m_nParentId))
			return false;
		}

	// here's the bit where we send a status email
	if(pTokenInfo->m_nInUse > pTokenInfo->m_nCount && CanSendEmailNow(1))
		{
		CSSQuickMail* pMail = new CSSQuickMail;
		pMail->m_sSubject = "FYI: License Count Exceeded";
		pMail->m_sMsg.Format("%s\r\nToken: %ld (%s)\r\nCount: %ld\r\nIn Use: %ld\r\nMax: %ld\r\n",
			(LPCTSTR) CTime::GetCurrentTime().Format("%c"),
			pTokenInfo->m_token.GetId(), pTokenInfo->m_token.GetName(),
			pTokenInfo->m_nCount, pTokenInfo->m_nInUse, pTokenInfo->m_nMax);
		GetIO()->SendQuickMail(pMail);
		}

	if(pTokenInfo->m_nInUse == pTokenInfo->m_nMax)
		{
		pCert->SetCommandParam(CCertificateCommands::ERR_TOKEN_NOT_AVAIL);
		GetIO()->Output(IOMASK_CONST|IOMASK_ERR, "No more tokens available");
		return false;
		}

	pCert->GetTokenMap()->SetAt(pTokenInfo->m_token.GetId(), &pTokenInfo->m_token);
	pTokenInfo->m_pCertificates->Add(pCert);
	pTokenInfo->m_nInUse++;

	pCert->GetTokenInfoMap()->SetAt(pTokenInfo->m_token.GetId(), pTokenInfo);

	// dont need a write lock because IssueCertificate has one already
	if(! GetCertMasters()->Find(pCert->GetId()))
		if(! GetCertMasters()->Add(pCert))
			{
			GetIO()->Output(IOMASK_CONST|IOMASK_ERR, "Program error - can't add certificate to internal database");
			return false;
			}

	return true;
	}

//--------------------------------------------------------------------------------
bool CDBSubSystem::ReleaseToken(const CCertificate& rCert, ULONG nTokenId)
	{
	CWriteLock lock(GetCertMasters(), false);
	if(! lock.Lock(10000))
		return false;

	CCertificateMaster* pCert = (CCertificateMaster*) GetCertMasters()->Find(rCert.GetId());

	if(pCert == NULL)
		return false;

	CDBTokenInfoMap* pDB = pCert->GetTokenInfoMap();
	if(pDB == NULL)
		{
		GetIO()->Output(IOMASK_CONST|IOMASK_ERR, "Program error - Token DB == null");
		return false;
		}

	CWriteLock dbLock(pDB, false);
	if(! dbLock.Lock(30000))
		{
		GetIO()->Output(IOMASK_CONST|IOMASK_ERR, "Program error - can't lock the token DB");
		return false;
		}

	CDBTokenInfo* pTokenInfo = NULL;
	pDB->Lookup(nTokenId, pTokenInfo);

	// trying to release a token that's not associated with the certificate
	if(pTokenInfo == NULL)
		{
		// token not found
		GetIO()->Output(IOMASK_1|IOMASK_CONST|IOMASK_DEBUG, "IssueCertificate Status 5");
		return false;
		}

	CWriteLock dbLock2(pTokenInfo->m_pCertificates, false);
	if(! dbLock2.Lock(10000))
		return false;

	// remove token's references to this certificate
	int nCount = pTokenInfo->m_pCertificates->Remove(pCert, false);

	// decrement the tokens in use counter
	if(::InterlockedExchangeAdd((LONG*) &pTokenInfo->m_nInUse, -nCount) < 0)
		::InterlockedExchange((LONG*) &pTokenInfo->m_nInUse, 0);

	// remove the token from this certificate
	pCert->GetTokenInfoMap()->RemoveKey(nTokenId);

	// see if there are any child tokens of the token we just released
	// if so then they must be released as well
	CDWordArray nChildTokens;
	for(POSITION pos = pDB->GetStartPosition(); pos != NULL; )
		{
		DWORD nChildId;
		pDB->GetNextAssoc(pos, nChildId, pTokenInfo);
		if(pTokenInfo->m_nParentId == nTokenId)
			nChildTokens.Add(nChildId);
		}

	nCount = nChildTokens.GetUpperBound();
	while(nCount >= 0)
		{
		ReleaseToken(rCert, nChildTokens[nCount]);
		nChildTokens.RemoveAt(nCount--);
		}

	return true;
	}

//--------------------------------------------------------------------------------
bool CDBSubSystem::LoadFromFile()
	{
	CLicenseFile file;

	if(! file.Open(GetSystem()->GetLicenseFilename(), CFile::modeRead|CFile::shareExclusive))
		{
		GetIO()->FormatOutput(IOMASK_1, "Failed to open: %s\n", (LPCTSTR) GetSystem()->GetLicenseFilename());
		return false;
		}

	CDBTokenInfoMap* pDB = GetDB()->GetTokenInfoMap();
	CWriteLock lock(pDB, false);
	if(! lock.Lock(30000))
		return false;

	if(! file.Read())
		return false;

	// go through the list of tokens
	for(int i = 0; i < file.m_tokens.GetSize(); i++)
		{
		CDBTokenInfo* pTokenInfo = file.m_tokens.GetAt(i);

		// is there already a token with this id
		CDBTokenInfo* pExisting = NULL;
		if(pDB->Lookup(pTokenInfo->m_token.m_nId, pExisting))
			{
			// only set numbers if we're increasing licenses
			// if anything's being decreased, the system should be restarted
			if(pTokenInfo->m_nMax >= pExisting->m_nMax)
				{
				pExisting->m_nCount = pTokenInfo->m_nCount;
				pExisting->m_nMax = pTokenInfo->m_nMax;
				}
			delete pTokenInfo;
			}
		else
			// there's no existing token so we're adding a new one
			pDB->SetAt(pTokenInfo->m_token.m_nId, pTokenInfo);
		}

	// we're maintaining the CDBTokenInfo pointers so we'll delete them later
	// remove them from the list so the file.dtor doesnt delete them
	file.m_tokens.RemoveAll();

	CSSConfig configMaster(this);
	configMaster.SetDataFromLicenseFile(&file);

	CSSConfigBackup configBackup;
	if(configBackup.m_bIsBackup)
		GetSystem()->StartBackupMode();
	else
		GetSystem()->StartMainserverMode();

	// check for license file expiration
	CSSConfigGeneral config(this);
	if(CTime::GetCurrentTime() >= config.m_ctExpiration)
		return LicenseFileFailed(0);

	// make sure that we're on the correct machine
	if(config.m_sP3Serial.IsEmpty())
		return LicenseFileFailed(1);

	CString sTemp;
	if(! CPlatformInfo::GetProcessorSerialNumber(sTemp))
		return LicenseFileFailed(2);

	if(sTemp != config.m_sP3Serial)
		return LicenseFileFailed(3);

	GetSystem()->SetLicenseStatus(true);
	return true;
	}

//--------------------------------------------------------------------------------
bool CDBSubSystem::LicenseFileFailed(int nStatus)
	{
	CString sTemp;
	sTemp.Format("Status %d", nStatus);

	GetIO()->FormatOutput(IOMASK_1|IOMASK_CONST|IOMASK_DEBUG, "ERR: " + sTemp);
	CSSQuickMail* pMail = new CSSQuickMail;
	pMail->m_sSubject = pMail->m_sMsg = sTemp;
	GetIO()->SendQuickMail(pMail);
	GetSystem()->SetLicenseStatus(false);
	return false;
	}
