//--------------------------------------------------------------------------------
//
// Copyright (c) 2001 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "BackupThread.h"
#include "SSConfigBackup.h"
#include "SSConfigGeneral.h"
#include "Dongle.h"
#include "SecServTokenId.h"
#include "SSQuickMail.h"

#include <Certificate.h>
#include "SecServTokenId.h"	

#include <SmallSocket.h>

#define COMMWAITLEN 6000

// the possible states are:
// 1 - the main server is running
// 2 - the main server is down so we have taken over and are waiting for a dongle
//     we'll run fine until the dongle deadline passes
// 3 - the main server is down and the dongle has been moved to this server
// 4 - the main server has come back up
//--------------------------------------------------------------------------------
CBackupThread::CBackupThread(CSystemObject* pParent)
		: CSSThread(pParent)
		, m_ctStarted((time_t) 0)
	{
	}

//--------------------------------------------------------------------------------
CBackupThread::~CBackupThread()
	{
	}

//--------------------------------------------------------------------------------
bool CBackupThread::Init()
	{
	SetThreadPriority(THREAD_PRIORITY_HIGHEST);
	return true;
	}

//--------------------------------------------------------------------------------
bool CBackupThread::MainLoop()
	{
	bool bMainRunning = ConnectToMainServer();

	if(GetParent()->IsServing())
		{
		if(bMainRunning)
			{
			GetParent()->SetIsServing(false);
			CSSQuickMail* pMail = new CSSQuickMail;
			pMail->m_sSubject.Format("IMP: Main back %s", (LPCTSTR) CTime::GetCurrentTime().Format("%c"));
			pMail->m_sMsg = pMail->m_sSubject + "\r\nBackup returning to backup status\r\n";
			GetIO()->SendQuickMail(pMail);
			GetDB()->ReleaseAllCertificates();
			}
		}
	else
		{
		if(! bMainRunning)
			{
			GetParent()->SetIsServing(true);
			CSSQuickMail* pMail = new CSSQuickMail;
			pMail->m_sSubject.Format("IMP: Backup taking over %s", (LPCTSTR) CTime::GetCurrentTime().Format("%c"));
			pMail->m_sMsg = pMail->m_sSubject + "\r\n";
			GetIO()->SendQuickMail(pMail);
			}
		}

	return CThreadObject::MainLoop();
	}

//--------------------------------------------------------------------------------
DWORD CBackupThread::GetThreadLoopSleepInterval() const
	{
	return 1000;
	}

//--------------------------------------------------------------------------------
bool CBackupThread::ConnectToMainServer()
	{
	static CTime ctLastConnectOk((time_t) 0);

	if((CTime::GetCurrentTime() - ctLastConnectOk).GetTotalSeconds() < 10)
		return true;

	CSmallSocket socket;

	CSSConfigGeneral configGeneral;
	if(! socket.Create(0, configGeneral.m_sMainIP))
		return false;

	CSSConfigBackup config;
	if(! socket.Connect(config.m_sIP, config.m_nPort))
		return false;

	CString sTemp;
	// get the server hello string
	if(socket.Receive(sTemp, CSmallSocket::WAITFORDATA, COMMWAITLEN) < 1)
		return false;

	if(sTemp.GetLength() < 3)
		return false;

	if(sTemp.Left(3) != "000")
		return false;

	char buf[512];
	CCertificate cert(CCertificateCommands::CMD_BACKUPCONNECT, 0, SSTokenId::BackupServer);

	int nLen = cert.Encode(buf, false);

	if(socket.Send(buf, nLen, CSmallSocket::WAITFORWOULDBLOCK, COMMWAITLEN) != nLen)
		return false;

	if(socket.Receive(sTemp, CSmallSocket::WAITFORDATA, COMMWAITLEN) < 1)
		return false;

	if(sTemp == "MainOk")
		{
		ctLastConnectOk = CTime::GetCurrentTime();
		return true;
		}

	return false;
	}
