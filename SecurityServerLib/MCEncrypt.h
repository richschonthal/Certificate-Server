//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------
#ifdef	UNIX

#define CONST  const

typedef unsigned char  BYTE;
typedef char CHAR;
typedef CHAR *LPSTR;
typedef LPSTR LPTSTR;
typedef CONST CHAR *LPCSTR;
typedef LPCSTR LPCTSTR;

#endif // UNIX

#if !defined(AFX_MINIENCRYPT_H__7951B9A4_74F9_11D3_AEE4_005004A1C5F3__INCLUDED_)
#define AFX_MINIENCRYPT_H__7951B9A4_74F9_11D3_AEE4_005004A1C5F3__INCLUDED_

#include <stdio.h>
#include <string.h>


//--------------------------------------------------------------------------------
class CMCEncrypt  
	{
	friend class CMCDecrypt;

	private:
		BYTE* m_pData;
		BYTE* m_pKey;
		int m_nLen;

	public:
		CMCEncrypt(const BYTE* = NULL, int = 0, const BYTE* = NULL);
		virtual ~CMCEncrypt();

		int GetLength()			{ return m_nLen; }
		BYTE* GetData()			{ return m_pData; }

		bool Encrypt(const BYTE*, int, const BYTE* = NULL);
	};

//--------------------------------------------------------------------------------
class CMCDecrypt  
	{
	private:
		BYTE* m_pData;
		BYTE* m_pKey;
		int m_nLen;

	public:
		CMCDecrypt(const BYTE* = NULL, int = 0, const BYTE* = NULL);
		CMCDecrypt(const CMCEncrypt&);
		virtual ~CMCDecrypt();

		int GetLength()			{ return m_nLen; }
		BYTE* GetData()			{ return m_pData; }

		bool Decrypt(const BYTE*, int, const BYTE* = NULL);
	};


#endif // !defined(AFX_MINIENCRYPT_H__7951B9A4_74F9_11D3_AEE4_005004A1C5F3__INCLUDED_)
