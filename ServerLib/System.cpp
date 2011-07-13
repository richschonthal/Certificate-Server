//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#include "stdafx.h"
#include "System.h"
#include "SubSystem.h"

//--------------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CSystem, CSystemControlObject)

//--------------------------------------------------------------------------------
CSystem::CSystem(CSystemControlObject* pParent)
		: CSystemControlObject(pParent)
	{
	m_pState = new CSystemState;
	}

//--------------------------------------------------------------------------------
CSystem::~CSystem()
	{
	}

#ifdef _DEBUG
//--------------------------------------------------------------------------------
bool CSystem::PostMessage(UINT nMsg, WPARAM wParam, LPARAM lParam)
	{
	POSITION pos = m_subSystemList.GetHeadPosition();
	bool bRv = pos != NULL;
	for(; pos != NULL; )
		{
		CSubSystem* pSub = static_cast<CSubSystem*>(m_subSystemList.GetNext(pos));
		if(! pSub->PostMessage(nMsg, wParam, lParam))
			bRv = false;
		}

	return bRv;
	}
#else
//--------------------------------------------------------------------------------
bool CSystem::PostMessage(UINT nMsg, WPARAM wParam, LPARAM lParam)
	{
	for(POSITION pos = m_subSystemList.GetHeadPosition(); pos != NULL; )
		{
		if(! m_subSystemList.GetNext(pos)->PostMessage(nMsg, wParam, lParam))
			return false;
		}

	return true;
	}
#endif

//--------------------------------------------------------------------------------
bool CSystem::AddSubSystem(CSubSystem* pSub)
	{
	if(! pSub->Init())
		return false;

	return m_subSystemList.AddTail(pSub) != NULL;
	}

