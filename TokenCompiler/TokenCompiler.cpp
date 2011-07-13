//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#include "stdafx.h"
#include <conio.h>
#include <assert.h>
#include <time.h>
#include <exception>
#include <SecServTokenId.h>

// this file is not included in sourcesafe for security reasons
// it contains 1 define statement: #define PASSWORD "password"
#include "password.h"

//--------------------------------------------------------------------------------
struct tagTokenNameTable
	{
	unsigned long m_nId;
	LPCTSTR m_pText;
	} g_tokenNameTable[] =
	{
	SSTokenId::Unknown,				"Unknown",
	SSTokenId::Viewer2,				"Viewer2",
	SSTokenId::Viewer3,				"Viewer3",
	SSTokenId::Viewer4,				"Viewer4",
	SSTokenId::Viewer5,				"Viewer5",
	SSTokenId::Viewer6,				"Viewer6",
	SSTokenId::Viewer7,				"Viewer7",
	SSTokenId::Viewer8,				"Viewer8",
	SSTokenId::ViewerTest2,			"ViewerTest2",
	SSTokenId::ViewerTest3,			"ViewerTest3",
	SSTokenId::ViewerTest4,			"ViewerTest4",
	SSTokenId::ViewerTest5,			"ViewerTest5",
	SSTokenId::ViewerTest6,			"ViewerTest6",
	SSTokenId::ViewerTest7,			"ViewerTest7",
	SSTokenId::ViewerTest8,			"ViewerTest8",
	SSTokenId::HL7_3,				"HL7_3",
	SSTokenId::HL7_4,				"HL7_4",
	SSTokenId::HL7_5,				"HL7_5",
	SSTokenId::HL7_3Test,			"HL7_3Test",
	SSTokenId::HL7_4Test,			"HL7_4Test",
	SSTokenId::HL7_5Test,			"HL7_5Test",
	SSTokenId::TeleRadServer2,		"TeleRadServer2",
	SSTokenId::TeleRadServer3,		"TeleRadServer3",
	SSTokenId::TeleRadServer4,		"TeleRadServer4",
	SSTokenId::TeleRadServer5,		"TeleRadServer5",
	SSTokenId::TeleRadServer2Test,	"TeleRadServer2Test",
	SSTokenId::TeleRadServer3Test,	"TeleRadServer3Test",
	SSTokenId::TeleRadServer4Test,	"TeleRadServer4Test",
	SSTokenId::TeleRadServer5Test,	"TeleRadServer5Test",
	SSTokenId::AcqModule1,			"AcqModule1",
	SSTokenId::AcqModule2,			"AcqModule2",
	SSTokenId::AcqModule3,			"AcqModule3",
	SSTokenId::AcqModule4,			"AcqModule4",
	SSTokenId::AcqModule5,			"AcqModule5",
	SSTokenId::AcqModule1Test,		"AcqModule1Test",
	SSTokenId::AcqModule2Test,		"AcqModule2Test",
	SSTokenId::AcqModule3Test,		"AcqModule3Test",
	SSTokenId::AcqModule4Test,		"AcqModule4Test",
	SSTokenId::AcqModule5Test,		"AcqModule5Test",
	SSTokenId::MCImageLib1,			"MCImageLib1",
	SSTokenId::MCImageLib2,			"MCImageLib2",
	SSTokenId::MCImageLib3,			"MCImageLib3",
	SSTokenId::MCImageLib4,			"MCImageLib4",
	SSTokenId::MCImageLib5,			"MCImageLib5",
	SSTokenId::MCImageLib1Test,		"MCImageLib1Test",
	SSTokenId::MCImageLib2Test,		"MCImageLib2Test",
	SSTokenId::MCImageLib3Test,		"MCImageLib3Test",
	SSTokenId::MCImageLib4Test,		"MCImageLib4Test",
	SSTokenId::MCImageLib5Test,		"MCImageLib5Test",
	SSTokenId::RisHis1,				"RisHis1",
	SSTokenId::RisHis2,				"RisHis2",
	SSTokenId::RisHis3,				"RisHis3",
	SSTokenId::RisHis4,				"RisHis4",
	SSTokenId::RisHis5,				"RisHis5",
	SSTokenId::RisHis1Test,			"RisHis1Test",
	SSTokenId::RisHis2Test,			"RisHis2Test",
	SSTokenId::RisHis3Test,			"RisHis3Test",
	SSTokenId::RisHis4Test,			"RisHis4Test",
	SSTokenId::RisHis5Test,			"RisHis5Test",
	SSTokenId::MDServer7,			"MDServer7",
	SSTokenId::MDServer8,			"MDServer8",
	SSTokenId::MDServer9,			"MDServer9",
	SSTokenId::MDServer10,			"MDServer10",
	SSTokenId::MDServer7Test,		"MDServer7Test",
	SSTokenId::MDServer8Test,		"MDServer8Test",
	SSTokenId::MDServer9Test,		"MDServer9Test",
	SSTokenId::MDServer10Test,		"MDServer10Test",
	SSTokenId::ArchiveD1,			"ArchiveD1",
	SSTokenId::ArchiveD2,			"ArchiveD2",
	SSTokenId::ArchiveD3,			"ArchiveD3",
	SSTokenId::ArchiveD4,			"ArchiveD4",
	SSTokenId::ArchiveD5,			"ArchiveD5",
	SSTokenId::ArchiveD1Test,		"ArchiveD1Test",
	SSTokenId::ArchiveD2Test,		"ArchiveD2Test",
	SSTokenId::ArchiveD3Test,		"ArchiveD3Test",
	SSTokenId::ArchiveD4Test,		"ArchiveD4Test",
	SSTokenId::ArchiveD5Test,		"ArchiveD5Test",
	SSTokenId::ArchiveAIB1,			"ArchiveAIB1",
	SSTokenId::ArchiveAIB2,			"ArchiveAIB2",
	SSTokenId::ArchiveAIB3,			"ArchiveAIB3",
	SSTokenId::ArchiveAIB4,			"ArchiveAIB4",
	SSTokenId::ArchiveAIB5,			"ArchiveAIB5",
	SSTokenId::ArchiveAIB1Test,		"ArchiveAIB1Test",
	SSTokenId::ArchiveAIB2Test,		"ArchiveAIB2Test",
	SSTokenId::ArchiveAIB3Test,		"ArchiveAIB3Test",
	SSTokenId::ArchiveAIB4Test,		"ArchiveAIB4Test",
	SSTokenId::ArchiveAIB5Test,		"ArchiveAIB5Test",
	SSTokenId::CompressServer1,		"CompressServer1",
	SSTokenId::CompressServer2,		"CompressServer2",
	SSTokenId::CompressServer3,		"CompressServer3",
	SSTokenId::CompressServer4,		"CompressServer4",
	SSTokenId::CompressServer5,		"CompressServer5",
	SSTokenId::CompressServer1Test,	"CompressServer1Test",
	SSTokenId::CompressServer2Test,	"CompressServer2Test",
	SSTokenId::CompressServer3Test,	"CompressServer3Test",
	SSTokenId::CompressServer4Test,	"CompressServer4Test",
	SSTokenId::CompressServer5Test,	"CompressServer5Test",
	SSTokenId::WorkListMgr1,		"WorkListMgr1",
	SSTokenId::WorkListMgr2,		"WorkListMgr2",
	SSTokenId::WorkListMgr3,		"WorkListMgr3",
	SSTokenId::WorkListMgr4,		"WorkListMgr4",
	SSTokenId::WorkListMgr5,		"WorkListMgr5",
	SSTokenId::WorkListMgr1Test,	"WorkListMgr1Test",
	SSTokenId::WorkListMgr2Test,	"WorkListMgr2Test",
	SSTokenId::WorkListMgr3Test,	"WorkListMgr3Test",
	SSTokenId::WorkListMgr4Test,	"WorkListMgr4Test",
	SSTokenId::WorkListMgr5Test,	"WorkListMgr5Test",
	SSTokenId::AdHoc2,				"AdHoc2",
	SSTokenId::AdHoc3,				"AdHoc3",
	SSTokenId::AdHoc4,				"AdHoc4",
	SSTokenId::AdHoc5,				"AdHoc5",
	SSTokenId::AdHoc6,				"AdHoc6",
	SSTokenId::AdHoc7,				"AdHoc7",
	SSTokenId::AdHoc8,				"AdHoc8",
	SSTokenId::AdHoc2Test,			"AdHoc2Test",
	SSTokenId::AdHoc3Test,			"AdHoc3Test",
	SSTokenId::AdHoc4Test,			"AdHoc4Test",
	SSTokenId::AdHoc5Test,			"AdHoc5Test",
	SSTokenId::AdHoc6Test,			"AdHoc6Test",
	SSTokenId::AdHoc7Test,			"AdHoc7Test",
	SSTokenId::AdHoc8Test,			"AdHoc8Test",
	SSTokenId::ViewPrint2,			"ViewPrint2",
	SSTokenId::ViewPrint3,			"ViewPrint3",
	SSTokenId::ViewPrint4,			"ViewPrint4",
	SSTokenId::ViewPrint5,			"ViewPrint5",
	SSTokenId::ViewPrint6,			"ViewPrint6",
	SSTokenId::ViewPrint7,			"ViewPrint7",
	SSTokenId::ViewPrint8,			"ViewPrint8",
	SSTokenId::ViewPrint2Test,		"ViewPrint2Test",
	SSTokenId::ViewPrint3Test,		"ViewPrint3Test",
	SSTokenId::ViewPrint4Test,		"ViewPrint4Test",
	SSTokenId::ViewPrint5Test,		"ViewPrint5Test",
	SSTokenId::ViewPrint6Test,		"ViewPrint6Test",
	SSTokenId::ViewPrint7Test,		"ViewPrint7Test",
	SSTokenId::ViewPrint8Test,		"ViewPrint8Test",
	SSTokenId::DeptServ2,			"DeptServ2",
	SSTokenId::DeptServ3,			"DeptServ3",
	SSTokenId::DeptServ4,			"DeptServ4",
	SSTokenId::DeptServ5,			"DeptServ5",
	SSTokenId::DeptServ6,			"DeptServ6",
	SSTokenId::DeptServ7,			"DeptServ7",
	SSTokenId::DeptServ8,			"DeptServ8",
	SSTokenId::DeptServ2Test,		"DeptServ2Test",
	SSTokenId::DeptServ3Test,		"DeptServ3Test",
	SSTokenId::DeptServ4Test,		"DeptServ4Test",
	SSTokenId::DeptServ5Test,		"DeptServ5Test",
	SSTokenId::DeptServ6Test,		"DeptServ6Test",
	SSTokenId::DeptServ7Test,		"DeptServ7Test",
	SSTokenId::DeptServ8Test,		"DeptServ8Test",
	SSTokenId::IMS_1,				"IMS_1", 
	SSTokenId::IMS_2,				"IMS_2", 
	SSTokenId::IMS_3,				"IMS_3", 
	SSTokenId::IMS_4,				"IMS_4", 
	SSTokenId::IMS_5,				"IMS_5", 
	SSTokenId::IMS_6,				"IMS_6", 
	SSTokenId::IMS_1Test,			"IMS_1Test", 
	SSTokenId::IMS_2Test,			"IMS_2Test", 
	SSTokenId::IMS_3Test,			"IMS_3Test", 
	SSTokenId::IMS_4Test,			"IMS_4Test", 
	SSTokenId::IMS_5Test,			"IMS_5Test", 
	SSTokenId::IMS_6Test,			"IMS_6Test", 
	SSTokenId::Digitizer1,			"Digitizer1", 
	SSTokenId::Digitizer2,			"Digitizer2", 
	SSTokenId::Digitizer3,			"Digitizer3", 
	SSTokenId::Digitizer4,			"Digitizer4", 
	SSTokenId::Digitizer5,			"Digitizer5", 
	SSTokenId::Digitizer1Test,		"Digitizer1Test", 
	SSTokenId::Digitizer2Test,		"Digitizer2Test", 
	SSTokenId::Digitizer3Test,		"Digitizer3Test", 
	SSTokenId::Digitizer4Test,		"Digitizer4Test", 
	SSTokenId::Digitizer5Test,		"Digitizer5Test", 
	SSTokenId::Modality1,			"Modality1", 
	SSTokenId::Modality2,			"Modality2", 
	SSTokenId::Modality3,			"Modality3", 
	SSTokenId::Modality4,			"Modality4", 
	SSTokenId::Modality5,			"Modality5", 
	SSTokenId::Modality1Test,		"Modality1Test", 
	SSTokenId::Modality2Test,		"Modality2Test", 
	SSTokenId::Modality3Test,		"Modality3Test", 
	SSTokenId::Modality4Test,		"Modality4Test", 
	SSTokenId::Modality5Test,		"Modality5Test", 
	SSTokenId::HIS_RIS_Bi_1,		"HIS_RIS_Bi_1", 
	SSTokenId::HIS_RIS_Bi_2,		"HIS_RIS_Bi_2", 
	SSTokenId::HIS_RIS_Bi_3,		"HIS_RIS_Bi_3", 
	SSTokenId::HIS_RIS_Bi_4,		"HIS_RIS_Bi_4", 
	SSTokenId::HIS_RIS_Bi_5,		"HIS_RIS_Bi_5", 
	SSTokenId::HIS_RIS_Bi_1Test,	"HIS_RIS_Bi_1Test", 
	SSTokenId::HIS_RIS_Bi_2Test,	"HIS_RIS_Bi_2Test", 
	SSTokenId::HIS_RIS_Bi_3Test,	"HIS_RIS_Bi_3Test", 
	SSTokenId::HIS_RIS_Bi_4Test,	"HIS_RIS_Bi_4Test", 
	SSTokenId::HIS_RIS_Bi_5Test,	"HIS_RIS_Bi_5Test",
	SSTokenId::WorkListClient1,		"WorkListClient1", 
	SSTokenId::WorkListClient2,		"WorkListClient2", 
	SSTokenId::WorkListClient3,		"WorkListClient3", 
	SSTokenId::WorkListClient4,		"WorkListClient4", 
	SSTokenId::WorkListClient5,		"WorkListClient5", 
	SSTokenId::WorkListClient1Test, "WorkListClient1Test", 
	SSTokenId::WorkListClient2Test, "WorkListClient2Test", 
	SSTokenId::WorkListClient3Test, "WorkListClient3Test", 
	SSTokenId::WorkListClient4Test, "WorkListClient4Test", 
	SSTokenId::WorkListClient5Test, "WorkListClient5Test", 
	SSTokenId::ViewQA1,				"ViewQA1", 
	SSTokenId::ViewQA2,				"ViewQA2", 
	SSTokenId::ViewQA3,				"ViewQA3", 
	SSTokenId::ViewQA4,				"ViewQA4", 
	SSTokenId::ViewQA5,				"ViewQA5", 
	SSTokenId::ViewQA1Test,			"ViewQA1Test", 
	SSTokenId::ViewQA2Test,			"ViewQA2Test", 
	SSTokenId::ViewQA3Test,			"ViewQA3Test", 
	SSTokenId::ViewQA4Test,			"ViewQA4Test", 
	SSTokenId::ViewQA5Test,			"ViewQA5Test", 
	SSTokenId::AcqModuleFilmScan,	"AcqModuleFilmScan",
	SSTokenId::AcqModuleDocScan,	"AcqModuleDocScan",
	SSTokenId::AcqModuleJpegComp,	"AcqModuleJpegComp", 
	SSTokenId::AcqModuleWaveComp,	"AcqModuleWaveComp",
	SSTokenId::AcqModuleWorklist,	"AcqModuleWorklist",
	};

