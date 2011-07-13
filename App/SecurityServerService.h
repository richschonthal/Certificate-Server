//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_SECURITYSERVERSERVICE_H__F9BB4AF3_C90D_11D3_AF0E_005004A1C5F3__INCLUDED_)
#define AFX_SECURITYSERVERSERVICE_H__F9BB4AF3_C90D_11D3_AF0E_005004A1C5F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <NTService.h>

//--------------------------------------------------------------------------------
class CSecurityServerService : public CNTService  
	{
	public:
		CSecurityServerService();
		virtual ~CSecurityServerService();

		virtual LPCTSTR GetServiceName();
		virtual LPCTSTR GetDisplayName();

		virtual DWORD GetStopPendingTime();

		//BOOL ProcessCommandLine(LPCTSTR);
	};

#endif // !defined(AFX_SECURITYSERVERSERVICE_H__F9BB4AF3_C90D_11D3_AF0E_005004A1C5F3__INCLUDED_)
