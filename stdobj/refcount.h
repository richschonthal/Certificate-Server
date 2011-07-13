//********************************************************************************
// Ref count class
// Class for implementing reference counted classes
//
// designed and coded by Rich Schonthal
// Copyright (c) 1997 Rich Schonthal
//
// this code is made public domain by the author
// it may be used in products commercial or otherwise
// as long as all copyright notices remain in tact
//********************************************************************************

#ifndef _REFCOUNT_H_
#define _REFCOUNT_H_

class CRefCount
   {
   private:
      BYTE m_nRefCount;

   public:
      CRefCount()       { m_nRefCount = 0; }
#ifdef _DEBUG
      ~CRefCount()      { _ASSERT(m_nRefCount == 0); } 
#endif

      void IncRefCount()  { m_nRefCount++; }
      void DecRefCount()  { m_nRefCount--; }
   };

#endif // _REFCOUNT_H_