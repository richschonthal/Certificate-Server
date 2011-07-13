//--------------------------------------------------------------------------------
// Copyright (c) 2001 MarkCare Medical Solutions, Inc.
// Created...: 6/11/01
// Author....: Rich Schonthal
//--------------------------------------------------------------------------------

#if !defined(AFX_CHANGENOTIFICATIONSUBSYSTEM_H__B66F74A9_E5CF_11D3_B10F_00A0CC271D0D__INCLUDED_)
#define AFX_CHANGENOTIFICATIONSUBSYSTEM_H__B66F74A9_E5CF_11D3_B10F_00A0CC271D0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
#include "SubSystem.h"
#include "System.h"
#include "ChangeNotificationThread.h"

#include <Directory.h>
#include <FileNotifyInformation.h>

#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
#endif

//--------------------------------------------------------------------------------
class CChangeNotificationSubSystem : public CThreadPoolSubSystem  
	{
	// needs to access AddToQueue and MsgAddToQueue
	friend class CChangeNotificationThread;

	public:
		DECLARE_DYNAMIC(CChangeNotificationSubSystem);

	protected:
		enum
			{
			MsgChangeNotify = CSystem::MSG_DERIVED_START
			};

		class CFileNotifyInformationContainer
			{
			public:
				CFileNotifyInformation* m_pInfo;
				// the index to the root path in m_sWatchedPaths 
				int m_nPathIndex;
			};

		CTypedPtrList<CPtrList, CFileNotifyInformationContainer*> m_queue;
		CMutex m_mutex;

		// when a watch thread is added, the path it's watching is kept here
		// these paths are never removed even if the thread exits
		// and no checks are made to determine if the path is already in the array
		// so it's not a good idea to start and stop a lot of watch threads
		CStringArray m_sWatchedPaths;
		CMutex m_mutPaths;

		LONG m_nQueueSize;

	public:
		CChangeNotificationSubSystem(CSystem*);
		virtual ~CChangeNotificationSubSystem();

		virtual bool AddWatchPath(LPCTSTR pPath, bool bWatchSubDir = true, DWORD nFilter =
			FILE_NOTIFY_CHANGE_FILE_NAME|FILE_NOTIFY_CHANGE_DIR_NAME|FILE_NOTIFY_CHANGE_ATTRIBUTES|
			FILE_NOTIFY_CHANGE_SIZE);

		virtual bool GetNextInfo(CFileNotifyInformation*&, LPCTSTR& pPath);

		int GetNotifyCount();

		LPCTSTR GetPath(int);

		virtual bool PreAddWatchPath(LPCTSTR pPath) { return true;}

	protected:
		virtual bool AddToQueue(const FILE_NOTIFY_INFORMATION*, int nIndex);

		// cant do this with this sub system since each thread is watching a different directory
		virtual bool PostNextThreadMessage(UINT, WPARAM = 0, LPARAM = 0);
	};

#endif // !defined(AFX_CHANGENOTIFICATIONSUBSYSTEM_H__B66F74A9_E5CF_11D3_B10F_00A0CC271D0D__INCLUDED_)
