//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "MCEncrypt.h"
/*

#ifdef	UNIX

#define CONST  const

typedef unsigned char  BYTE;
typedef char CHAR;
typedef CHAR *LPSTR;
typedef LPSTR LPTSTR;
typedef CONST CHAR *LPCSTR;
typedef LPCSTR LPCTSTR;

#endif // UNIX

#ifndef _MCCRYPT_H_
#define _MCCRYPT_H_

#include <stdio.h>
#include <string.h>

//--------------------------------------------------------------------------------
class CMCCrypt  
	{
	protected:
		BYTE* m_pData;
		BYTE* m_pKey;
		int m_nLen;
		static const BYTE** m_pEnTables;
		static const BYTE** m_pDeTables;

	protected:
		bool Crypt(const BYTE**, const BYTE*, int, const BYTE*);

		CMCCrypt();
		virtual ~CMCCrypt();

		bool Encrypt(const BYTE*, int, const BYTE* = NULL);
		bool Decrypt(const BYTE*, int, const BYTE* = NULL);

	public:
		static void SetTumblerTables(const BYTE** pEnTables, const BYTE** pDeTables);

		int GetLength()			{ return m_nLen; }
		BYTE* GetData()			{ return m_pData; }
	};

//--------------------------------------------------------------------------------
class CMCEncrypt : public CMCCrypt
	{
	public:
		CMCEncrypt(const BYTE*, int, const BYTE* = NULL);
	};

//--------------------------------------------------------------------------------
class CMCDecrypt : public CMCCrypt
	{
	public:
		CMCDecrypt(const BYTE*, int, const BYTE* = NULL);
	};

#endif // _MCCRYPT_H_
*/