class CTokenMaster
	{
	public:
		CTokenMaster() {m_nCount = m_nMax = m_nParent = 0;}

	public:
		CAccessToken m_token;
		DWORD m_nCount;
		DWORD m_nMax;
		DWORD m_nParent;
	};

//--------------------------------------------------------------------------------
char g_pP3Serial[64] = {0};
char g_pP3SerialBkup[64] = {0};
char g_pDongle[64] = {0};
char g_pEmail[512] = {0};
CTokenMaster* g_pTokens[128];
int g_nNextToken = 0;
time_t g_nExpires = 0;
bool g_bIsBackup = false;
unsigned long g_nMaxBackupRunTime = 0;
char g_pMonitorUser[64] = {0};
char g_pMonitorPwd[64] = {0};

//--------------------------------------------------------------------------------
int Parse(CStdioFile&, CFile&, CFile&);
void MakeBackupFilename(const char* pIn, char* pOut);
int GetPasswordFromConsole(char* pPass, int nMax, int nCode);

//--------------------------------------------------------------------------------
int main(int argc, char* argv[])
	{
	::SetConsoleTitle("SecurityServer TokenCompiler Version 1.4");

	char sPass[32];
	GetPasswordFromConsole(sPass, 32, 2);

#ifndef _DEBUG
	if(strncmp(sPass, PASSWORD, strlen(PASSWORD)) != 0)
		{
		printf("Password Failed\n");
		return -1;
		}
#endif

	if(argc < 2)
		{
		printf("TokenCompiler by Rich Schonthal\n");
		printf("Copyright (c) 2000 MarkCare Medical Systems, Inc.\n");
		printf("usage:\n");
		printf("tokencompiler <input filename> [output filename]\n");
		printf("tokencompiler -dongle <starting serial number> <count>\n");
		printf("default output filename mcmssecserv.dat\n");
		return -1;
		}

	// generate dongle files only
	if(stricmp("-dongle", argv[1]) == 0)
		{
		if(argc < 4)
			{
			printf("TokenCompiler by Rich Schonthal\n");
			printf("Copyright (c) 2000 MarkCare Medical Systems, Inc.\n");
			printf("usage:\n");
			printf("tokencompiler -dongle <starting serial number> <count>\n");
			return -1;
			}

		int nStart = atoi(argv[2]);
		int nCount = atoi(argv[3]);

		for(int i = nStart; i < nStart + (nCount * 10); i += 10)
			{
			CFile file;
			CString sName;
			sName.Format("%d.sec", i);
			// now write the dongle file
			if(! file.Open(sName, CFile::modeWrite|CFile::modeCreate|CFile::shareExclusive))
				{
				printf("error creating dongle file\n");
				return -1;
				}
			else
				{
				char cTemp[128];
				memset(cTemp, 0, 128);

				sprintf(cTemp, "MarkCareMedicalSystemsSecurityServerDongle-%d", i);
				file.Write(cTemp, 128);
				}
			}
		}

	CStdioFile fIn;
	if(! fIn.Open(argv[1], CFile::modeRead|CFile::shareExclusive))
		{
		printf("error opening %s for input\n", argv[1]);
		return -1;
		}

	LPCTSTR pOutname = argc == 2 ? "mcmssecserv.dat" : argv[2];

	CFile fOut;
	if(! fOut.Open(pOutname, CFile::modeWrite|CFile::modeCreate|CFile::shareExclusive))
		{
		printf("error opening %s for output\n", pOutname);
		return -1;
		}

	char* pBkupName = new char[strlen(pOutname) + 96];
	MakeBackupFilename(pOutname, pBkupName);

	CFile fOutBkup;
	if(! fOutBkup.Open(pBkupName, CFile::modeWrite|CFile::modeCreate|CFile::shareExclusive))
		{
		printf("error opening %s for output\n", pBkupName);
		::DeleteFile(pOutname);
		return -1;
		}

	int nRv = Parse(fIn, fOut, fOutBkup);

	CString sPath(fIn.GetFilePath());
	int nIndex = sPath.ReverseFind('\\');
	if(nIndex > -1)
		{
		sPath.GetBufferSetLength(nIndex + 1);
		sPath.ReleaseBuffer();
		}
	else
		sPath = ".\\";

	fIn.Close();
	fOut.Close();
	fOutBkup.Close();

	if(nRv != 0)
		{
		printf("error in output - file not created\n");
		::DeleteFile(pOutname);
		::DeleteFile(pBkupName);
		}

	// now create the dongle filename
	char* pBackslash = strrchr(pBkupName, '\\');
	char cDongleName[64];

	sprintf(cDongleName, "%08lx.sec", time(NULL));

	if(pBackslash == NULL)
		strcpy(pBkupName, cDongleName);
	else
		strcpy(pBackslash+1, cDongleName);

	{
	// remove old dongle files
	WIN32_FIND_DATA find;
	HANDLE hand = ::FindFirstFile(sPath + "*.sec", &find);
	if(hand != INVALID_HANDLE_VALUE)
		for(;;)
			{
			::DeleteFile(sPath + find.cFileName);
			if(! ::FindNextFile(hand, &find))
				break;
			}
	}

	// now write the dongle file
	if(! fOutBkup.Open(pBkupName, CFile::modeWrite|CFile::modeCreate|CFile::shareExclusive))
		{
		printf("error creating dongle file\n");
		nRv = false;
		}
	else
		{
		char cTemp[128];
		memset(cTemp, 0, 128);

		sprintf(cTemp, "MarkCareMedicalSystemsSecurityServerDongle-%s", g_pDongle);
		fOutBkup.Write(cTemp, 128);
		}

	delete[] pBkupName;
	return nRv;
	}

