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
		static CMutex m_mutex;

#endif // _SECCON_NODONGLE

	public:
		bool Connect();
		bool GetDongleMemory(BYTE[128]);
		bool SetDongleMemory(BYTE[32]);

	public:
		// override to provide your own memory comparison
		virtual bool IsCorrectDongleMemory();

	protected:
		// override for non-default values
		virtual WORD GetMagicNumber();
		virtual BYTE* GetRefKey();
		virtual BYTE* GetVersionKey();
	};

#endif // _DONGLE_H_
