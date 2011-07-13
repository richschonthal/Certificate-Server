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
bool CDongle::GetDongleMemory(BYTE[128])	{ return false; }
//--------------------------------------------------------------------------------
bool CDongle::SetDongleMemory(BYTE[32])	{ return false; }

#else
#include <hlapi_c.h>

//#pragma comment(lib, "hlw32_mc.lib")

#ifdef _DEBUG
bool CDongle::m_bDoNothing = 
#ifdef _CHECK_DONGLE
							false;
#else
							false;
#endif

#endif

CMutex CDongle::m_mutex;
BYTE pVerKey[9] = {0x9A,0x7D,0x8E,0x75,0x4F,0xEF,0xC4,0x39, 0x00};
BYTE pRefKey[9] = {'M', 'O', 'O', 'N', 'A', 'C', 'H', 'I', 0};

// maybe hackers will get confused and think its already been cracked!! : )

//--------------------------------------------------------------------------------
WORD CDongle::GetMagicNumber()
	{// 0x612c or 0x6122
	return 0x612c;
	}

//--------------------------------------------------------------------------------
BYTE* CDongle::GetRefKey()
	{
	return pRefKey;
	}

//--------------------------------------------------------------------------------
BYTE* CDongle::GetVersionKey()
	{
	return pVerKey;
	}

//--------------------------------------------------------------------------------
bool CDongle::Connect()
	{
#ifdef _DEBUG
	if(m_bDoNothing)
		return true;
#endif

	CSingleLock lock(&m_mutex);
	if(! lock.Lock(3000))
		return false;

	bool bOk = false;
	switch(HL_LOGIN(GetMagicNumber(), LOCAL_DEVICE, GetRefKey(), GetVersionKey()))
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
	return bOk;
	}

//--------------------------------------------------------------------------------
bool CDongle::GetDongleMemory(BYTE pMem[128])
	{
#ifdef _DEBUG
	if(m_bDoNothing)
		return true;
#endif

	CSingleLock lock(&m_mutex);
	if(! lock.Lock(3000))
		return false;

	bool bOk = false;
	switch(HL_LOGIN(GetMagicNumber(), LOCAL_DEVICE, GetRefKey(), GetVersionKey()))
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

	for(int i = 0; i < 64; i += 2)
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
	CSingleLock lock(&m_mutex);
	if(! lock.Lock(3000))
		return false;

	bool bOk = false;
	switch(HL_LOGIN(GetMagicNumber(), LOCAL_DEVICE, GetRefKey(), GetVersionKey()))
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

	if(bOk)
		bOk = HL_WRITEBL(pMem) == STATUS_OK;

	HL_LOGOUT();
	return bOk;
	}

//--------------------------------------------------------------------------------
bool CDongle::IsCorrectDongleMemory()
	{
	return false;
	}

#endif // _SECCON_NODONGLE
