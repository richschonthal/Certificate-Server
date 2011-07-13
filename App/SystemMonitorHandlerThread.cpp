//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include <LockedSocket.h>
#include <ReadLock.h>
#include <WriteLock.h>
#include <PlatformInfo.h>

#include "SecuritySystem.h"
#include "SystemMonitorHandlerThread.h"
#include "MonitorSubSystem.h"
#include "DBSubSystem.h"
#include "DBTokens.h"
#include "SSConfigMonitor.h"
#include "SSConfigGeneral.h"
#include "SSConfigBackup.h"
#include "SSConfigGeneral.h"
#include "SSConfigPerformance.h"
#include "SecServTokenId.h"
#include "SSQuickMail.h"
#include "dongle.h"

#define SECSERV_VERSION_STRING "071701"

//--------------------------------------------------------------------------------
CSystemMonitorHandlerThread::CSystemMonitorHandlerThread(CMonitorSubSystem* pParent)
		: CSSThread(pParent)
		, m_bLoggedIn(false)
	{
#ifdef _DEBUG
	m_bLoggedIn = true;
#endif
	}

//--------------------------------------------------------------------------------
CSystemMonitorHandlerThread::~CSystemMonitorHandlerThread()
	{
	if(::GetCurrentThreadId() != m_nThreadId)
		::WaitForSingleObject(m_hThread, INFINITE);
	}

//--------------------------------------------------------------------------------
void CSystemMonitorHandlerThread::OnMessage(UINT nMsg, WPARAM hSocket, LPARAM lParam)
	{
	if(nMsg != CMonitorSubSystem::MSG_NEWCONNECTION)
		return;
	m_socket.Attach((SOCKET) hSocket);
	SetThreadBusy(true);
	CSSQuickMail* pMail = new CSSQuickMail;
	pMail->m_sSubject = "FYI: Control Connection Made";
	CString sSockAddr;
	UINT nPort;
	m_socket.GetPeerName(sSockAddr, nPort);
	pMail->m_sMsg.Format("client %s port=%u\r\n", (LPCTSTR) sSockAddr, nPort);
	GetIO()->SendQuickMail(pMail);
	}

//--------------------------------------------------------------------------------
bool CSystemMonitorHandlerThread::MainLoop()
	{
	CReadLock lock(GetStatePtr());

	if(! IsThreadBusy())
		return CheckState();

	// m_pSocket wont be valid until MSG_NEWCONNECTION msg is received
	if(! m_socket.IsValid())
		{
		SetThreadBusy(false);
		return CheckState();
		}

#ifndef _DEBUG
	CTime ctCurrent(CTime::GetCurrentTime());
	if(m_ctLastActivity < ctCurrent - CTimeSpan(0, 0, 5, 0))
		{
		CString sTemp("----* Session timed out *----\r\n");
		m_socket.Send(sTemp, CSmallSocket::WAITFORWOULDBLOCK);
		SetThreadBusy(false);
		return CheckState();
		}
#endif

	CString sTemp;
	int nLen = 0;

	nLen = m_socket.Receive(sTemp.GetBuffer(1024), 1024, CSmallSocket::WAITFORDATA, 1000);

	// when there's no data (but no error) Receive will return len==-1 and err == WSAEWOULDBLOCK
	long nErr = ::WSAGetLastError();
	if(nLen == -1 && nErr != WSAEWOULDBLOCK)
		{
		::WSASetLastError(nErr);
		sTemp.ReleaseBuffer(0);
		SetThreadBusy(false);
		return CheckState();
		}

	if(nLen < 1)
		nLen = 0;

	sTemp.ReleaseBuffer(nLen);

	if(nLen > 0)
		{
#ifndef _DEBUG
		m_ctLastActivity = ctCurrent;
#endif

		int nIndex = sTemp.FindOneOf("\r\n");
		if(nIndex != -1)
			{
			sTemp.GetBufferSetLength(nIndex);
			sTemp.ReleaseBuffer();
			m_sCurCmd += sTemp;
			if(! DoCommand())
				SetThreadBusy(false);
			}
		else
			if(sTemp.GetLength() > 0)
				m_sCurCmd += sTemp;
		}

	return CheckState();
	}

//--------------------------------------------------------------------------------
bool CSystemMonitorHandlerThread::Init()
	{
	SetThreadPriority(THREAD_PRIORITY_BELOW_NORMAL);
	return true;
	}

//--------------------------------------------------------------------------------
bool CSystemMonitorHandlerThread::OnDeactivate()
	{
	m_sUser.Empty();
	m_sPass.Empty();
	m_bLoggedIn = false;
	m_socket.Close();
	return true;
	}