//--------------------------------------------------------------------------------
void MakeBackupFilename(const char* pIn, char* pOut)
	{
	for(int i = strlen(pIn) - 1;i > 1 || pIn[i] != '\\';i--)
		{
		if(pIn[i] == '.')
			{
			strncpy(pOut, pIn, i);
			strncpy(&pOut[i], "bkup", 4);
			strcpy(&pOut[i+4], &pIn[i]);
			return;
			}
		}

	// we didnt find a suitable place
	// so tack it onto the end
	strcpy(pOut, pIn);
	strcat(pOut, "bkup");
	}

//--------------------------------------------------------------------------------
bool WriteFile(CFile& fOut, CFile& fOutBkup)
	{
	bool bRv = true;

	// pre-flight checks
	if(g_pP3Serial[0] == 0)
		{
		printf("Pentium III serial number must be specified\n");
		bRv = false;
		}

	if(g_pP3SerialBkup[0] == 0)
		printf("WARNING - Backup Server Pentium III serial not specified\n");

	if(g_pDongle[0] == 0)
		{
		printf("Dongle serial number must be specified\n");
		bRv = false;
		}

	if(g_pEmail[0] == 0)
		{
		printf("Email information must be specified (ip.ip.ip.ip, from test, to address)\n");
		bRv = false;
		}

	// this is a run once loop - i do this so that i dont need a goto
	while(bRv)
		{
		BYTE temp[32767];
		BYTE temp2[32767];
		int nIndex;

		// write the signature
		memcpy(temp, "MARKCAREMEDICAL", 16);
		nIndex = 16;

		// write the file version
		DWORD nVersion = 2;
		memcpy(&temp[nIndex], &nVersion, sizeof(nVersion));
		nIndex += sizeof(nVersion);

		// save the index so we can insert the backup serial number later
		int nSerialIndex = nIndex;
		// write the p3 serial number
		memcpy(&temp[nIndex], g_pP3Serial, sizeof(g_pP3Serial));
		nIndex += sizeof(g_pP3Serial);

		// write the dongle serial number
		memcpy(&temp[nIndex], g_pDongle, sizeof(g_pDongle));
		nIndex += sizeof(g_pDongle);

		// write the expiration date
		memcpy(&temp[nIndex], &g_nExpires, sizeof(time_t));
		nIndex += sizeof(time_t);

		// get and write the length of the email address
		DWORD nLen = strlen(g_pEmail);
		if(nLen >= 64)
			{
			printf("error nLen >= 64\n");
			bRv = false;
			break;
			}
		memcpy(&temp[nIndex], &nLen, sizeof(nLen));
		nIndex += sizeof(nLen);

		// write the email info
		memcpy(&temp[nIndex], g_pEmail, nLen);
		nIndex += nLen;

		// we'll need to set true in the backup file
		// and false in the other file so save the location for later
		int nBkupIndex = nIndex;
		// write 1 or 0 for backup
		nIndex += 1;

		// write the max backup runtime even if IsBackup is false
		memcpy(&temp[nIndex], &g_nMaxBackupRunTime, sizeof(g_nMaxBackupRunTime));
		nIndex += sizeof(g_nMaxBackupRunTime);

		// get and write the length of the monitor user name
		nLen = strlen(g_pMonitorUser);
		if(nLen >= 64)
			{
			printf("error nLen >= 64\n");
			bRv = false;
			break;
			}
		memcpy(&temp[nIndex], &nLen, sizeof(nLen));
		nIndex += sizeof(nLen);

		// write the monitor user name
		memcpy(&temp[nIndex], g_pMonitorUser, nLen);
		nIndex += nLen;

		// get and write the length of the monitor password
		nLen = strlen(g_pMonitorPwd);
		if(nLen >= 64)
			{
			printf("error nLen >= 64\n");
			bRv = false;
			break;
			}
		memcpy(&temp[nIndex], &nLen, sizeof(nLen));
		nIndex += sizeof(nLen);

		// write the monitor password
		memcpy(&temp[nIndex], g_pMonitorPwd, nLen);
		nIndex += nLen;

		// write the "extra" data - this version doesnt include any
		// but future versions may write some additional data
		nLen = 0;
		memcpy(&temp[nIndex], &nLen, sizeof(nLen));
		nIndex += sizeof(nLen);

		for(int i = 0; i < g_nNextToken; i++)
			{
			if(g_pTokens[i]->m_nMax == 0)
				{
				g_pTokens[i]->m_nMax = g_pTokens[i]->m_nCount < 100 ? g_pTokens[i]->m_nCount + 5 : g_pTokens[i]->m_nCount + 20;
				printf("WARNING - max not specified for '%s', defaulting to %ld\n", g_pTokens[i]->m_token.m_pName, g_pTokens[i]->m_nMax);
				}
			else
				if(g_pTokens[i]->m_nCount > g_pTokens[i]->m_nMax)
					{
					printf("count must be <= max\n");
					bRv = false;
					break;
					}

			// write the token id
			memcpy(&temp[nIndex], &g_pTokens[i]->m_token.m_nId, sizeof(g_pTokens[i]->m_token.m_nId));
			nIndex += sizeof(g_pTokens[i]->m_token.m_nId);

			// write the length of the name
			int nLen = strlen(g_pTokens[i]->m_token.m_pName);
			if(nLen >= 64)
				{
				printf("error nLen >= 64\n");
				bRv = false;
				goto exit;
				}
			memcpy(&temp[nIndex], &nLen, sizeof(nLen));
			nIndex += sizeof(nLen);

			// if there's a name, write it
			if(nLen > 0)
				memcpy(&temp[nIndex], g_pTokens[i]->m_token.m_pName, nLen);

			nIndex += nLen;

			// serialize the token into a string
			char tempToken[1024];
			nLen = g_pTokens[i]->m_token.Encode(tempToken);

			// write the length of the token
			memcpy(&temp[nIndex], &nLen, sizeof(nLen));
			nIndex += sizeof(nLen);

			// write the token
			memcpy(&temp[nIndex], tempToken, nLen);
			nIndex += nLen;

#ifndef _DEBUG
const int nSize1 = sizeof(char*);
const int nSize2 = sizeof(DWORD);
#if nSize1 != nSize2
#error pointers assumed to be the same size as DWORDs
#endif
#endif
			// write the available count
			memcpy(&temp[nIndex], &(DWORD) g_pTokens[i]->m_nCount, sizeof(g_pTokens[i]->m_nCount));
			nIndex += sizeof(g_pTokens[i]->m_nCount);

			// write the max count
			memcpy(&temp[nIndex], &(DWORD) g_pTokens[i]->m_nMax, sizeof(g_pTokens[i]->m_nMax));
			nIndex += sizeof(g_pTokens[i]->m_nMax);

			// write the parent id
			memcpy(&temp[nIndex], &(DWORD) g_pTokens[i]->m_nParent, sizeof(g_pTokens[i]->m_nParent));
			nIndex += sizeof(g_pTokens[i]->m_nParent);
			}

		memcpy(temp2, temp, sizeof(temp));

		// set the IsBackup bool for each file
		temp[nBkupIndex] = 0;
		temp2[nBkupIndex] = 1;

		// write the p3 serial number
		memcpy(&temp2[nSerialIndex], g_pP3SerialBkup, sizeof(g_pP3SerialBkup));

		// encrypt the whole deal (nIndex == total size)
		CMCEncrypt encrypt(temp, nIndex + 1);

		// write it to the file
		fOut.Write(encrypt.GetData(), encrypt.GetLength());

		// now for the backup file
		CMCEncrypt encrypt2(temp2, nIndex + 1);

		// write it to the file
		fOutBkup.Write(encrypt2.GetData(), encrypt2.GetLength());

		break;
		}

exit:;
	for(int i = 0; i < g_nNextToken; i++)
		delete g_pTokens[i];

	g_nNextToken = 0;
	return bRv;
	}

