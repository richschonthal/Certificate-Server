//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _SSQUICKMAIL_H_
#define _SSQUICKMAIL_H_

#include <QuickMail.h>

//--------------------------------------------------------------------------------
class CSSQuickMail : public CQuickMail
	{
	public:
		CSSQuickMail();
		virtual bool Send();
	};

#endif //_SSQUICKMAIL_H_
