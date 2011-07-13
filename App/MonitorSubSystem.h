//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _MONITORSUBSYSTEM_H_
#define _MONITORSUBSYSTEM_H_

//--------------------------------------------------------------------------------
#include "SSSubSystem.h"

//--------------------------------------------------------------------------------
class CSecuritySystem;
class CSystemMonitorThread;

//--------------------------------------------------------------------------------
class CMonitorSubSystem : public CSSSubSystem<CThreadPoolSubSystem>
	{
	public:
		DECLARE_DYNAMIC(CMonitorSubSystem);

		enum
			{
			MSG_NEWCONNECTION = WM_USER
			};

	private:
		CSystemMonitorThread* m_pHandler;

	public:
		CMonitorSubSystem(CSecuritySystem*);
		virtual ~CMonitorSubSystem();
	};

#endif // _MONITORSUBSYSTEM_H_
