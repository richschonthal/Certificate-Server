//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#include "stdafx.h"
#include "SystemObject.h"
#include "SystemConfig.h"

#ifdef _DEBUG
#include "IOThread.h"
#endif

//--------------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CSystemObject, CObject);

//--------------------------------------------------------------------------------
CSystemObject::CSystemObject(CSystemObject* pParent)
		: CLockObject(&m_mutex)
		, m_pState(NULL)
	{
	m_pParent = pParent;
	m_ctLastPulse = CTime::GetCurrentTime();
	}

//--------------------------------------------------------------------------------
CSystemObject::~CSystemObject()
	{
	delete m_pState;
	}

//--------------------------------------------------------------------------------
bool CSystemObject::PostMessage(UINT, WPARAM, LPARAM)
	{
	return false;
	}

//--------------------------------------------------------------------------------
CSystemObject* CSystemObject::GetParent()
	{
	return m_pParent;
	}

//--------------------------------------------------------------------------------
DWORD CSystemObject::GetPulseInterval()
	{
	return SO_DEFAULT_PULSE_INTERVAL;
	}

//--------------------------------------------------------------------------------
void CSystemObject::Pulse()
	{
	m_ctLastPulse = CTime::GetCurrentTime();
	}

//--------------------------------------------------------------------------------
DWORD CSystemObject::GetState()
	{
	if(m_pState == NULL)
		return STATE_UNKNOWN;
	return m_pState->GetValue();
	}

//--------------------------------------------------------------------------------
CStateObject* CSystemObject::GetStatePtr()
	{
	return m_pState;
	}

//--------------------------------------------------------------------------------
CSystemConfig* CSystemObject::GetConfiguration()
	{
	CSystemObject* pPar = GetParent();
	if(pPar == NULL)
		return NULL;
	return pPar->GetConfiguration();
	}

//--------------------------------------------------------------------------------
void CSystemObject::AddStateClient()
	{
	if(m_pState != NULL)
		m_pState->AddClient();
	}

//--------------------------------------------------------------------------------
void CSystemObject::RemoveStateClient()
	{
	if(m_pState != NULL)
		m_pState->RemoveClient();
	}

//--------------------------------------------------------------------------------
void CSystemObject::AcknowledgeStateChange()
	{
	if(m_pState != NULL)
		m_pState->AcknowledgeChange();
	}