//--------------------------------------------------------------------------------
bool DoNumber(const char* pBuf, unsigned long& rLong)
	{
	int nBase = 10;

	if(strlen(pBuf) > 2)
		{
		if(pBuf[0] == '0' && (pBuf[1] == 'x' || pBuf[1] == 'X'))
			{
			pBuf += 2;
			nBase = 16;
			}
		}

	char* pErr;
	rLong = strtoul(pBuf, &pErr, nBase);
	return *pErr == 0;
	}

//--------------------------------------------------------------------------------
bool DoTokenIdName(const char* pBuf, unsigned long& rLong)
	{
	const int nSize = sizeof(g_tokenNameTable) / sizeof(struct tagTokenNameTable);
	for(int i = 0; i < nSize; i++)
		{
		if(stricmp(pBuf, g_tokenNameTable[i].m_pText) == 0)
			{
			rLong = g_tokenNameTable[i].m_nId;
			return true;
			}
		}

	return false;
	}

//--------------------------------------------------------------------------------
bool DoToken(const char* pBuf, CTokenMaster*& pMaster)
	{
	pMaster = new CTokenMaster;

	if(isalpha(*pBuf))
		{
		if(! DoTokenIdName(pBuf, pMaster->m_token.m_nId))
			{
			printf("syntax error ^^^^^^^ (token id name not found)\n");
			delete pMaster;
			pMaster = NULL;
			return false;
			}
		}
	else
		if(! DoNumber(pBuf, pMaster->m_token.m_nId))
			{
			printf("syntax error ^^^^^^^ (invalid character in token id)\n");
			delete pMaster;
			pMaster = NULL;
			return false;
			}

	g_pTokens[g_nNextToken++] = pMaster;
	return true;
	}

