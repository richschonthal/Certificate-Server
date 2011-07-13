//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#include "stdafx.h"
#include "SystemConfig.h"
#include "StateObject.h"
#include "SystemControlObject.h"

//--------------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CSystemControlObject, CSystemObject)

//--------------------------------------------------------------------------------
CSystemControlObject::CSystemControlObject(CSystemObject* pParent)
		: CSystemObject(pParent)
		, m_pConfig(NULL)
	{
	}

//--------------------------------------------------------------------------------
CSystemControlObject::~CSystemControlObject()
	{
	}

//--------------------------------------------------------------------------------
CSystemConfig* CSystemControlObject::GetObjectConfiguration()
	{
	return m_pConfig;
	}

//--------------------------------------------------------------------------------
CSystemConfig* CSystemControlObject::GetConfiguration()
	{
	CSystemObject* pPar = GetParent();
	if(pPar == NULL)
		return m_pConfig;
	return pPar->GetConfiguration();
	}

//--------------------------------------------------------------------------------
CSystemConfig* CSystemControlObject::SetObjectConfiguration(CSystemConfig* pConfig)
	{
	CSystemConfig* pOld = m_pConfig;
	m_pConfig = pConfig;
	return pOld;
	}

//--------------------------------------------------------------------------------
void CSystemControlObject::DeleteConfiguration()
	{
	delete m_pConfig;
	m_pConfig = NULL;
	}

//--------------------------------------------------------------------------------
bool CSystemControlObject::SetState(DWORD nState)
	{
	if(m_pState == NULL)
		return false;

	return m_pState->SetValue(nState);
	}

//--------------------------------------------------------------------------------
bool CSystemControlObject::WaitForStateSync(DWORD nWait)
	{
	if(m_pState == NULL)
		return false;

	if(m_pState->m_nClients == 0)
		return true;

	return ::WaitForSingleObject(m_pState->m_evtClientSync, nWait) == WAIT_OBJECT_0;
	}

//--------------------------------------------------------------------------------
void CSystemControlObject::OnStateSync()
	{
	}
