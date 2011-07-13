//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _ALLOCSTRING_H_
#define _ALLOCSTRING_H_

//--------------------------------------------------------------------------------
inline void AllocString(LPTSTR& pDest, LPCTSTR pSrc = NULL)
	{
	delete[] pDest;
	if(pSrc==NULL)
		{
		pDest=new TCHAR[sizeof(TCHAR)];
		*pDest=0;
		}
	else
		{
		pDest=new TCHAR[_tcslen(pSrc) + sizeof(TCHAR)];
		_tcscpy(pDest, pSrc);
		}
	}

#ifndef ALLOC_STRING
#define ALLOC_STRING AllocString
#endif

#endif // _ALLOCSTRING_H_
