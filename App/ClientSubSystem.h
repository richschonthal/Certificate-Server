//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_CLIENTSUBSYSTEM_H__3FFF6999_C933_11D3_AF10_005004A1C5F3__INCLUDED_)
#define AFX_CLIENTSUBSYSTEM_H__3FFF6999_C933_11D3_AF10_005004A1C5F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
#include "SSSubSystem.h"

//--------------------------------------------------------------------------------
class CSecuritySystem;
class CSSConfigClientHandler;
class CClientConnectThread;
class CSSIOSubSystem;
class CCertificateMaster;
class CMagicNumber;
class CConnection;
class CConnectionPool;

//--------------------------------------------------------------------------------
class CClientSubSystem : public CSSSubSystem<CThreadPoolSubSystem>
	{
	public:
		DECLARE_DYNAMIC(CClientSubSystem);

	protected:
		CSSConfigClientHandler* m_pConfig;
		CClientConnectThread* m_pConnectionThread;

	public:
		enum
			{
			MSG_NEWCONNECTION = WM_USER
			};

	public:
		CClientSubSystem(CSecuritySystem*);
		virtual ~CClientSubSystem();

		CSSIOSubSystem* GetIO();

		CMagicNumber* GetMagNumGen() const;
	};

#endif // !defined(AFX_CLIENTSUBSYSTEM_H__3FFF6999_C933_11D3_AF10_005004A1C5F3__INCLUDED_)
