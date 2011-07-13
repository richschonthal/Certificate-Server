//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "SSThread.h"
#include "SecurityServer.h"
#include <ThreadObject.h>

//--------------------------------------------------------------------------------
CSSThread::CSSThread(CSystemObject* pParent)
		: CThreadObject(pParent)
		, m_pIO(NULL)
		, m_pDB(NULL)
		, m_pClient(NULL)
	{
	}

//--------------------------------------------------------------------------------
CSSIOSubSystem* CSSThread::GetIO()
	{
	if(m_pIO == NULL)
		m_pIO = ((CSecuritySystem*) GetSystem())->GetIOSubSystem();
	return m_pIO;
	}

//--------------------------------------------------------------------------------
CDBSubSystem* CSSThread::GetDB()
	{
	if(m_pDB == NULL)
		m_pDB = ((CSecuritySystem*) GetSystem())->GetDBSubSystem();
	return m_pDB;
	}

//--------------------------------------------------------------------------------
CClientSubSystem* CSSThread::GetClientSubSystem()
	{
	if(m_pClient == NULL)
		m_pClient = ((CSecuritySystem*) GetSystem())->GetClientSubSystem();
	return m_pClient;
	}

//--------------------------------------------------------------------------------
CSecuritySystem* CSSThread::GetSystem()
	{
	return (CSecuritySystem*) CThreadObject::GetSystem();
	}

//--------------------------------------------------------------------------------
void CSSThread::Exit(bool)
	{
	}

