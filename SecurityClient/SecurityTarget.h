//--------------------------------------------------------------------------------
//
// Copyright (c) 2001 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _SECURITY_TARGET_H_
#define _SECURITY_TARGET_H_

//--------------------------------------------------------------------------------
class CThreadObject;

//--------------------------------------------------------------------------------
class CSecurityTarget
	{
	private:
		enum
			{
			TargetNone,
			// ServerLib target
			TargetThreadObj,
			// MFC targets
			TargetThread, TargetCWnd,
			// non-MFC targets
			TargetHWND, TargetThreadId
			} m_nTargetType;

		union
			{ 
			CWinThread* pThrd;
			CWnd* pWnd;
			CThreadObject* pThrdObj;
			HWND hWnd;
			DWORD nThreadId;
			} m_target;

		UINT m_nMsg;

	protected:
		void DoPostMessage(WPARAM = 0, LPARAM = 0);

	public:
		enum { nMustTerminate = 1 };

		void SetTarget(CWinThread*, UINT);
		void SetTarget(CWnd*, UINT);
		void SetTarget(CThreadObject*, UINT);
		void SetTarget(HWND, UINT);
		void SetTarget(DWORD, UINT);

		void SetTargetNone();
	};

#endif // _SECURITY_TARGET_H_