//--------------------------------------------------------------------------------
bool DoP3Serial(const char* pBuf, CTokenMaster*& pMaster)
	{
	if(pBuf == NULL)
		{
		printf("error ^^^^^^^\n");
		return false;
		}

	char* pSerial2 = strchr(pBuf, ',');
	if(pSerial2 != NULL)
		{
		*pSerial2 = 0;
		pSerial2++;
		}

	char sTemp[256];
	for(unsigned int i = 0; i < 256 && *pBuf; pBuf++)
		if(isalnum(*pBuf))
			sTemp[i++] = *pBuf;

	sTemp[i] = 0;
	strcpy(g_pP3Serial, sTemp);

	if(pSerial2 != NULL)
		{
		for(i = 0; i < 256 && *pSerial2; pSerial2++)
			if(isalnum(*pSerial2))
				sTemp[i++] = *pSerial2;
		sTemp[i] = 0;
		strcpy(g_pP3SerialBkup, sTemp);
		}

	return true;
	}

//--------------------------------------------------------------------------------
bool DoDongle(const char* pBuf, CTokenMaster*& pMaster)
	{
	if(pBuf == NULL)
		{
		printf("error ^^^^^^^\n");
		return false;
		}

	strcpy(g_pDongle, pBuf);
	return true;
	}

