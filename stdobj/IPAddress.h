//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

// IPAddress.h: interface for the CIPAddress class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPADDRESS_H__7951B9A1_74F9_11D3_AEE4_005004A1C5F3__INCLUDED_)
#define AFX_IPADDRESS_H__7951B9A1_74F9_11D3_AEE4_005004A1C5F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------------
typedef struct tagIPADDRESS
	{
	union
		{
		struct
			{
			unsigned n1:8;
			unsigned n2:8;
			unsigned n3:8;
			unsigned n4:8;
			} m_bits;

		DWORD m_nDWord;
		};
	} IPADDRESS;

//--------------------------------------------------------------------------------
class CIPAddress  : public IPADDRESS
	{
	public:
		CIPAddress(DWORD = 0);
		CIPAddress(LPCTSTR);
		CIPAddress(const CIPAddress&);

		virtual ~CIPAddress();

		static bool StringToAddr(LPCTSTR, DWORD&);
		static CString AddrToString(DWORD);

	public:
		operator DWORD() const;
		operator DWORD&();
		operator=(LPCTSTR);
		operator=(DWORD);
		operator=(const CIPAddress&);

		DWORD GetAddress() const;
		void GetAddress(CString&) const;

		void SetAddress(DWORD);
		void SetAddress(LPCTSTR);
	};

#endif // !defined(AFX_IPADDRESS_H__7951B9A1_74F9_11D3_AEE4_005004A1C5F3__INCLUDED_)
