//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#if !defined(AFX_SYSTEMCONTROLOBJECT_H__E88D4A3F_869D_11D3_B0D4_00A0CC271D0D__INCLUDED_)
#define AFX_SYSTEMCONTROLOBJECT_H__E88D4A3F_869D_11D3_B0D4_00A0CC271D0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
#include "SystemObject.h"

//--------------------------------------------------------------------------------
class CSystemControlObject : public CSystemObject
	{
	DECLARE_DYNAMIC(CSystemControlObject)

	public:
		CSystemControlObject(CSystemObject* pParent);
		virtual ~CSystemControlObject();

	protected:
		// the configuration for this object or null if not
		CSystemConfig* m_pConfig;

	public:
		// state
		//--------------------------------------------------------------------------------
		// set the state of this object
		virtual bool SetState(DWORD);
		// waits until all state clients have signaled
		virtual bool WaitForStateSync(DWORD = INFINITE);
		// called when all state clients reach sync
		virtual void OnStateSync();

		//--------------------------------------------------------------------------------
		// configuration
		//--------------------------------------------------------------------------------
		// returns the configuration of the parent
		// if the parent is null then the object's configuration is returned
		virtual CSystemConfig* GetConfiguration();
		// returns the configuration of this object or null if there's no configuration
		virtual CSystemConfig* GetObjectConfiguration();
		// sets the configuration pointer
		// returns the previous configuration (or null on the 1st call)
		CSystemConfig* SetObjectConfiguration(CSystemConfig*);
		// deallocates configuration, GetConfiguration will now return parents configuration
		virtual void DeleteConfiguration();
	};

#endif // !defined(AFX_SYSTEMCONTROLOBJECT_H__E88D4A3F_869D_11D3_B0D4_00A0CC271D0D__INCLUDED_)