//--------------------------------------------------------------------------------
bool DoEmail(const char* pBuf, CTokenMaster*& pMaster)
	{
	if(pBuf == NULL)
		{
		printf("error ^^^^^^^\n");
		return false;
		}

	strcpy(g_pEmail, pBuf);
	return true;
	}

//--------------------------------------------------------------------------------
bool DoBackup(const char* pBuf, CTokenMaster*& pMaster)
	{
	if(pBuf == NULL)
		{
		printf("error ^^^^^^^\n");
		return false;
		}

	g_bIsBackup = true;
	DoNumber(pBuf, g_nMaxBackupRunTime);
	return true;
	}

//--------------------------------------------------------------------------------
bool DoCount(const char* pBuf, CTokenMaster*& pMaster)
	{
	if(pMaster == NULL || pBuf == NULL)
		{
		printf("syntax error ^^^^^^^ (no token declared)\n");
		return false;
		}

	if(! DoNumber(pBuf, pMaster->m_nCount))
		{
		printf("syntax error ^^^^^^^ (invalid character in count)\n");
		return false;
		}

	if(pMaster->m_nCount == 0)
		{
		printf("syntax error ^^^^^^^ (count cannot be zero)\n");
		return false;
		}

	return true;
	}

//--------------------------------------------------------------------------------
bool DoParent(const char* pBuf, CTokenMaster*& pMaster)
	{
	if(pMaster == NULL || pBuf == NULL)
		{
		printf("syntax error ^^^^^^^ (no token declared)\n");
		return false;
		}

	if(isalpha(*pBuf))
		{
		if(! DoTokenIdName(pBuf, pMaster->m_nParent))
			{
			printf("syntax error ^^^^^^^ (token id name not found)\n");
			delete pMaster;
			pMaster = NULL;
			return false;
			}
		}
	else
		if(! DoNumber(pBuf, pMaster->m_nParent))
			{
			printf("syntax error ^^^^^^^ (invalid character in parent token id)\n");
			delete pMaster;
			pMaster = NULL;
			return false;
			}

	return true;
	}

