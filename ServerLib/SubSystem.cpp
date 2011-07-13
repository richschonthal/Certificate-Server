//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
#include "stdafx.h"
#include "SubSystem.h"
#include "ThreadObject.h"
#include "System.h"

//--------------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CSubSystem, CSystemControlObject)

//--------------------------------------------------------------------------------
CSubSystem::CSubSystem(CSystem* pSys)
		: CSystemControlObject(pSys)
	{
	m_pState = pSys->m_pState;
	}

//--------------------------------------------------------------------------------
CSubSystem::~CSubSystem()
	{
	m_pState = NULL;
	}

//--------------------------------------------------------------------------------
CSystem* CSubSystem::GetSystem()
	{
	return (CSystem*) GetParent();
	}

//--------------------------------------------------------------------------------
bool CSubSystem::Init()
	{
	return true;
	}
