//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _DONGLE_H_
#define _DONGLE_H_

//--------------------------------------------------------------------------------
class CDongle
	{
#ifndef _SECCON_NODONGLE
#ifdef _DEBUG
	public:
		static bool m_bDoNothing;
#endif

	protected:
		static BYTE m_pRefKey[9];
		static BYTE m_pVerKey[16];
		static WORD m_nMagicNumber1;
		static WORD m_nMagicNumber2;
		static DWORD m_nMagicNumber3;
		static DWORD m_nUselessVariable;

		static CMutex m_mutex;
		static CTime m_ctLastAccess;
		static CTime m_ctLastConnectOk;

	public:
		static DWORD m_nRunOkFlag;

	public:
		inline static void SetRunOk(bool b)
			{ b ? ::InterlockedDecrement((long*) &m_nRunOkFlag) : ::InterlockedIncrement((long*) &m_nRunOkFlag); }

	protected:
		static void SetLastAccessTime();
		static bool GetCanAccess();

#endif // _SECCON_NODONGLE

	public:
		bool Connect();
		bool Connect2(DWORD nFakeParam1, LPTSTR pFakeParam2);
		bool GetDongleMemory(BYTE[128]);
		bool SetDongleMemory(BYTE[32]);

	public:
		// override to provide your own memory comparison
		virtual bool IsCorrectDongleMemory();
	};

#endif // _DONGLE_H_