//--------------------------------------------------------------------------------
bool DoMax(const char* pBuf, CTokenMaster*& pMaster)
	{
	if(pMaster == NULL || pBuf == NULL)
		{
		printf("syntax error ^^^^^^^ (no token declared)\n");
		return false;
		}

	if(! DoNumber(pBuf, pMaster->m_nMax))
		{
		printf("syntax error ^^^^^^^ (invalid character in max)\n");
		return false;
		}

	if(pMaster->m_nMax == 0)
		{
		printf("syntax error ^^^^^^^ (max cannot be zero)\n");
		return false;
		}

	if(pMaster->m_nMax > 0 && pMaster->m_nMax < pMaster->m_nCount)
		{
		printf("syntax error ^^^^^^^ (max must be >= count)\n");
		return false;
		}

	return true;
	}

//--------------------------------------------------------------------------------
bool DoBits(const char* pBuf, CTokenMaster*& pMaster)
	{
	if(pMaster == NULL || pBuf == NULL)
		{
		printf("syntax error ^^^^^^^ (no token declared)\n");
		return false;
		}

	char temp[4096];

	for(char* pCur; *pBuf;)
		{
		while(*pBuf && ! (isdigit(*pBuf) || isxdigit(*pBuf)))
			pBuf++;
		if(! *pBuf)
			break;
		pCur = temp;
		
		while(*pBuf && *pBuf != ',')
			{
			*pCur = *pBuf;
			pCur++;
			pBuf++;
			}

		*pCur = 0;
		if(temp[0] != 0)
			{
			unsigned long nWhich;
			const unsigned long nMax = CAccessToken::BitCount;
			if(DoNumber(temp, nWhich))
				{
				if(nWhich >= nMax)
					{
					printf("syntax error ^^^^^^^ (only bits 0-%ld allowed)\n", nMax - 1);
					return false;
					}

				printf("setting bit %d\n", nWhich);
				pMaster->m_token.SetBit(nWhich, true);
				}
			}
		}

	return true;
	}

//--------------------------------------------------------------------------------
bool DoName(const char* pBuf, CTokenMaster*& pMaster)
	{
	if(pMaster == NULL || pBuf == NULL)
		{
		printf("syntax error ^^^^^^^ (no token declared)\n");
		return false;
		}

	pMaster->m_token.m_pName = new char[strlen(pBuf) + 1];
	strcpy(pMaster->m_token.m_pName, pBuf);
	return true;
	}

//--------------------------------------------------------------------------------
bool DoExpires(const char* pBuf, CTokenMaster*& pMaster)
	{
	if(pBuf == NULL)
		{
		printf("error ^^^^^^^\n");
		return false;
		}

	// the input must be in the format:
	// expires=yXXXX mXX dXX
	char* pSrc = strchr(pBuf, 'm');
	if(pSrc == NULL)
		{
		printf("syntax error ^^^^^^^ (must specify month)\n");
		return false;
		}

	pSrc++;

	char sTemp[5];

	for(int i = 0; *pSrc && isdigit(*pSrc) && i < 2; i++, pSrc++)
		{
		sTemp[i] = *pSrc;
		}

	sTemp[i] = 0;

	struct tm tmTime;
	memset(&tmTime, 0, sizeof(struct tm));

	tmTime.tm_mon = atoi(sTemp) - 1;
	if(tmTime.tm_mon < 0 || tmTime.tm_mon > 11)
		{
		printf("syntax error ^^^^^^^ (month must be 1-12)\n");
		return false;
		}

	tmTime.tm_mday = 1;
	pSrc = strchr(pBuf, 'd');
	if(pSrc == NULL)
		printf("DEFAULT VALUE WARNING ^^^^^^^ (day not specified - defaults to 1)\n");
	else
		{
		pSrc++;
		for(i = 0; *pSrc && isdigit(*pSrc) && i < 2; i++, pSrc++)
			{
			sTemp[i] = *pSrc;
			}
		sTemp[i] = 0;
		tmTime.tm_mday = atoi(sTemp);
		}

	pSrc = strchr(pBuf, 'y');
	if(pSrc == NULL)
		{
		printf("syntax error ^^^^^^^ (must specify year)\n");
		return false;
		}

	pSrc++;
	for(i = 0; *pSrc && isdigit(*pSrc) && i < 4; i++, pSrc++)
		{
		sTemp[i] = *pSrc;
		}

	sTemp[i] = 0;

	tmTime.tm_year = atoi(sTemp) - 1900;

	g_nExpires = mktime(&tmTime);
	return g_nExpires > 0;
	}

//--------------------------------------------------------------------------------
bool DoExtra(const char* pBuf, CTokenMaster*& pMaster)
	{
	if(pBuf == NULL)
		{
		printf("error ^^^^^^^\n");
		return false;
		}

	if(pMaster->m_token.m_pExtra == NULL)
		pMaster->m_token.m_pExtra = new unsigned char[8192];

	char sTemp[32];
	for(;;)
		{
		if(*pBuf == 0)
			break;
		for(int i = 0; isdigit(*pBuf) && i < 31; i++, pBuf++)
			sTemp[i] = *pBuf;

		sTemp[i] = 0;
		pMaster->m_token.m_pExtra[pMaster->m_token.m_nExtraLen++] = atoi(sTemp);
		while(*pBuf && isspace(*pBuf))
			pBuf++;
		}

	return true;
	}

//--------------------------------------------------------------------------------
char* TrimString(char* pSrc)
	{
	while(*pSrc && isspace(*pSrc))
		pSrc++;

	if(! *pSrc)
		return pSrc;

	char* pEnd = pSrc + strlen(pSrc) - 1;
	while(pEnd > pSrc && isspace(*pEnd))
		{
		*pEnd = 0;
		pEnd--;
		}

	return pSrc;
	}

