//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#if !defined(AFX_SSIOSUBSYSTEM_H__CC79BAB6_CE84_11D3_AF12_005004A1C5F3__INCLUDED_)
#define AFX_SSIOSUBSYSTEM_H__CC79BAB6_CE84_11D3_AF12_005004A1C5F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
#include <IOSubSystem.h>
#include "SSSubSystem.h"

class CSecuritySystem;

//--------------------------------------------------------------------------------
class CSSIOSubSystem : public CSSSubSystem<CIOSubSystem>
	{
	public:
		DECLARE_DYNAMIC(CSSIOSubSystem);
/*
		enum
			{
			SIOMASK_ IOMASK_1 =		0x00000001,
			IOMASK_2 =		0x00000002,
			IOMASK_3 =		0x00000004,
			IOMASK_4 =		0x00000008,
			IOMASK_5 =		0x00000010,
			IOMASK_6 =		0x00000020,
			IOMASK_7 =		0x00000040,
			IOMASK_8 =		0x00000080,

			};
*/
	public:
		CSSIOSubSystem(CSecuritySystem*);
		virtual ~CSSIOSubSystem();

	private:
		CTime m_ctLastLogCreate;
		CString m_sLastLogPath;

	protected:
		virtual void WriteLogFileHeader();
		virtual CThreadObject* CreateThread();

	public:
		virtual CStdioFile* GetLogFilePtr();
	};

#endif // !defined(AFX_SSIOSUBSYSTEM_H__CC79BAB6_CE84_11D3_AF12_005004A1C5F3__INCLUDED_)
