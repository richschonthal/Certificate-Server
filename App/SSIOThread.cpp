//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "SecurityServer.h"
#include "SSIOThread.h"
#include "SSIOSubSystem.h"

#include "SSQuickMail.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//--------------------------------------------------------------------------------
CSSIOThread::CSSIOThread(CSSIOSubSystem* pParent)
		: CSSThread(pParent)
	{
	}

//--------------------------------------------------------------------------------
CSSIOThread::~CSSIOThread()
	{
	}

//--------------------------------------------------------------------------------
bool CSSIOThread::Init()
	{
	SetThreadPriority(THREAD_PRIORITY_BELOW_NORMAL);
	return true;
	}

//--------------------------------------------------------------------------------
void CSSIOThread::Exit(bool)
	{
	MSG msg;
	while(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
		switch(msg.message)
			{
			case CSSIOSubSystem::MSG_OUTPUT:
				delete[] (char*) msg.wParam;
				break;

			case CSSIOSubSystem::MSG_QUICKMAIL:
				{
				CQuickMail* pQm = reinterpret_cast<CSSQuickMail*>(msg.wParam);
				delete pQm;
				}
			}
		}
	}

//--------------------------------------------------------------------------------
void CSSIOThread::OnMessage(UINT nMsg, WPARAM param, LPARAM)
	{
	LPTSTR pText = (LPTSTR) param;

	switch(nMsg)
		{
		case CSSIOSubSystem::MSG_OUTPUT:
			Output(pText);
			delete[] pText;
			break;

		case CSSIOSubSystem::MSG_OUTPUT_CONST:
			Output(pText);
			break;

		case CSSIOSubSystem::MSG_QUICKMAIL:
			{
			CQuickMail* pQm = reinterpret_cast<CSSQuickMail*>(param);
			if(pQm != NULL)
				{
				if(! pQm->Send())
					{
					Output("Failed to send email\n");
					CString sMsg;
					pQm->GetErrorMessage(sMsg);
					Output(sMsg);
					Output("<===begin email==============");
					Output("\n" + pQm->GetFormattedMsg());
					Output("<===end email================");
					}

				delete pQm;
				}
			}
			break;
		}
	}

//--------------------------------------------------------------------------------
void CSSIOThread::Output(LPCTSTR pText)
	{
	CSingleLock* pLock = NULL;
	CStdioFile* pFile = NULL;

	CString sText;
	sText.Format("%s\t%s", (LPCTSTR) CTime::GetCurrentTime().Format("%c"), pText);

	if(GetSubSystem()->GetLogFilePtr() != NULL)
		{
		pLock = new CSingleLock(&GetSubSystem()->GetLogFileLock(), false);

		if(! pLock->Lock(3000))
			{
			delete pLock;
			pLock = NULL;
			}
		else
			pFile = GetSubSystem()->GetLogFilePtr();
		}

	if(pFile != NULL)
		{
		pFile->WriteString(sText);
		pFile->WriteString("\r\n");
		pFile->Flush();
		delete pLock;
		}

#ifndef _DEBUG
	if((GetSubSystem()->GetTraceLevel() & IOMASK_DEBUG) == IOMASK_DEBUG)
#endif
		{
		OutputDebugString(pText);
		OutputDebugString("\r\n");
		}

	CNTService* pService = GetSubSystem()->GetServicePtr();
	if(pService != NULL)
		pService->AddToMessageLog(sText);
	}