//--------------------------------------------------------------------------------
bool CSystemMonitorHandlerThread::OnActivate()
	{
	m_ctLastActivity = CTime::GetCurrentTime();
	return true;
	}

//--------------------------------------------------------------------------------
static LPCTSTR g_pCommands[] =
	{"load", "list", "pause", "continue", "exit", 
	"revoke", "help", "serial", "user", "pass", "dongle",
	"config"};

enum { CMDLOAD, CMDLIST, CMDPAUSE, CMDCONT, CMDEXIT,
	CMDREVOKE, CMDHELP, CMDSERIAL, CMDUSER, CMDPASS, CMDDONGLE, 
	CMDCONFIG, NUM_OF_CMDS };

//--------------------------------------------------------------------------------
bool CSystemMonitorHandlerThread::DoCommand()
	{
	ASSERT(NUM_OF_CMDS == sizeof(g_pCommands) / sizeof(LPCTSTR));

	bool bRv = true;

	m_sCurCmd.MakeLower();
	m_sCurCmd.TrimLeft();
	m_sCurCmd.TrimRight();

	TRY
		{
		CString sTemp(m_sCurCmd.Left(4));

		if(sTemp == "hey")
			m_socket.Send("there\r\n\r\n", 6, CSmallSocket::WAITFORWOULDBLOCK);
		else
			{
			for(int i = 0; i < NUM_OF_CMDS; i++)
				{
				if(strncmp(sTemp, g_pCommands[i], 4) == 0)
					break;
				}

			if(i == CMDEXIT)
				{
				m_sCurCmd.Empty();
				m_sUser.Empty();
				m_sPass.Empty();
				return false;
				}

			if(i == CMDUSER)
				{
				if(m_sCurCmd.GetLength() > 5)
					DoUser(m_sCurCmd.Mid(5));
				m_sCurCmd.Empty();
				return true;
				}

			if(i == CMDPASS)
				{
				if(m_sCurCmd.GetLength() > 5)
					DoPass(m_sCurCmd.Mid(5));
				m_sCurCmd.Empty();
				return true;
				}

			if(! m_bLoggedIn)
				{
				m_sCurCmd.Empty();
				return true;
				}

			switch(i)
				{
				case CMDLOAD:
					if(m_sCurCmd.GetLength() > 5)
						DoLoad(m_sCurCmd.Mid(5));
					else
						{
						CSSConfigGeneral config;
						DoLoad(config.m_sLicenseFileDefault);
						}
					break;
				case CMDLIST:
					DoList();
					break;
				case CMDPAUSE:
					DoPause();
					break;
				case CMDCONT:
					DoContinue();
					break;
				case CMDREVOKE:
					DoRevoke();
					break;
				case CMDHELP:
					{
					for(int i = 0; i < NUM_OF_CMDS; i++)
						{
						CString sTemp;
						sTemp.Format("%s\r\n", g_pCommands[i]);
						m_socket.Send(sTemp, sTemp.GetLength(), CSmallSocket::WAITFORWOULDBLOCK);
						}
					}
					break;
				case CMDSERIAL:
					{
					CString sTemp;
					for(int i = 0; CPlatformInfo::GetProcessorSerialNumber(sTemp, i); i++)
						{
						sTemp += "\r\n";
						m_socket.Send(sTemp, sTemp.GetLength(), CSmallSocket::WAITFORWOULDBLOCK);
						}
					}
					break;

				case CMDDONGLE:
					DoDongle();
					break;

				case CMDCONFIG:
					DoConfigDump();
					break;

				default:
					m_socket.Send("what?\r\n\r\n", 6, CSmallSocket::WAITFORWOULDBLOCK);
					break;
				}
			}
		}
	CATCH_ALL(e)
		{
		}
	END_CATCH_ALL

	m_sCurCmd.Empty();
	return bRv;
	}

//--------------------------------------------------------------------------------
void CSystemMonitorHandlerThread::DoUser(LPCTSTR pUser)
	{
	CSSConfigMonitor config;
	if(pUser == config.m_sUser)
		m_sUser = pUser;
	else
		m_sUser.Empty();

	m_bLoggedIn = m_sUser == config.m_sUser && m_sPass == config.m_sPass;
	}

//--------------------------------------------------------------------------------
void CSystemMonitorHandlerThread::DoPass(LPCTSTR pPass)
	{
	CSSConfigMonitor config;
	if(pPass == config.m_sPass)
		m_sPass = pPass;
	else
		m_sPass.Empty();

	m_bLoggedIn = m_sUser == config.m_sUser && m_sPass == config.m_sPass;

	if(! m_bLoggedIn)
		{
		m_sUser.Empty();
		m_sPass.Empty();
		}
	}