//--------------------------------------------------------------------------------
bool DoMonitor(const char* pBuf, CTokenMaster*& pMaster)
	{
	if(pBuf == NULL)
		{
		printf("error ^^^^^^^\n");
		return false;
		}

	char sTemp[256];
	strcpy(sTemp, pBuf);

	char* pUser = sTemp;
	char* pPwd = strchr(sTemp, ',');
	if(pPwd == NULL)
		{
		printf("error ^^^^^^^ must be username, password\n");
		return false;
		}

	*pPwd = 0;
	pPwd++;

	strcpy(g_pMonitorUser, TrimString(sTemp));
	strcpy(g_pMonitorPwd, TrimString(pPwd));
	return true;
	}

//--------------------------------------------------------------------------------
int Parse(CStdioFile& fIn, CFile& fOut, CFile& fOutBkup)
	{
	char temp[4096];
	char temp2[4096];
	char sName[512];
	char* pBuf;
	CTokenMaster* pMaster = NULL;

	sName[0] = 0;
	bool bRv = false;

	for(;;)
		{
		pBuf = temp;
		if(! fIn.ReadString(pBuf, 4096))
			break;
		while(isspace(*pBuf) && *pBuf)
			pBuf++;
		if(! *pBuf)
			continue;
		if(*pBuf == ';')
			continue;
		else
			{
			// remove all non esential characters from the string
			int i = 0;
			for(char* pBuf2 = pBuf; *pBuf2; pBuf2++)
				{
			//	if(isalnum(*pBuf2) || *pBuf2 == ',' || *pBuf2 == '=')
				if(isprint(*pBuf2))
					temp2[i++] = *pBuf2;
				}

			if(i == 0)
				continue;
			temp2[i] = 0;
			pBuf = temp2;
			}

		printf("%s\n", pBuf);
		char* pBreak = strchr(pBuf, '=');
		if(pBreak == NULL)
			{
			printf("syntax error ^^^^^^^ (= missing)\n");
			continue;
			}

		*pBreak = 0;

		if(*pBuf == 0)
			continue;

		LPCTSTR pKeywords[] = {"token", "count", "max", "bits", "name", "serial", "p3",
			"processor", "dongle", "email", "expires", "extra", "backup", "monitor", "parent" };
		enum { IDTOKEN, IDCOUNT, IDMAX, IDBITS, IDNAME, IDSERIAL, IDP3, IDPROCESSOR,
			IDDONGLE, IDEMAIL, IDEXPIRES, IDEXTRA, IDBACKUP, IDMONITOR, IDPARENT, NCOUNT };
		const int nCount = sizeof(pKeywords) / sizeof(LPCTSTR);

		assert(nCount == NCOUNT);

		for(int i = 0; i < nCount; i++)
			{
			if(strcmp(pBuf, pKeywords[i]) == 0)
				break;
			}

		switch(i)
			{
			case IDTOKEN:
				bRv = DoToken(pBreak+1, pMaster);
				break;
			case IDCOUNT:
				bRv = DoCount(pBreak+1, pMaster);
				break;
			case IDMAX:
				bRv = DoMax(pBreak+1, pMaster);
				break;
			case IDBITS:
				bRv = DoBits(pBreak+1, pMaster);
				break;
			case IDNAME:
				bRv = DoName(pBreak+1, pMaster);
				break;
			case IDSERIAL:
			case IDP3:
			case IDPROCESSOR:
				bRv = DoP3Serial(pBreak+1, pMaster);
				break;
			case IDDONGLE:
				bRv = DoDongle(pBreak+1, pMaster);
				break;
			case IDEMAIL:
				bRv = DoEmail(pBreak+1, pMaster);
				break;
			case IDEXPIRES:
				bRv = DoExpires(pBreak+1, pMaster);
				break;
			case IDEXTRA:
				bRv = DoExtra(pBreak+1, pMaster);
				break;
			case IDBACKUP:
				bRv = DoBackup(pBreak+1, pMaster);
				break;
			case IDMONITOR:
				bRv = DoMonitor(pBreak+1, pMaster);
				break;
			case IDPARENT:
				bRv = DoParent(pBreak+1, pMaster);
				break;
			default:
				printf("syntax error ^^^^^^^ (unknown keyword)\n");
				bRv = false;
				continue;
			}
		}

	if(! bRv)
		return -1;

	if(! WriteFile(fOut, fOutBkup))
		return -1;

	return 0;
	}
	
//--------------------------------------------------------------------------------
int GetPasswordFromConsole(char* pPass, int nMax, int nCode)
	{
	_cprintf("Password: ");
	char sTitle[256];
	::GetConsoleTitle(sTitle, 256);
	::SetConsoleTitle("TokenCompiler Password");
	int nCur = 0;
	for(;;)
		{
		while(! _kbhit())
			::Sleep(0);
		int c = _getch();
		switch(c)
			{
			case 0:
			case 0xe0:
				_getch();
				::Beep(1000, 10);
				break;

			case '\b':
				if(nCur)
					{
					nCur--;
					_putch('\b');
					_putch(' ');
					_putch('\b');
					}
				else
					::Beep(1000, 10);
				break;

			case '\r':
			case '\n':
				pPass[nCur] = 0;
				goto exit;

			default:
				if(! isalnum(c))
					{
					::Beep(1000, 10);
					continue;
					}

				pPass[nCur] = c + nCode;
				_putch('*');
				nCur++;
				if(nCur == nMax - 1)
					{
					pPass[nMax-1] = 0;
					::Beep(1000, 10);
					nCur = nMax;
					goto exit;
					}
				break;
			}
		}

exit:;
	::SetConsoleTitle(sTitle);
	_putch('\n');
	return nCur;
	}
