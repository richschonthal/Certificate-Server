//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _SSSUBSYSTEM_H_
#define _SSSUBSYSTEM_H_

//--------------------------------------------------------------------------------
#include "SecuritySystem.h"

class CSSIOSubSystem;
class CDBSubSystem;
class CClientSubSystem;
class CSecuritySystem;

//--------------------------------------------------------------------------------
template<class BASECLASS>
class CSSSubSystem : public BASECLASS
	{
	private:
		CSSIOSubSystem* m_pIO;
		CDBSubSystem* m_pDB;
		CClientSubSystem* m_pClient;

	public:
		CSSSubSystem(CSecuritySystem* pSystem)
				: BASECLASS(pSystem), m_pIO(NULL) , m_pDB(NULL) , m_pClient(NULL) {}

		CSecuritySystem* GetSystem() { return (CSecuritySystem*) BASECLASS::GetSystem(); }

		CSSIOSubSystem* GetIO()
			{
			if(m_pIO == NULL)
				m_pIO = GetSystem()->GetIOSubSystem();
			return m_pIO;
			}

		CDBSubSystem* GetDB()
			{
			if(m_pDB == NULL)
				m_pDB = GetSystem()->GetDBSubSystem();
			return m_pDB;
			}

		CClientSubSystem* GetClientSubSystem()
			{
			if(m_pClient == NULL)
				m_pClient = GetSystem()->GetClientSubSystem();
			return m_pClient;
			}
	};

/*
//--------------------------------------------------------------------------------
class CSSLBSubSystem : public CLoadBalancedSubSystem
	{
	private:
		CSSIOSubSystem* m_pIO;
		CDBSubSystem* m_pDB;
		CClientSubSystem* m_pClient;

	public:
		CSSLBSubSystem(CSecuritySystem*);

		CSSIOSubSystem* GetIO();
		CDBSubSystem* GetDB();
		CClientSubSystem* GetClientSubSystem();
	};
*/

#endif // _SSSubSystem_H_
