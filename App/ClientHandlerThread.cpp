//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"

#include <LockedSocket.h>

#include "SecurityServer.h"
#include "ClientSubSystem.h"
#include "SSIOSubSystem.h"
#include "DBSubSystem.h"
#include "SSConfigBackup.h"
#include "ClientHandlerThread.h"
#include "CertificateMaster.h"
#include "AccessToken.h"
#include "SSQuickMail.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#if 0
#define RECEIVECERTWAIT INFINITE
#define RECEIVEPORTCONFIRM INFINITE
#else
#define RECEIVECERTWAIT 6000
#define RECEIVEPORTCONFIRM 6000
#endif

//--------------------------------------------------------------------------------
LPCTSTR CClientHandlerThread::m_pMsgHello = "000 MCMS_SECSERV1000\n";
LPCTSTR CClientHandlerThread::m_pMsgServerDown = "999 SERVDWN\n";
// max clients that one client handler thread will support
int CClientHandlerThread::m_nMaxClients = 32;
// the max # of minutes a connection is allowed to be idle before its deleted
int CClientHandlerThread::m_nMaxIdleTime = 3;

//--------------------------------------------------------------------------------
CClientHandlerThread::CClientHandlerThread(CSystemObject* pParent)
		: CSSThread(pParent)
	{
	}

//--------------------------------------------------------------------------------
CClientHandlerThread::~CClientHandlerThread()
	{
	MSG msg;
	while(::PeekMessage(&msg, NULL, CClientSubSystem::MSG_NEWCONNECTION, CClientSubSystem::MSG_NEWCONNECTION, PM_REMOVE))
		delete (CSmallSocket*) msg.wParam;
	}

//--------------------------------------------------------------------------------
bool CClientHandlerThread::Init()
	{
	SetThreadPriority(THREAD_PRIORITY_HIGHEST);
	return true;
	}

//--------------------------------------------------------------------------------
void CClientHandlerThread::OnMessage(UINT nMsg, WPARAM hSocket, LPARAM)
	{
	CSmallSocket socket;
	socket.Attach((SOCKET) hSocket);

	if(nMsg == WM_QUIT || nMsg != CClientSubSystem::MSG_NEWCONNECTION)
		return;

	ULONG nState = GetState();
	// if we're not in a run state then tell the client that the server is down
	bool bAccepting = (nState == STATE_RUN) && GetSystem()->IsServing() && GetSystem()->IsLicensed();
	LPCTSTR pMsg = bAccepting ? m_pMsgHello : m_pMsgServerDown;
	long nLen = strlen(pMsg);

	// send the hello msg
	if(! socket.Send(pMsg, CSmallSocket::WAITFORWOULDBLOCK, 2000))
		{
		GetIO()->FormatOutput(IOMASK_ERR, "1001 socket error %ld", ::WSAGetLastError());
		return;
		}

	// we told the client that the server is down so exit
	if(! bAccepting)
		return;

	CCertificate cert;
	if(! ReceiveCertificate(cert, socket))
		return;

	int nCmd = cert.GetCommand();
	switch(nCmd)
		{
		case CCertificateCommands::CMD_BACKUPCONNECT:
			DoBackupServerConnect(cert, socket);
			break;

		case CCertificateCommands::CMD_NEW:
			if(cert.GetId() == 0)
				DoNewCertificate(cert, socket);
			else
				DoAquireToken(cert, socket);
			break;

		case CCertificateCommands::CMD_REFRESH:
			DoRefreshCertificate(cert, socket);
			break;

		case CCertificateCommands::CMD_RELEASE:
			DoReleaseCertificate(cert, socket);
			break;

		default:
			GetIO()->Output(IOMASK_ERR|IOMASK_CONST, "Illegal command received");
			break;
		}
	}

//--------------------------------------------------------------------------------
bool CClientHandlerThread::ReceiveCertificate(CCertificate& rCert, CSmallSocket& socket)
	{
	if(! socket.WaitForBytesAvail(RECEIVECERTWAIT))
		return false;

	char sTemp[4096];
	int nLen = socket.Receive(sTemp, 4096, CSmallSocket::WAITFORDATA, RECEIVECERTWAIT);
	if(nLen < 1)
		return false;

	sTemp[nLen] = 0;

	rCert.Decode(sTemp, nLen);

	return true;
	}

//--------------------------------------------------------------------------------
bool CClientHandlerThread::DoBackupServerConnect(const CCertificate& cert, CSmallSocket& socket)
	{
	CString sAddr;
	UINT nPort;

	if(! socket.GetPeerName(sAddr, nPort))
		{
		GetIO()->Output(IOMASK_ERR|IOMASK_CONST, "Error obtaining client IP address");
		return false;
		}

	CSSConfigBackup config;
	CString sTemp;
	if(sAddr == config.m_sIP)
		sTemp = "MainOk";
	else
		sTemp = "GoAway";

	if(! socket.Send(sTemp, CSmallSocket::WAITFORWOULDBLOCK, 10000))
		{
		GetIO()->Output(IOMASK_ERR|IOMASK_CONST, "Send error DBCS 1");
		return false;
		}

	if(! GetSystem()->IsBackupRunning())
		{
		CSSQuickMail* pMail = new CSSQuickMail;
		pMail->m_sSubject = "IMP: Backup Connection Made";
		pMail->m_sMsg.Format("%s %s\r\n", (LPCTSTR) sAddr, (LPCTSTR) CTime::GetCurrentTime().Format("%c"));
		GetIO()->SendQuickMail(pMail);
		}

	GetSystem()->SetBackupIsRunning();
	return true;
	}

