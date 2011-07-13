//--------------------------------------------------------------------------------
// Copyright (c) 2001 MarkCare Medical Solutions, Inc.
// Created...: 6/8/01
// Author....: Rich Schonthal
//--------------------------------------------------------------------------------

#if !defined(AFX_CHANGENOTIFICATIONTHREAD_H__1A4AB94F_5C82_11D5_B191_00A0CC271D0D__INCLUDED_)
#define AFX_CHANGENOTIFICATIONTHREAD_H__1A4AB94F_5C82_11D5_B191_00A0CC271D0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
#include <Directory.h>
#include <Handle.h>

#include "ThreadObject.h"

//--------------------------------------------------------------------------------
class CChangeNotificationSubSystem;

//--------------------------------------------------------------------------------
class CChangeNotificationThread : public CThreadObject
	{
	private:
		CHandle m_hDir;
		int m_nPathIndex;
		bool m_bWatchSubDir;
		DWORD m_nFilter;

		// for the overlapped result
		CEvent m_event;
		OVERLAPPED m_overlapped;

		CHandle m_hCompletionPort;

		enum { BUFFER_SIZE = 524288, BUFFER_BLOCK_SIZE = 4096 };
		UCHAR m_pBuffer[BUFFER_SIZE];
		int m_nCurBufIndex;

		CChangeNotificationSubSystem* m_pSubSystem;

	public:
		CChangeNotificationThread(CChangeNotificationSubSystem*, int nPathIndex,
			bool bWatchSubDir = true, DWORD nFilter =
			FILE_NOTIFY_CHANGE_FILE_NAME|FILE_NOTIFY_CHANGE_DIR_NAME|FILE_NOTIFY_CHANGE_ATTRIBUTES|
			FILE_NOTIFY_CHANGE_SIZE);

		virtual ~CChangeNotificationThread();

		virtual bool MainLoop();
		virtual DWORD GetThreadLoopSleepInterval() const;

		int GetPathIndex() const;
	};

#endif // !defined(AFX_CHANGENOTIFICATIONTHREAD_H__1A4AB94F_5C82_11D5_B191_00A0CC271D0D__INCLUDED_)