//--------------------------------------------------------------------------------
void CSystemMonitorHandlerThread::DoPause()
	{
	CString sTemp("This command does nothing - surprised?\r\n");
	m_socket.Send(sTemp, sTemp.GetLength(), CSmallSocket::WAITFORWOULDBLOCK);
//	GetSystem()->SetState(STATE_PAUSE);
//	GetSystem()->WaitForStateSync();
	}

//--------------------------------------------------------------------------------
void CSystemMonitorHandlerThread::DoContinue()
	{
	CString sTemp("Is pause does nothing, what makes you think this command would?\r\n");
	m_socket.Send(sTemp, sTemp.GetLength(), CSmallSocket::WAITFORWOULDBLOCK);
//	GetSystem()->SetState(STATE_RUN);
//	GetSystem()->WaitForStateSync();
	}

//--------------------------------------------------------------------------------
void CSystemMonitorHandlerThread::DoLoad(LPCTSTR pFilename)
	{
	CString sTemp(pFilename);

	if(! sTemp.IsEmpty())
		GetSystem()->SetLicenseFilename(sTemp);
	if(GetDB()->LoadFromFile())
		sTemp = "file loaded ok\r\n";
	else
		sTemp = "error while loading file\r\n";

	m_socket.Send(sTemp, sTemp.GetLength(), CSmallSocket::WAITFORWOULDBLOCK);
	}

//--------------------------------------------------------------------------------
void CSystemMonitorHandlerThread::DoList()
	{
	if(m_sCurCmd.Find("all") != -1)
		{
		DoListAll();
		return;
		}

	CDBTokenInfoMap* pDB = GetDB()->GetTokenInfoMap();
	CString sTemp;
	CReadLock lock(pDB, false);
	if(! lock.Lock(30000))
		{
		sTemp = "can't lock the database right now (probably a lot of activity)\r\n";
		m_socket.Send(sTemp, sTemp.GetLength(), CSmallSocket::WAITFORWOULDBLOCK);
		return;
		}

	sTemp = "  Id         Name In Use  Count    Max\r\n";
	m_socket.Send(sTemp, sTemp.GetLength(), CSmallSocket::WAITFORWOULDBLOCK);

	for(POSITION pos = pDB->GetStartPosition(); pos != NULL; )
		{
		DWORD nId;
		CDBTokenInfo* pInfo;

		pDB->GetNextAssoc(pos, nId, pInfo);
		if(! pInfo)
			continue;

		sTemp.Format("%4d %12s %6d %6d %6d\r\n",
			nId, pInfo->m_token.GetName(), pInfo->m_nInUse, pInfo->m_nCount, pInfo->m_nMax);

		m_socket.Send(sTemp, sTemp.GetLength(), CSmallSocket::WAITFORWOULDBLOCK);
		}

	if(GetSystem()->IsBackupServer())
		sTemp.Format("%s%s\r\n", "Running as backup server", GetSystem()->IsServing() ? " currently serving" : "");
	else
		sTemp.Format("Backup SecurityServer is %s\r\n", GetSystem()->IsBackupRunning() ? "running" : "not running");

	m_socket.Send(sTemp, sTemp.GetLength(), CSmallSocket::WAITFORWOULDBLOCK);

#ifdef _DEBUG
#pragma message("Don't forget to change the listed version for the next release!")
#endif

	sTemp.Format("This server is version " SECSERV_VERSION_STRING "\r\n");
	m_socket.Send(sTemp, sTemp.GetLength(), CSmallSocket::WAITFORWOULDBLOCK);
	}

