//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// LoadString.cpp: implementation of the CLoadString class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LoadString.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------
CLoadString::CLoadString()
	{

	}

//--------------------------------------------------------------------------------
CLoadString::CLoadString(int nId, CString& sDest)
	{
	sDest = Get(nId);
	}

//--------------------------------------------------------------------------------
CLoadString::~CLoadString()
	{

	}

//--------------------------------------------------------------------------------
CString CLoadString::Get(int nId)
	{
	CString sTemp;
	TRY
		{
		sTemp.LoadString(nId);
		}
	CATCH_ALL(e)
		{
		}
	END_CATCH_ALL

	return sTemp;
	}
