//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include "LicenseCheckThread.h"

#include "dongle.h"
#include "SSConfigGeneral.h"
#include "SSConfigBackup.h"
#include "SSQuickMail.h"

#define SIGNATURE "MarkCareMedicalSystemsSecurityServerDongle-"

//--------------------------------------------------------------------------------
CLicenseCheckThread::CLicenseCheckThread(CSystemObject* pParent)
		: CSSThread(pParent)
		, m_ctLastCheckLicFile(0)
	{
	}

//--------------------------------------------------------------------------------
bool CLicenseCheckThread::Init()
	{
	m_bFirstRun = m_bRunOk = true;
	SetThreadPriority(THREAD_PRIORITY_ABOVE_NORMAL);
	return true;
	}

//--------------------------------------------------------------------------------
bool CLicenseCheckThread::MainLoop()
	{
	DoChecking();
	return CThreadObject::MainLoop();
	}

//--------------------------------------------------------------------------------
void CLicenseCheckThread::DoChecking()
	{
	CTime ctNow(CTime::GetCurrentTime());

	if((ctNow - m_ctLastCheckLicFile).GetDays() > 0)
		{
		m_ctLastCheckLicFile = ctNow;
		CSSConfigGeneral config;
		LONG nDaysTilExpire = (config.m_ctExpiration - ctNow).GetDays();
		if(nDaysTilExpire <= 5)
			{
			if(nDaysTilExpire < 1)
				{
				CSSQuickMail* pMail = new CSSQuickMail;
				pMail->m_sMsg = pMail->m_sSubject = "ERR: License file expired!";
				GetIO()->SendQuickMail(pMail);
				}
			else
				{
				CSSQuickMail* pMail = new CSSQuickMail;
				pMail->m_sSubject.Format("ERR: License file will expire in %d day%c", nDaysTilExpire,
					nDaysTilExpire == 1 ? ' ': 's');
				pMail->m_sMsg = pMail->m_sSubject;
				GetIO()->SendQuickMail(pMail);
				}
			}
		}

	if(GetParent()->GetDongleDeadlineTime() > CTime::GetCurrentTime() || ! GetParent()->IsServing())
		return;

	// first check the dongle signature and serial number
	CDongle dongle;
	BYTE nMem[128];
	bool bDongleOk = false;
	if(dongle.GetDongleMemory(nMem))
		{
		CString sTemp(SIGNATURE);
		CSSConfigGeneral config;
		sTemp += config.m_sDongle;
		if(sTemp != (LPCTSTR) nMem)
			{
			GetSystem()->SetDongleStatus(false);
			return;
			}

		// now check the last run date
		time_t* pLastRun = (time_t*) &nMem[96];

		// if last run is 0 then this is the first time we're using this dongle
		if(*pLastRun == 0)
			{
			*pLastRun = CTime::GetCurrentTime().GetTime();
			dongle.SetDongleMemory(&nMem[96]);
			bDongleOk = true;
			}
		else
			{
			// we've used this dongle already
			TRY
				{
				// invalid dates cause an exception there shouldnt be an invalid date
				// if there is then there must have been tampering
				CTime ctLastRun(*pLastRun);

				// the clock has not been turned back
				// just check for days so we dont get confused by DST change etc
				if( (CTime::GetCurrentTime() - ctLastRun).GetDays() >= 0)
					{
					// the last run time checks out so if this is our first run today
					// update the last run in the dongle
					if(m_bFirstRun)
						{
						m_bFirstRun = false;
						*pLastRun = CTime::GetCurrentTime().GetTime();
						dongle.SetDongleMemory(&nMem[96]);
						}

					bDongleOk = true;
					}
				}
			CATCH_ALL(e)
				{
				}
			END_CATCH_ALL
			}
		}

	GetSystem()->SetDongleStatus(bDongleOk);
	}

//--------------------------------------------------------------------------------
DWORD CLicenseCheckThread::GetThreadLoopSleepInterval() const
	{
	return 60000;
	}
