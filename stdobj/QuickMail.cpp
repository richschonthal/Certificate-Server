//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "QuickMail.h"
#include "SmallSocket.h"

#define RECEIVEWAIT 5000

//--------------------------------------------------------------------------------
CQuickMail::CQuickMail(LPCTSTR pIP, LPCTSTR pUser, LPCTSTR pFrom, LPCTSTR pTo, LPCTSTR pSubject, LPCTSTR pMsg)
		: m_nPort(25)
	{
	if(pIP != NULL)
		m_sIP = pIP;
	if(pUser != NULL)
		m_sUser = pUser;
	if(pFrom != NULL)
		m_sFrom = pFrom;
	if(pTo != NULL)
		m_sTo = pTo;
	if(pSubject != NULL)
		m_sSubject = pSubject;
	if(pMsg != NULL)
		m_sMsg = pMsg;
	}

//--------------------------------------------------------------------------------
bool CQuickMail::Send(LPCTSTR pIP, int nPort, LPCTSTR pUser, LPCTSTR pFrom, LPCTSTR pTo, LPCTSTR pSubject, LPCTSTR pMsg)
	{
	CSmallSocket sock(true);

	if(! sock.Create())
		{
		TRACE("CQuickMail::Send err 1 %ld\n", sock.GetLastError());
		SetResult(errOS, sock.GetLastError());
		return false;
		}

	if(! sock.Connect(pIP, nPort))
		{
		TRACE("CQuickMail::Send err 2 %ld\n", sock.GetLastError());
		SetResult(errOS, sock.GetLastError());
		return false;
		}

#define IS_ERROR_CODE (sTemp.GetLength() > 1 &&(sTemp[0] == '4' || sTemp[0] == '5'))

	CString sTemp;
	sTemp.Format(_T("helo <%s>\r\n"), pUser);
	if(! sock.Send(sTemp, CSmallSocket::WAITFORWOULDBLOCK, RECEIVEWAIT))
		{
		TRACE("CQuickMail::Send err 3 %ld\n", sock.GetLastError());
		SetResult(errOS, sock.GetLastError());
		return false;
		}

	int nErr = sock.Receive(sTemp, CSmallSocket::WAITFORWOULDBLOCK, RECEIVEWAIT);
	if(nErr == SOCKET_ERROR || IS_ERROR_CODE)
		{
		TRACE("CQuickMail::Send err 4 %ld\n", sock.GetLastError());
		SetResult(errOS, sock.GetLastError());
		if(! sTemp.IsEmpty())
			SetErrorMessage(sTemp);
		return false;
		}

	sTemp.Format(_T("mail from: %s\r\n"), pFrom);
	if(! sock.Send(sTemp, CSmallSocket::WAITFORWOULDBLOCK, RECEIVEWAIT))
		{
		TRACE("CQuickMail::Send err 5 %ld\n", sock.GetLastError());
		SetResult(errOS, sock.GetLastError());
		return false;
		}

	nErr = sock.Receive(sTemp, CSmallSocket::WAITFORWOULDBLOCK, RECEIVEWAIT);
	if(nErr == SOCKET_ERROR || IS_ERROR_CODE)
		{
		TRACE("CQuickMail::Send err 6 %ld\n", sock.GetLastError());
		SetResult(errOS, sock.GetLastError());
		if(! sTemp.IsEmpty())
			SetErrorMessage(sTemp);
		return false;
		}

	sTemp.Format(_T("rcpt to: %s\r\n"), pTo);
	if(! sock.Send(sTemp, CSmallSocket::WAITFORWOULDBLOCK, RECEIVEWAIT))
		{
		TRACE("CQuickMail::Send err 7 %ld\n", sock.GetLastError());
		SetResult(errOS, sock.GetLastError());
		return false;
		}

	nErr = sock.Receive(sTemp, CSmallSocket::WAITFORWOULDBLOCK, RECEIVEWAIT);
	if(nErr == SOCKET_ERROR || IS_ERROR_CODE)
		{
		TRACE("CQuickMail::Send err 8 %ld\n", sock.GetLastError());
		SetResult(errOS, sock.GetLastError());
		if(! sTemp.IsEmpty())
			SetErrorMessage(sTemp);
		return false;
		}

	sTemp = _T("data\r\n");
	if(! sock.Send(sTemp, CSmallSocket::WAITFORWOULDBLOCK, RECEIVEWAIT))
		{
		TRACE("CQuickMail::Send err 9 %ld\n", sock.GetLastError());
		SetResult(errOS, sock.GetLastError());
		return false;
		}

	nErr = sock.Receive(sTemp, CSmallSocket::WAITFORWOULDBLOCK, RECEIVEWAIT);
	if(nErr == SOCKET_ERROR || IS_ERROR_CODE)
		{
		TRACE("CQuickMail::Send err 10 %ld\n", sock.GetLastError());
		SetResult(errOS, sock.GetLastError());
		if(! sTemp.IsEmpty())
			SetErrorMessage(sTemp);
		return false;
		}

	TIME_ZONE_INFORMATION tzi;
	::GetTimeZoneInformation(&tzi);

	CString sTimeZoneInfo;
	int nOffset = -(((tzi.Bias+tzi.DaylightBias) / 60)*100);
	bool bIsNeg = nOffset < 0;
	nOffset = abs(nOffset);

	sTimeZoneInfo.Format(_T("%c%04d"), bIsNeg ? '-' : '+', nOffset);
	CTime ctNow(CTime::GetCurrentTime());

	sTemp.Format(_T("From: %s\r\nTo: %s\r\nDate: %s %s\r\nSubject: %s\r\n\r\n%s\r\n.\r\n"), 
		pFrom, pTo, (LPCTSTR) ctNow.Format(_T("%a, %d %b %y %H:%M:%S")), (LPCTSTR) sTimeZoneInfo, pSubject, pMsg);

	if(! sock.Send(sTemp, sTemp.GetLength(), CSmallSocket::WAITFORWOULDBLOCK, 100))
		{
		TRACE("CQuickMail::Send err 11 %ld\n", sock.GetLastError());
		SetResult(errOS, sock.GetLastError());
		return false;
		}

	nErr = sock.Receive(sTemp, CSmallSocket::WAITFORWOULDBLOCK, 100);
	if(nErr == SOCKET_ERROR || IS_ERROR_CODE)
		{
		TRACE("CQuickMail::Send err 12 %ld\n", sock.GetLastError());
		SetResult(errOS, sock.GetLastError());
		if(! sTemp.IsEmpty())
			SetErrorMessage(sTemp);
		return false;
		}

	return true;

#undef IS_ERROR_CODE
	}

//--------------------------------------------------------------------------------
bool CQuickMail::Send()
	{
	return Send(m_sIP, m_nPort, m_sUser, m_sFrom, m_sTo, m_sSubject, m_sMsg);
	}

//--------------------------------------------------------------------------------
CString CQuickMail::GetFormattedMsg()
	{
	CString sTemp;
	sTemp.Format(_T("IP = %s User=%s\nFrom: %s\nTo: %s\nSubject: %s\n%s\n"),
		(LPCTSTR) m_sIP,
		(LPCTSTR) m_sUser,
		(LPCTSTR) m_sFrom,
		(LPCTSTR) m_sTo,
		(LPCTSTR) m_sSubject,
		(LPCTSTR) m_sMsg );

	return sTemp;
	}
