//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _ACCESSTOKEN_H_
#define _ACCESSTOKEN_H_

//--------------------------------------------------------------------------------
class CAccessToken
	{
	public:
		CAccessToken();

	public:
		enum
			{
			DWordCount = 4,
			BitCount = DWordCount * (sizeof(DWORD) * 8)
			};

		CAccessToken(const DWORD[DWordCount]);
		CAccessToken(const CAccessToken&);
		virtual ~CAccessToken();

	private:
		unsigned long m_nData[DWordCount];

	public:
		unsigned long m_nVersion;
		unsigned long m_nId;
		char* m_pName;
		int m_nExtraLen;
		unsigned char* m_pExtra;

	public:
		unsigned long GetId() const;
		const char* GetName() const;

		CAccessToken& operator=(const CAccessToken&);
		CAccessToken operator&(const CAccessToken&) const;
		CAccessToken operator|(const CAccessToken&) const;
		bool operator==(const CAccessToken&) const;
		bool operator!=(const CAccessToken&) const;

		void SetBit(UINT, bool);
		bool IsBitSet(UINT);

		int Encode(char*);
		int Decode(const char*, int);

		void SetExtraInfo(void*, int);
		void GetExtraInfo(void*&, int&);
	};

//--------------------------------------------------------------------------------
#if _MFC_VER > 0

//--------------------------------------------------------------------------------
#include <afxtempl.h>

//--------------------------------------------------------------------------------
class CAccessTokenMap : public CMap<ULONG, ULONG, CAccessToken*, CAccessToken*>
	{
	public:
		~CAccessTokenMap();
		operator=(const CAccessTokenMap& rVal);
		bool operator==(const CAccessTokenMap&);
	};

#endif

#endif // _ACCESSTOKEN_H_

