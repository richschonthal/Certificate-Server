//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "dongle.h"

// if we're building a security client that does not support dongle backup...
#ifdef _SECCON_NODONGLE
//--------------------------------------------------------------------------------
bool CDongle::Connect()			{ return true; }
//--------------------------------------------------------------------------------
bool CDongle::Connect2(DWORD, LPTSTR)		{ return true; }
//--------------------------------------------------------------------------------
bool CDongle::GetDongleMemory(BYTE[128])	{ return false; }
//--------------------------------------------------------------------------------
bool CDongle::SetDongleMemory(BYTE[32])	{ return false; }

#else
#include "hlapi_c.h"

//#pragma comment(lib, "hlw32_mc.lib")

// i want all my useless do nothing code included
#pragma optimize("", off)

#ifdef _DEBUG
bool CDongle::m_bDoNothing = 
#ifdef _CHECK_DONGLE
							false;
#else
							true;
#endif

#endif

#undef _USA

CMutex CDongle::m_mutex;
CTime CDongle::m_ctLastAccess(CTime::GetCurrentTime());
CTime CDongle::m_ctLastConnectOk(CTime(1990, 1, 1, 0, 0, 0));

// the system is allowed to run if this value is zero
// if may be modified in other modules (it is a global)
DWORD CDongle::m_nRunOkFlag = 0;

Byte CDongle::m_pRefKey[9] = {'M' + 2, 'O' + 2, 'O' + 2, 'N' + 2, 'A' + 2, 'C' + 2, 'H' + 2, 'I' + 2, 0 + 2}; //Reference key
// maybe hackers will get confused and think its already been cracked!! : )
Byte CDongle::m_pVerKey[16] = {'c', 'r', 'a', 'c', 'k', 't', 0x00, 0x9A,0x7D,0x8E,0x75,0x4F,0xEF,0xC4,0x39, 0x00};	// verification of Refkey

DWORD CDongle::m_nUselessVariable = 0;

Word CDongle::m_nMagicNumber1 = 
#ifndef _USA
0x612c;
#else
0x6122;
#endif

Word CDongle::m_nMagicNumber2 = 
#ifndef _USA
0x2c61;
#else
0x2261;
#endif

DWORD CDongle::m_nMagicNumber3 = 
#ifndef _USA
0x6F1F2FcF;
#else
0x6E1E2E2E;
#endif

#define MAGIC1() m_nMagicNumber1
#define MAGIC2() ( ((m_nMagicNumber2&0xff00)>>8) + ((m_nMagicNumber2&0xff)<<8) )
#define MAGIC3() (Word) ( ((m_nMagicNumber3&0xf0000000) >> 16) + ((m_nMagicNumber3&0x00f00000)>>12) + ((m_nMagicNumber3&0x0000f000)>>8) + ((m_nMagicNumber3&0xf0)>>4))

//--------------------------------------------------------------------------------
void CDongle::SetLastAccessTime()
	{
	m_ctLastAccess = CTime::GetCurrentTime();
	}

//--------------------------------------------------------------------------------
bool CDongle::GetCanAccess()
	{
	return (m_ctLastAccess + CTimeSpan(0, 0, 0, 2) < CTime::GetCurrentTime()) != 0;
	}

