//--------------------------------------------------------------------------------
// Copyright (c) 2001 MarkCare Medical Solutions, Inc.
// Created...: 6/8/01
// Author....: Rich Schonthal
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "ChangeNotificationThread.h"
#include "ChangeNotificationSubSystem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//--------------------------------------------------------------------------------
typedef BOOL (__stdcall *ReadDirectoryChangesWType)(
    IN HANDLE hDirectory,
    IN OUT LPVOID lpBuffer,
    IN DWORD nBufferLength,
    IN BOOL bWatchSubtree,
    IN DWORD dwNotifyFilter,
    OUT LPDWORD lpBytesReturned,
    IN LPOVERLAPPED lpOverlapped,
    IN LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
    );

static ReadDirectoryChangesWType pReadDirectoryChangesW = NULL;

//--------------------------------------------------------------------------------
CChangeNotificationThread::CChangeNotificationThread(CChangeNotificationSubSystem* pParent, int nPathIndex,
													 bool bWatchSubDir, DWORD nFilter)
		: CThreadObject((CSystemObject*) pParent)
		, m_nPathIndex(nPathIndex)
		, m_bWatchSubDir(bWatchSubDir)
		, m_nFilter(nFilter)
		, m_event(FALSE, TRUE)
		, m_hCompletionPort(INVALID_HANDLE_VALUE)
		, m_nCurBufIndex(0)
	{
	if(pReadDirectoryChangesW == NULL)
		pReadDirectoryChangesW = (ReadDirectoryChangesWType)
				GetProcAddress(GetModuleHandle("kernel32.dll"), "ReadDirectoryChangesW");

	m_pSubSystem = pParent;

	LPCTSTR pPath = pParent->GetPath(nPathIndex);

	CDirectory::CreatePath(pPath);

	m_hDir = ::CreateFile(pPath,
		  FILE_LIST_DIRECTORY,                // access (read-write) mode
		  FILE_SHARE_READ|FILE_SHARE_WRITE,//|FILE_SHARE_DELETE,  // share mode
		  NULL,                               // security descriptor
		  OPEN_EXISTING,                      // how to create
		  FILE_FLAG_BACKUP_SEMANTICS|FILE_FLAG_OVERLAPPED ,         // file attributes
		  NULL                                // file with attributes to copy
		  );

	if(! m_hDir.IsValid())
		return;

	memset(&m_overlapped, 0, sizeof(m_overlapped));
	m_overlapped.hEvent = m_event;
	m_event.ResetEvent();

	m_hCompletionPort = ::CreateIoCompletionPort(m_hDir, NULL, (DWORD) m_pBuffer, 0);

	DWORD nSize;
	nSize = GetLastError();
	(pReadDirectoryChangesW)(m_hDir, m_pBuffer, BUFFER_SIZE, m_bWatchSubDir, m_nFilter, &nSize, &m_overlapped, NULL);
	nSize = GetLastError();
	}

//--------------------------------------------------------------------------------
CChangeNotificationThread::~CChangeNotificationThread()
	{
	}

//--------------------------------------------------------------------------------
DWORD CChangeNotificationThread::GetThreadLoopSleepInterval() const
	{
	return 100;
	}

//--------------------------------------------------------------------------------
bool CChangeNotificationThread::MainLoop()
	{
	if(! m_hCompletionPort.IsValid())
		return true;

	if(::WaitForSingleObject(m_event, 0) == WAIT_OBJECT_0)
		{
		DWORD nSize;
		UCHAR* pBase;
		OVERLAPPED* pOver;

		if(! ::GetQueuedCompletionStatus(m_hCompletionPort, &nSize, (DWORD*) &pBase, &pOver, 100))
			return true;

		DWORD nOffset = 0;

		for(;;)
			{
			FILE_NOTIFY_INFORMATION* pInfo = (FILE_NOTIFY_INFORMATION*) (pBase + nOffset);
			nOffset += pInfo->NextEntryOffset;
			m_pSubSystem->AddToQueue((FILE_NOTIFY_INFORMATION*) pInfo, (int) m_nPathIndex);
			if(pInfo->NextEntryOffset == 0)
				break;
			}

		m_event.ResetEvent();
		(pReadDirectoryChangesW)(m_hDir, m_pBuffer, BUFFER_BLOCK_SIZE, m_bWatchSubDir, m_nFilter, &nSize, &m_overlapped, NULL);
		}

	return true;
	}

//--------------------------------------------------------------------------------
int CChangeNotificationThread::GetPathIndex() const
	{
	return m_nPathIndex;
	}

