//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_SSIOTHREAD_H__C10D0D11_CF6E_11D3_AF12_005004A1C5F3__INCLUDED_)
#define AFX_SSIOTHREAD_H__C10D0D11_CF6E_11D3_AF12_005004A1C5F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
#include "SSThread.h"

class CSSIOSubSystem;

//--------------------------------------------------------------------------------
class CSSIOThread : public CSSThread  
	{
	public:
		CSSIOThread(CSSIOSubSystem*);
		virtual ~CSSIOThread();

		CSSIOSubSystem* GetSubSystem()	{ return (CSSIOSubSystem*) CThreadObject::GetSubSystem(); }

		virtual void OnMessage(UINT, WPARAM, LPARAM);
		void Output(LPCTSTR);

		virtual bool Init();
		virtual void Exit(bool);
	};

#endif // !defined(AFX_SSIOTHREAD_H__C10D0D11_CF6E_11D3_AF12_005004A1C5F3__INCLUDED_)