//--------------------------------------------------------------------------------
bool CDongle::Connect()
	{
#ifdef _DEBUG
	if(m_bDoNothing)
		return true;
#endif

	char pRef[9];
	char pVer[16];

	memcpy(pRef, m_pRefKey, 9);
	memcpy(pVer, m_pVerKey, 16);

	char* pRealVer = NULL;

	pRealVer = &pVer[7];

	for(int i = 0; i < 9; i++)
		pRef[i] -= 2;

	CSingleLock lock(&m_mutex);
	// if the dongle was ok less than a minute ago then just return ok
	// theres no point in reading more than once a minute
	if(m_ctLastConnectOk + CTimeSpan(0, 0, 1, 0) > CTime::GetCurrentTime())
		return true;
	// dont access the dongle with 3 seconds of the last access
	// it may have been accessed by another thread so...
	while(! GetCanAccess())
		::Sleep(10);
	SetLastAccessTime();
	bool bOk = false;
	switch(HL_LOGIN(MAGIC1(), LOCAL_DEVICE, (Byte*) pRef, (Byte*) pRealVer))
		{
		case NETWORK_ERROR:
			TRACE("NE\n");
			break;
		case NO_DONGLE:
			TRACE("ND\n");
			HL_LOGOUT();
			return false;
		case VERSION_MISMATCH:
			TRACE("VM\n");
			break;
		case INVALID_ENV:
			TRACE("IE\n");
			break;
		case CANNOT_OPEN_DRIVER:
			TRACE("COD\n");
			break;
		case DYNALINK_FAILED:
			TRACE("DF\n");
			break;
		case (Word)TOO_MANY_USERS:
			TRACE("TMU\n");
			break;
		case INVALID_LIC:
			TRACE("IL\n");
			break;
		case NO_LICENSE:
			TRACE("NL\n");
			break;
		default:
			bOk = true;
			break;
      }

	HL_LOGOUT();

	if(! bOk)
		return false;

	m_ctLastConnectOk = CTime::GetCurrentTime();
	return true;
	}

//--------------------------------------------------------------------------------
bool CDongle::Connect2(DWORD nFakeParam1, LPTSTR pFakeParam2)
	{
#ifdef _DEBUG
	if(m_bDoNothing)
		return true;
#endif

	DWORD nFake = nFakeParam1 & 0xF001F001;

	char pRef[9];
	char pVer[16];

	memcpy(pRef, m_pRefKey, 9);
	memcpy(pVer, m_pVerKey, 16);

	char* pRealVer = NULL;

	pRealVer = &pVer[7];

	for(int i = 0; i < 9; i++)
		pRef[i] -= 2;

	CSingleLock lock(&m_mutex);
	// if the dongle was ok less than a minute ago then just return ok
	// theres no point in reading more than once a minute
	if(m_ctLastConnectOk + CTimeSpan(0, 0, 1, 0) > CTime::GetCurrentTime())
		return true;
	// dont access the dongle with 3 seconds of the last access
	// it may have been accessed by another thread so...
	while(! GetCanAccess())
		::Sleep(10);
	SetLastAccessTime();

	Word nResult = HL_LOGIN(MAGIC3(), LOCAL_DEVICE, (Byte*) pRef, (Byte*) pRealVer);

	if(nResult == NETWORK_ERROR || nResult == VERSION_MISMATCH || nResult == INVALID_ENV || nResult == CANNOT_OPEN_DRIVER)
		return false;

	if(nResult == NO_DONGLE)
		{
		TRACE("NDCOD\n");
		CTime ctNow(CTime::GetCurrentTime());
		if(ctNow.GetSecond() < 30)
			m_nUselessVariable = ctNow.GetMinute();
		else
			m_nUselessVariable += 1;
		return false;
		}

	if(nResult == DYNALINK_FAILED || nResult == TOO_MANY_USERS || nResult == INVALID_LIC || nResult == NO_LICENSE)
		{
		TRACE("DFTMUILNL\n");
		DWORD nCrap = ::GetTickCount();
		::Sleep(20);

		if(::GetTickCount() - nCrap > 30)
			m_nUselessVariable += 2;
		else
			m_nUselessVariable += 3;

		return false;
		}

	bool (CDongle::*p)() = CDongle::Connect;

	memcpy(&m_nUselessVariable, &p, sizeof(void*));

	HL_LOGOUT();
	m_ctLastConnectOk = CTime::GetCurrentTime();
	return true;
	}

