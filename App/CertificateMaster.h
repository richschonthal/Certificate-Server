//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _CERTIFICATEMASTER_H_
#define _CERTIFICATEMASTER_H_

//--------------------------------------------------------------------------------
#include <Certificate.h>
#include <ReadWriteObject.h>
#include <DBDatabase.h>
#include "DBTokens.h"

//--------------------------------------------------------------------------------
class CCertificateMaster : public CReadWriteObject, public CDBDatabaseElement, public CCertificate
	{
	friend class CDBSubSystem;

	protected:
		LPTSTR m_pIP;
		CTime m_ctLastRefresh;
		CDBTokenInfoMap* m_pTokenInfoMap;

	public:
		CCertificateMaster();
		virtual ~CCertificateMaster();

		LPCTSTR GetIP() const;
		void SetIP(LPCTSTR);

		CTime GetLastRefresh();
		void SetLastRefresh(CTime);

		CDBTokenInfoMap* GetTokenInfoMap();
	};

#endif // _CERTIFICATEMASTER_H_
