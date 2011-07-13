//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _CLIENTHANDLERTHREAD_H_
#define _CLIENTHANDLERTHREAD_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
#include "SSThread.h"
#include <Certificate.h>
#include "SecuritySystem.h"

//--------------------------------------------------------------------------------
class CDBSubSystem;
class CSmallSocket;

//--------------------------------------------------------------------------------
class CClientHandlerThread : public CSSThread  
	{
	private:
		static LPCTSTR m_pMsgHello;
		static LPCTSTR m_pMsgServerDown;
		static int m_nMaxClients;
		static int m_nMaxIdleTime;

	public:
		CClientHandlerThread(CSystemObject*);
		virtual ~CClientHandlerThread();

	public:
		// take care of initial connection in OnMessage
		virtual void OnMessage(UINT, WPARAM, LPARAM);
		// maintain connections in MainLoop
		virtual bool Init();

	private:
		bool ReceiveCertificate(CCertificate&, CSmallSocket&);

		bool DoNewCertificate(const CCertificate&, CSmallSocket&);
		bool DoAquireToken(CCertificate&, CSmallSocket&);
		bool DoBackupServerConnect(const CCertificate&, CSmallSocket&);

		bool DoRefreshCertificate(const CCertificate&, CSmallSocket&);
		bool DoReleaseCertificate(const CCertificate&, CSmallSocket&);

	public:
		virtual bool OnActivate();
		virtual bool OnDeactivate();
	};

#endif // _CLIENTHANDLERTHREAD_H_
