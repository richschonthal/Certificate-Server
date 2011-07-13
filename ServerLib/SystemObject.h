//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#if !defined(AFX_SYSTEMOBJECT_H__9F10BA26_835B_11D3_B0D3_00A0CC271D0D__INCLUDED_)
#define AFX_SYSTEMOBJECT_H__9F10BA26_835B_11D3_B0D3_00A0CC271D0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
#include "result.h"
#include "StateObject.h"
#include "LockObject.h"

#include <LockedList.h>

//--------------------------------------------------------------------------------
enum
	{
	SO_DEFAULT_PULSE_INTERVAL = 2000,
	// error codes
	SO_NULL_PARENT = -1
	};

//--------------------------------------------------------------------------------
class CSystemConfig;
class CSystemControlObject;

//--------------------------------------------------------------------------------
class CSystemObject : public CObject, public CResult, public CLockObject
	{
	friend class CThreadObject;
	friend class CSubSystemBase;

	DECLARE_DYNAMIC(CSystemObject);

	public:
		// general purpose object lock
		CMutex m_mutex;
		// general purpose signal
		CEvent m_event;
		// signaled when this system object
		// throws an exception
		CEvent m_evtException;
		// time of last pulse used to determine if the thread is alive
		CTime m_ctLastPulse;

	public:
		CSystemObject(CSystemObject* pParent);
		virtual ~CSystemObject();

	// attributes
	protected:
		// the parent of this object or null if this is a root object
		CSystemObject* m_pParent;
		// the state - a non systemcontrol object will be given a local state
		// by a systemcontrol object
		CStateObject* m_pState;

	public:
		// returns the parent of this object or null if this is a root object
		CSystemObject* GetParent();

		// returns the configuration of the parent
		// or null if no parent
		virtual CSystemConfig* GetConfiguration();

		// state
		//--------------------------------------------------------------------------------
		// returns the state of the controlling parent
		virtual DWORD GetState();

		virtual CStateObject* GetStatePtr();

		virtual void AddStateClient();
		virtual void RemoveStateClient();
		virtual void AcknowledgeStateChange();

		// pulse
		//--------------------------------------------------------------------------------
		virtual DWORD GetPulseInterval();
		DWORD GetTicksSincePulse();

		// messaging
		//--------------------------------------------------------------------------------
		virtual bool PostMessage(UINT, WPARAM = 0, LPARAM = 0);

	protected:
		// updates lastpulse time
		virtual void Pulse();
	};

//--------------------------------------------------------------------------------
class CSystemObjectList : public CLockedList<CSystemObject*, CSystemObject*>
	{
	public:
		CSystemObjectList(int nBlock = 10);
		virtual ~CSystemObjectList();

		POSITION Find(CSystemObject*, POSITION = NULL) const;
	};

#endif // !defined(AFX_SYSTEMOBJECT_H__9F10BA26_835B_11D3_B0D3_00A0CC271D0D__INCLUDED_)
