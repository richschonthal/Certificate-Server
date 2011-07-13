//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _LICENSEFILE_H_
#define _LICENSEFILE_H_

//--------------------------------------------------------------------------------
#include <AfxTempl.h>

//--------------------------------------------------------------------------------
class CDBTokenInfo;

//--------------------------------------------------------------------------------
class CLicenseFile : public CFile
	{
	public:
		char m_sP3Serial[64];
		char m_sDongle[64];
		char m_sEmailIP[64];
		char m_sEmailFrom[64];
		char m_sEmailTo[64];
		CTime m_ctExpiration;

		char m_sMonitorUser[64];
		char m_sMonitorPass[64];

		bool m_bIsBackup;
		// the amount of time allowed for the backup system to run
		// before the dongle is moved from the down main system
		int m_nMinutesToWaitForDongle;

		UCHAR* m_pExtraData;
		int m_nExtraDataLen;

	private:
		char* m_pBuffer;

	private:
		void ParseEmailField(const CString&, char*, char*, char*);

	public:
		CLicenseFile();
		virtual ~CLicenseFile();

		bool Read();

		class CTokenInfoList : public CTypedPtrArray<CPtrArray, CDBTokenInfo*>
			{
			public:
				~CTokenInfoList();
			} m_tokens;
	};

#endif // _LICENSEFILE_H_
