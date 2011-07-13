//--------------------------------------------------------------------------------
//
// Copyright (c) 2000 MarkCare Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _CERTIFICATE_H_
#define _CERTIFICATE_H_

#include <time.h>

#ifdef	UNIX

typedef long time_t;
typedef int  BOOL;

#define TRUE 1
#define FALSE 0

#endif // UNIX

#ifdef WIN32
#endif

// 1 to 5 minutes
#define MIN_CERT_LIFE 60
#define MAX_CERT_LIFE 300

//--------------------------------------------------------------------------------
class CAccessToken;
class CAccessTokenMap;

//--------------------------------------------------------------------------------
namespace CCertificateCommands
	{
	enum
		{
		MASK_CMD = 0x0000ffff,
		MASK_PARAM = 0xffff0000,

		CMD_NEW = 0x0001,
		CMD_REFRESH,
		CMD_RELEASE,
		CMD_BACKUPCONNECT,

		INFO_OK = 0x0010,
		INFO_REVOKED,
		INFO_EXPIRED,
		INFO_SERVER_SHUTTING_DOWN,

		ERR_NO_SUCH_TOKEN = 0x0100,
		ERR_NO_SUCH_CERT,
		ERR_TOKEN_NOT_AVAIL,

		RESERVED1
		};
	};

//--------------------------------------------------------------------------------
class CCertificate
	{
	public:
		CCertificate();
		CCertificate(const CCertificate&);
		CCertificate(ULONG, ULONG, ULONG);
		virtual ~CCertificate();

	// data items that are sent/received from server
	protected:
		// used internally on the server side - m_bLocalTokenAlloc is not encoded or transmitted
		// when true, the certificate deletes the tokens - on the server, the db system deletes the tokens
		bool m_bLocalTokenAlloc;

		ULONG m_nCmd;
		ULONG m_nMagicNumber;
		ULONG m_nId;
		ULONG m_nUserClass;
		time_t m_tmIssued;
		time_t m_nSecondsToLive;
		CAccessTokenMap* m_pTokenMap;

	public:
		UINT GetCommand() const;
		void SetCommand(UINT);
		void SetCommand(UINT, UINT);

		UINT GetCommandParam() const;
		void SetCommandParam(UINT);

		ULONG GetMagicNumber() const;

		int Encode(char*, bool bIncludeToken = true);
		void Decode(const char*, int);

		operator=(const CCertificate&);
		bool operator==(const CCertificate&);

		CAccessToken* GetToken(ULONG);
		bool HasToken(ULONG);

		CAccessTokenMap* GetTokenMap() const;

		ULONG GetId() const;
		ULONG GetUserClass() const;

		time_t GetTimeIssued() const;
		time_t GetSecondsToLive() const;

		bool IsExpired();

		virtual bool OnExpired();
		virtual bool OnRevoked();
	};

#endif // _CERTIFICATE_H_