//--------------------------------------------------------------------------------
void CSystemMonitorHandlerThread::DoListAll()
	{
	CString sTemp;

	CReadLock lock(GetDB()->GetCertMasters(), false);
	if(! lock.Lock(10000))
		{
		sTemp = "Can't lock the certificate database right now )\r\n";
		m_socket.Send(sTemp, CSmallSocket::WAITFORWOULDBLOCK);
		return;
		}

	POSITION pos = GetDB()->GetCertMasters()->GetHeadPosition();
	if(pos == NULL)
		{
		sTemp = "there are currently 0 connections\r\n";
		m_socket.Send(sTemp, CSmallSocket::WAITFORWOULDBLOCK);
		return;
		}

	CReadLock lock2(GetDB()->GetTokenInfoMap(), false);
	if(! lock2.Lock(1000))
		{
		sTemp = "Can't lock the token database right now\r\n";
		m_socket.Send(sTemp, sTemp.GetLength(), CSmallSocket::WAITFORWOULDBLOCK);
		return;
		}

	sTemp = "IP              Last Refresh          Cert ID User Class Token Name\r\n";
	m_socket.Send(sTemp, sTemp.GetLength(), CSmallSocket::WAITFORWOULDBLOCK);

	while(pos != NULL)
		{
		CCertificateMaster* pCert = (CCertificateMaster*) GetDB()->GetCertMasters()->GetNext(pos);
		if(! pCert)
			break;

		CReadLock lock(pCert, false);
		if(! lock.Lock(1000))
			{
			sTemp = "Can't lock one of the certificates right now\r\n";
			m_socket.Send(sTemp, sTemp.GetLength(), CSmallSocket::WAITFORWOULDBLOCK);
			continue;
			}

		POSITION pos = pCert->GetTokenMap()->GetStartPosition();
		LPCTSTR pFormat = "%-15s %-20s %8ld %10ld  %-16s\r\n";
		if(pos == NULL)
			{
			sTemp.Format(pFormat,
				pCert->GetIP(), 
				(LPCTSTR) pCert->GetLastRefresh().Format("%c"),
				pCert->GetId(),
				pCert->GetUserClass(),
				"");
			m_socket.Send(sTemp, sTemp.GetLength(), CSmallSocket::WAITFORWOULDBLOCK);
			}
		else
			{
			while(pos != NULL)
				{
				CAccessToken* pTok;
				ULONG nId;
				pCert->GetTokenMap()->GetNextAssoc(pos, nId, pTok);
				if(pTok == NULL)
					break;
				sTemp.Format(pFormat,
					pCert->GetIP(), 
					(LPCTSTR) pCert->GetLastRefresh().Format("%c"),
					pCert->GetId(),
					pCert->GetUserClass(),
					pTok->GetName());
				m_socket.Send(sTemp, sTemp.GetLength(), CSmallSocket::WAITFORWOULDBLOCK);
				}
			}
		}
	}

//--------------------------------------------------------------------------------
void CSystemMonitorHandlerThread::DoRevoke()
	{
	int nIndex = m_sCurCmd.ReverseFind(' ');
	if(nIndex == -1)
		return;

	if(nIndex == (m_sCurCmd.GetLength() - 1))
		return;

	CString sTemp(m_sCurCmd.Mid(nIndex + 1));

	if(sTemp == "all")
		{
		DoRevokeAll();
		return;
		}

	if(sTemp.Find('.') != -1)
		{
		DoRevokeByIP(sTemp);
		return;
		}

	DWORD nId = (DWORD) atol(sTemp);

	if(! GetDB()->ReleaseCertificate(nId))
		sTemp.Format("certificate %d not found\n\r", nId);
	else
		sTemp.Format("certificate %d revoked\r\n", nId);

	m_socket.Send(sTemp, sTemp.GetLength(), CSmallSocket::WAITFORWOULDBLOCK);
	}

//--------------------------------------------------------------------------------
void CSystemMonitorHandlerThread::DoRevokeByIP(const CString& sIP)
	{
	CString sTemp;

	if(! GetDB()->ReleaseCertificate(sIP))
		sTemp.Format("no certificates issued to %s\n\r", (LPCTSTR) sIP);
	else
		sTemp.Format("certificates issued to %s revoked\n\r", (LPCTSTR) sIP);

	m_socket.Send(sTemp, sTemp.GetLength(), CSmallSocket::WAITFORWOULDBLOCK);
	}

//--------------------------------------------------------------------------------
void CSystemMonitorHandlerThread::DoRevokeAll()
	{
	CString sTemp;

	if(! GetDB()->ReleaseAllCertificates())
		sTemp = "certificates not found\n\r";
	else
		sTemp = "all certificate revoked\r\n";

	m_socket.Send(sTemp, sTemp.GetLength(), CSmallSocket::WAITFORWOULDBLOCK);
	}

//--------------------------------------------------------------------------------
void CSystemMonitorHandlerThread::DoDongle()
	{
	CDongle dongle;
	BYTE buf[128];
	CString sTemp;
	if(dongle.GetDongleMemory(buf))
		{
		for(int i = 0; i < 128; i++)
			{
			if(! isgraph(buf[i]))
				buf[i] = '.';
			}
		buf[127] = 0;
		sTemp = buf;
		}
	else
		sTemp = "** Error reading dongle **";

	sTemp += "\r\n";

	m_socket.Send(sTemp, CSmallSocket::WAITFORWOULDBLOCK);
	}

//--------------------------------------------------------------------------------
void CSystemMonitorHandlerThread::DoConfigDump()
	{
	CString sTemp("Configuration:\r\n");

	if(! m_socket.Send(sTemp, CSmallSocket::WAITFORWOULDBLOCK))
		return;

	CStringArray sArray;
	GetSystem()->GetSystemConfigStringMessage(sArray);
	for(int i = 0; i < sArray.GetSize(); i++)
		if(! m_socket.Send(sArray[i] + "\r\n", CSmallSocket::WAITFORWOULDBLOCK))
			return;
	}
