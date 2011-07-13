//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _READLOCK_H_
#define _READLOCK_H_

//--------------------------------------------------------------------------------
#ifndef __AFXMT_H__
#include <afxmt.h>
#endif

//--------------------------------------------------------------------------------
#include "ReadWriteObject.h"
class CWriteLock;

//--------------------------------------------------------------------------------
class CReadLock : public CReadWriteLock
	{
	// Constructor
	public:
		CReadLock(CReadWriteObject*, bool = true);
		~CReadLock();

	// Operations
		virtual bool Lock(DWORD dwTimeout = INFINITE);
		virtual bool Unlock();
		bool Lock(CWriteLock*);
	};

#endif // _READLOCK_H_
