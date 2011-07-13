//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _WRITELOCK_H_
#define _WRITELOCK_H_

//--------------------------------------------------------------------------------
#ifndef __AFXMT_H__
#include <afxmt.h>
#endif

//--------------------------------------------------------------------------------
#include "ReadWriteObject.h"

//--------------------------------------------------------------------------------
class CReadWriteObject;
class CReadLock;

//--------------------------------------------------------------------------------
class CWriteLock : public CReadWriteLock
	{
	// Constructor
	public:
		CWriteLock(CReadWriteObject*, bool = true);
		~CWriteLock();

	// Attributes
	public:
	// Operations
		virtual bool Lock(DWORD dwTimeout = INFINITE);
		virtual bool Unlock();
	};

#endif // _WRITELOCK_H_