//--------------------------------------------------------------------------------
bool CDongle::GetDongleMemory(BYTE pMem[128])
	{
#ifdef _DEBUG
	if(m_bDoNothing)
		return true;
#endif

	char pRef[9];
	char pVer[16];

	memcpy(pRef, m_pRefKey, 9);
	memcpy(pVer, m_pVerKey, 16);

	char* pRealVer = NULL;

	pRealVer = &pVer[7];

	for(int i = 0; i < 9; i++)
		pRef[i] -= 2;

	CSingleLock lock(&m_mutex);
	while(! GetCanAccess())
		::Sleep(10);
	SetLastAccessTime();
	bool bOk = false;
	switch(HL_LOGIN(MAGIC1(), LOCAL_DEVICE, (Byte*) pRef, (Byte*) pRealVer))
		{
		case NETWORK_ERROR:
			TRACE("NE\n");
			break;
		case NO_DONGLE:
			TRACE("ND\n");
			HL_LOGOUT();
			return false;
		case VERSION_MISMATCH:
			TRACE("VM\n");
			break;
		case INVALID_ENV:
			TRACE("IE\n");
			break;
		case CANNOT_OPEN_DRIVER:
			TRACE("COD\n");
			break;
		case DYNALINK_FAILED:
			TRACE("DF\n");
			break;
		case (Word)TOO_MANY_USERS:
			TRACE("TMU\n");
			break;
		case INVALID_LIC:
			TRACE("IL\n");
			break;
		case NO_LICENSE:
			TRACE("NL\n");
			break;
		default:
			bOk = true;
			break;
      }

	if(! bOk)
		{
		HL_LOGOUT();
		return false;
		}

	if(HL_READBL(pMem) != STATUS_OK)
		{
		HL_LOGOUT();
		return false;
		}

	for(i = 0; i < 64; i += 2)
		{
		BYTE nTemp = pMem[i];
		pMem[i] = pMem[i+1];
		pMem[i+1] = nTemp;
		}

	HL_LOGOUT();
	return true;
	}

//--------------------------------------------------------------------------------
bool CDongle::SetDongleMemory(BYTE pMem[32])
	{
#ifdef _DEBUG
	if(m_bDoNothing)
		return true;
#endif

	char pRef[9];
	char pVer[16];

	memcpy(pRef, m_pRefKey, 9);
	memcpy(pVer, m_pVerKey, 16);

	char* pRealVer = NULL;

	pRealVer = &pVer[7];

	for(int i = 0; i < 9; i++)
		pRef[i] -= 2;

	CSingleLock lock(&m_mutex);
	while(! GetCanAccess())
		::Sleep(10);
	SetLastAccessTime();
	bool bOk = false;
	switch(HL_LOGIN(MAGIC1(), LOCAL_DEVICE, (Byte*) pRef, (Byte*) pRealVer))
		{
		case NETWORK_ERROR:
			TRACE("NE\n");
			break;
		case NO_DONGLE:
			TRACE("ND\n");
			HL_LOGOUT();
			return false;
		case VERSION_MISMATCH:
			TRACE("VM\n");
			break;
		case INVALID_ENV:
			TRACE("IE\n");
			break;
		case CANNOT_OPEN_DRIVER:
			TRACE("COD\n");
			break;
		case DYNALINK_FAILED:
			TRACE("DF\n");
			break;
		case (Word)TOO_MANY_USERS:
			TRACE("TMU\n");
			break;
		case INVALID_LIC:
			TRACE("IL\n");
			break;
		case NO_LICENSE:
			TRACE("NL\n");
			break;
		default:
			bOk = true;
			break;
      }

	if(! bOk)
		{
		HL_LOGOUT();
		return false;
		}

	if(HL_WRITEBL(pMem) != STATUS_OK)
		{
		HL_LOGOUT();
		return false;
		}

	HL_LOGOUT();
	return true;
	}

//--------------------------------------------------------------------------------
bool CDongle::IsCorrectDongleMemory()
	{
	return false;
	}

#pragma optimize("", on)

#endif // _SECCON_NODONGLE
