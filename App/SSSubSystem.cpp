//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "SSSubSystem.h"
#include "SecurityServer.h"

//--------------------------------------------------------------------------------
CSSSubSystem::CSSSubSystem(CSecuritySystem* pParent)
		: CSubSystem(pParent)
		, m_pIO(NULL)
		, m_pDB(NULL)
		, m_pClient(NULL)
	{
	}

//--------------------------------------------------------------------------------
CSSIOSubSystem* CSSSubSystem::GetIO()
	{
	if(m_pIO == NULL)
		m_pIO = ((CSecuritySystem*) GetSystem())->GetIOSubSystem();
	return m_pIO;
	}

//--------------------------------------------------------------------------------
CDBSubSystem* CSSSubSystem::GetDB()
			{
			if(m_pDB == NULL)
				m_pDB = ((CSecuritySystem*) GetSystem())->GetDBSubSystem();
			return m_pDB;
			}

//--------------------------------------------------------------------------------
CClientSubSystem* CSSSubSystem::GetClientSubSystem()
			{
			if(m_pClient == NULL)
				m_pClient = ((CSecuritySystem*) GetSystem())->GetClientSubSystem();
			return m_pClient;
			}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
CSSLBSubSystem::CSSLBSubSystem(CSecuritySystem* pParent)
		: CLoadBalancedSubSystem(pParent)
		, m_pIO(NULL)
		, m_pDB(NULL)
		, m_pClient(NULL)
	{
	}

//--------------------------------------------------------------------------------
CSSIOSubSystem* CSSLBSubSystem::GetIO()
	{
	if(m_pIO == NULL)
		m_pIO = ((CSecuritySystem*) GetSystem())->GetIOSubSystem();
	return m_pIO;
	}

//--------------------------------------------------------------------------------
CDBSubSystem* CSSLBSubSystem::GetDB()
	{
	if(m_pDB == NULL)
		m_pDB = ((CSecuritySystem*) GetSystem())->GetDBSubSystem();
	return m_pDB;
	}

//--------------------------------------------------------------------------------
CClientSubSystem* CSSLBSubSystem::GetClientSubSystem()
	{
	if(m_pClient == NULL)
		m_pClient = ((CSecuritySystem*) GetSystem())->GetClientSubSystem();
	return m_pClient;
	}
