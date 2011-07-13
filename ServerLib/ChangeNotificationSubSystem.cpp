//--------------------------------------------------------------------------------
// Copyright (c) 2001 MarkCare Medical Solutions, Inc.
// Created...: 6/9/01
// Author....: Rich Schonthal
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#include "stdafx.h"
#include "ChangeNotificationSubSystem.h"
#include "ChangeNotificationThread.h"
#include "ChangeNotificationDirThread.h"

#include <Directory.h>

//--------------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CChangeNotificationSubSystem, CSubSystem)

//--------------------------------------------------------------------------------
void AFXAPI DestructElements<CFileNotifyInformation*>(CFileNotifyInformation** pElements, int nCount)
	{
	for(int i = 0; i < nCount; i++)
		delete *pElements;
	}

//--------------------------------------------------------------------------------
CChangeNotificationSubSystem::CChangeNotificationSubSystem(CSystem* pParent)
		: CThreadPoolSubSystem(pParent)
		, m_nQueueSize(0)
	{
	}

//--------------------------------------------------------------------------------
CChangeNotificationSubSystem::~CChangeNotificationSubSystem()
	{
	}

//--------------------------------------------------------------------------------
bool CChangeNotificationSubSystem::GetNextInfo(CFileNotifyInformation*& pInfo, LPCTSTR& pPath)
	{
	CSingleLock lock(&m_mutex, false);
	if(! lock.Lock(1000))
		return false;

	CFileNotifyInformationContainer* pHead = m_queue.GetHead();
	m_queue.RemoveHead();
	pInfo = pHead->m_pInfo;
	pPath = GetPath(pHead->m_nPathIndex);
	delete pHead;
	::InterlockedDecrement(&m_nQueueSize);
	return true;
	}

//--------------------------------------------------------------------------------
int CChangeNotificationSubSystem::GetNotifyCount()
	{
	return (int) m_nQueueSize;
	}

//--------------------------------------------------------------------------------
bool CChangeNotificationSubSystem::AddToQueue(const FILE_NOTIFY_INFORMATION* pOrigInfo, int nIndex)
	{
	CSingleLock lock(&m_mutex, false);
	if(! lock.Lock(1000))
		return false;

	CFileNotifyInformationContainer* pNew = new CFileNotifyInformationContainer;
	if(pNew == NULL)
		return false;
	pNew->m_pInfo = new CFileNotifyInformation(pOrigInfo);
	if(pNew->m_pInfo == NULL)
		{
		delete pNew;
		return false;
		}
	pNew->m_nPathIndex = nIndex;
	m_queue.AddTail(pNew);
	::InterlockedIncrement(&m_nQueueSize);
	return true;
	}

//--------------------------------------------------------------------------------
bool CChangeNotificationSubSystem::AddWatchPath(LPCTSTR pPath, bool bWatchSubDir, DWORD nFilter)
	{
	CSingleLock lock(&m_mutPaths, false);
	if(! lock.Lock())
		return false;

	if(! PreAddWatchPath(pPath))
		return false;

	int nIndex = m_sWatchedPaths.Add(pPath);

	AddThread((CThreadObject*) new CChangeNotificationThread(this, nIndex, bWatchSubDir, nFilter));
	return true;
	}

//--------------------------------------------------------------------------------
LPCTSTR CChangeNotificationSubSystem::GetPath(int nIndex)
	{
	CSingleLock lock(&m_mutPaths, false);
	if(! lock.Lock())
		return NULL;

	if(nIndex >= m_sWatchedPaths.GetSize())
		return NULL;

	return m_sWatchedPaths.GetAt(nIndex);
	}