//--------------------------------------------------------------------------------
bool CClientHandlerThread::DoNewCertificate(const CCertificate& cert, CSmallSocket& socket)
	{
	CString sAddr;
	UINT nPort;
	if(! socket.GetPeerName(sAddr, nPort))
		{
		GetIO()->Output(IOMASK_ERR|IOMASK_CONST, "Error obtaining client IP address");
		return false;
		}


	CCertificateMaster* pCert = GetDB()->IssueCertificate(cert, sAddr);
	if(pCert == NULL)
		{
		GetIO()->Output(IOMASK_1|IOMASK_DEBUG|IOMASK_CONST, "IssueCertificate returns NULL (1)");
		return false;
		}

	char temp[4096];
	int nLen = pCert->Encode(temp);

	// send the new certificate
	if(socket.Send(temp, nLen, CSmallSocket::WAITFORWOULDBLOCK, 10000) != nLen)
		{
		GetIO()->Output(IOMASK_1|IOMASK_DEBUG|IOMASK_CONST, "error certificate not sent");
		return false;
		}

	GetIO()->FormatOutput(IOMASK_1|IOMASK_DEBUG, "issused certificate %ld", pCert->GetId());
	return true;
	}

//--------------------------------------------------------------------------------
using namespace CCertificateCommands;

//--------------------------------------------------------------------------------
bool CClientHandlerThread::DoAquireToken(CCertificate& cert, CSmallSocket& socket)
	{
	UINT nInPort;
	CString sAddr;

	if(! socket.GetPeerName(sAddr, nInPort))
		{
		GetIO()->Output(IOMASK_ERR, "Error obtaining client IP address");
		return false;
		}

	CCertificateMaster* pCert = GetDB()->FindCertMaster(cert);

	// only issue tokens when we are serving
	if(GetSystem()->IsServing() && pCert != NULL)
		GetDB()->IssueToken(pCert, cert.GetCommandParam());

	char temp[4096];
	int nLen = 0;

	if(pCert == NULL)
		{
		cert.SetCommand(CMD_REFRESH, ERR_NO_SUCH_CERT);
		nLen = cert.Encode(temp, false);
		}
	else
		nLen = pCert->Encode(temp, true);

	// send the new certificate
	return socket.Send(temp, nLen, CSmallSocket::WAITFORWOULDBLOCK, 2000) == nLen;
	}

//--------------------------------------------------------------------------------
bool CClientHandlerThread::DoRefreshCertificate(const CCertificate& cert, CSmallSocket& socket)
	{
	UINT nInPort;
	CString sAddr;

	if(! socket.GetPeerName(sAddr, nInPort))
		{
		GetIO()->Output(IOMASK_ERR, "Error obtaining client IP address");
		return false;
		}

	SetThreadPriority(THREAD_PRIORITY_TIME_CRITICAL);

	CCertificateMaster* pCert = NULL;
	// only refresh when we are serving
	if(GetSystem()->IsServing())
		pCert = GetDB()->RefreshCertificate(cert, sAddr);

	char temp[4096];
	int nLen = 0;

	if(pCert == NULL)
		{
		CCertificateMaster cert;
		cert.SetCommand(CMD_REFRESH, ERR_NO_SUCH_CERT);
		nLen = cert.Encode(temp, false);
		}
	else
		nLen = pCert->Encode(temp, false);

	// send the new certificate
	bool bRv = socket.Send(temp, nLen, CSmallSocket::WAITFORWOULDBLOCK, 2000) == nLen;

	SetThreadPriority(THREAD_PRIORITY_HIGHEST);
	
	return bRv;
	}

//--------------------------------------------------------------------------------
bool CClientHandlerThread::DoReleaseCertificate(const CCertificate& cert, CSmallSocket& socket)
	{
	int nTokenId = cert.GetCommandParam();
	if(nTokenId == 0)
		{
		UINT nInPort;
		CString sAddr;

		if(! socket.GetPeerName(sAddr, nInPort))
			{
			GetIO()->Output(IOMASK_ERR, "Error obtaining client IP address");
			return false;
			}

			GetDB()->ReleaseCertificate(cert, sAddr);
		}
	else
		GetDB()->ReleaseToken(cert, nTokenId);

	return true;
	}

//--------------------------------------------------------------------------------
bool CClientHandlerThread::OnActivate()
	{
	return true;
	}

//--------------------------------------------------------------------------------
bool CClientHandlerThread::OnDeactivate()
	{
	CThreadObject::OnDeactivate();
	return true;
	}
