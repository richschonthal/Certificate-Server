/****************************************************************************/
/**                                                                        **/
/**                           Hardlock API-Calls                           **/
/**                                                                        **/
/**   This program is just an example for some functions to access the     **/
/**   application programing interface (API) for Hardlock. Feel free to    **/
/**   define your own functions.                                           **/
/**                                                                        **/
/**                  Aladdin Knowledge Systems, Germany                    **/
/**                                                                        **/
/**  Revision history                                                      **/
/**  ----------------                                                      **/
/**
***  $Log: hlapi_c.c $
***  Revision 1.49  2000/01/26 12:43:30  Henri
***  Option change in HLM_LOGIN.
***  
***  Revision 1.48  1999/11/28 00:05:43  chris
***  accesses to Data field of the API structure are now done
***  with the set_data_ptr macro
***  
***  Revision 1.47  1999/11/18 17:27:46  Henri
***  Added a FuncOpt to CHECKEXPDATE.
***
***  Revision 1.46  1999/10/18 09:47:36  chris
***  don't try FFS logins if VK isn't there
***
***  Revision 1.45  1999/10/07 11:28:56  Henri
***  Removed uneeded TLV defines.
***
***  Revision 1.44  1999/10/07 09:31:19  Henri
***  Restore values in HLM_LOGIN in error case.
***
***  Revision 1.43  1999/10/04 10:54:32  chris
***  more changes in HLM_CHECKEXPDATE
***
***  Revision 1.42  1999/09/30 15:34:57  chris
***  changes in HLM_CHECKEXPDATE
***
***  Revision 1.41  1999/09/28 15:29:29  chris
***  HLM_GETRUSINFO: return error if no buffer given and length != 0
***
***  Revision 1.40  1999/09/24 09:18:23  chris
***  do the same for HL_LMLOGIN...
***
***  Revision 1.39  1999/09/24 09:15:57  chris
***  clear API fields Data and Bcnt after usage in HLM_LOGIN
***
***  Revision 1.38  1999/09/20 11:28:52  chris
***  HLM_WRITELICENSE: new parameter Options
***
***  Revision 1.37  1999/09/17 13:26:13  chris
***  some fixes in HLM_WRITELICENSE; always include HLM_LOGIN (not only
***  if WITH_HLM is defined)
***
***  Revision 1.36  1999/09/15 17:35:33  Henri
***  Removed static from HLM_LOGIN.
***
***  Revision 1.35  1999/09/15 17:03:59  Henri
***  Changed WriteLicense.
***
***  Revision 1.34  1999/09/15 13:04:10  Henri
***  Return proper API status in Logout function.
***
***  Revision 1.33  1999/09/14 16:35:50  chris
***  changed parameters for HLM_GETRUSINFO and HLM_WRITELICENSE
***
***  Revision 1.32  1999/09/14 15:56:23  Henri
***  Bugfix in Logout
***
***  Revision 1.31  1999/09/13 11:46:49  Henri
***  Set API structure to zero in Logout function.
***
***  Revision 1.30  1999/09/01 15:06:43  Adi
***  Added special handling of global expiration date.
***
***  Revision 1.29  1999/08/27 11:17:53  Adi
***  HLM_LOGIN accepts also "" empty strings as environment.
***  HLM_LOGIN is refused if svk key pointer is NULL.
***
***  Revision 1.28  1999/08/12 10:39:03  chris
***  moved HLM_GETRUSINFO into LowAPI
***
***  Revision 1.27  1999/08/09 13:06:11  chris
***  removed superfluous '#include <stdio.h>'
***
***  Revision 1.26  1999/08/09 13:02:57  chris
***  changed HLM_LOGIN to always do a API_FFS_INIT call
***
***  Revision 1.25  1999/08/04 16:01:18  chris
***  HLM_WRITELICENSE
***
***  Revision 1.24  1999/07/28 16:01:35  chris
***  HLM_GETRUSINFO now includes the hw dependend info
***
***  Revision 1.23  1999/07/27 13:04:12  Henri
***  Fixed ISRUSHL.
***
***  Revision 1.22  1999/07/26 10:59:23  Henri
***  Added ISRUSHL function.
***
***  Revision 1.21  1999/07/22 11:16:50  chris
***  added WITH_HLM ifdef
***
***  Revision 1.20  1999/07/19 10:16:00  Henri
***  First RUS implementation.
***
***  Revision 1.19  1998/07/11 13:17:03  Henri
***  Bugfix in PORTINF.
***
***  Revision 1.18  1998/07/02 18:12:39  Henri
***  Return 0 in HL_PORTINF if key is USB.
***
***  Revision 1.17  1998/06/26 16:38:31  Henri
***  Set Port=0 in HL_LOGOUT, so it can be set using HL_SELECT.
***
***  Revision 1.16  1998/05/22 11:36:08  Henri
***  Fixed offest of HL_READID.
***
***  Revision 1.15  1998/05/08 14:12:07  Henri
***  Added defines for HL_READID.
***
***  Revision 1.14  1998/04/15 09:22:51  Henri
***  Redefined some variables (short -> Word) because of compiler warnings.
***
***  Revision 1.13  1998/04/08 10:02:40  chris
***  implemented HL_READ_ID() function
***
***  Revision 1.12  1997/02/12 13:44:46  henri
***  Changed return code handling for LM.
***
***  Revision 1.11  1997/02/11 20:16:34  chris
***  High-API now checks whether Low-API supports API_LMINIT function.
***
***  Revision 1.10  1997/02/11 18:32:29  chris
***  fixed error code returned in HL_LMLOGIN()
***
***  Revision 1.9  1997/02/06 14:12:58  henri
***  Added one character to sslen.
***
***  Revision 1.8  1997/02/03 18:10:01  henri
***  Renamed INVALID_MEM -> INVALID_LIC
***
***  Revision 1.7  1997/01/31 17:37:31  henri
***  Added first version of HL_LMLOGIN
***
***  Revision 1.6  1996/11/13 16:56:25  chris
***  adapted to UNIX32 define
***
***  Revision 1.5  1996/10/02 20:34:09  henri
***  Added GETSTRUC define because of wrong pointer init by old Power Fortran linker.
***
***  Revision 1.4  1996/09/18 11:52:14  henri
***  Support for multi API strutures.
***
***  Revision 1.3  1996/08/08 14:47:01  henri
***  Added VCS log.
**/
/****************************************************************************/

#ifdef  _MSC_VER
  #pragma warning (disable:4103)                /* no #pragma pack warnings */
#endif

#define GETSTRUC if (!api_struc) api_struc = &hl_struc

#include "hlapi_c.h"

static HL_API hl_struc = {{0}};                 /* Hardlock API structure   */
static HL_API DATAFAR_ *api_struc=0;            /* Pointer to API structure */

static Word CALL_API (HL_API DATAFAR_ *api_struc);

#ifdef __cplusplus
  extern "C" {
#endif

#ifdef UNIX32
  #define CENTRY
#endif

#ifdef WINNT
  #define CENTRY
  #ifdef __WATCOMC__
    #pragma aux cdecl "_*" parm caller [] value struct routine [eax] modify [eax ebx ecx edx]
    #pragma aux (cdecl) API_HL_CSTACK;
  #endif
#endif

#ifdef SALFORD
  #include <dbos\lib.h>
  #define CENTRY
#endif

#ifdef CENTRY
  #ifdef DLL_USE
    #ifdef WINNT
      extern Word __stdcall _API_DLL(HL_API DATAFAR_ *api_struc);
      #define API_Function _API_DLL
    #endif
  #else
    #ifdef WINNT
      extern Word FAR_ __cdecl API_HL_CSTACK(HL_API DATAFAR_ *api_struc);
    #else
      extern Word FAR_ API_HL_CSTACK(HL_API DATAFAR_ *api_struc);
    #endif
    #define API_Function API_HL_CSTACK
  #endif
#else
  #ifdef DLL_USE
    extern Word FAR_ pascal _API_DLL(HL_API DATAFAR_ *api_struc);
    #define API_Function _API_DLL
  #else
    #ifdef __HIGHC__
      extern Word _API_HL_CSTACK(HL_API DATAFAR_ *api_struc);
      #define API_Function _API_HL_CSTACK
    #else
      extern Word FAR_ pascal _API_HL_STACK(HL_API DATAFAR_ *api_struc);
      #define API_Function _API_HL_STACK
    #endif
  #endif
#endif

#ifdef __cplusplus
  }
#endif

#ifdef WITH_HLM

/* Date helper functions: */
/* ---------------------- */
static int isleap (unsigned yr)
{return yr % 400 == 0 || (yr % 4 == 0 && yr % 100 != 0);}

static unsigned months_to_days (unsigned month)
{return (month * 3057 - 3007) / 100;}

static unsigned long years_to_days (unsigned yr)
{return yr * 365L + yr / 4 - yr / 100 + yr / 400;}

#endif

/****************************************************************************/
static Word CALL_API(HL_API DATAFAR_ *api_struc)
/****************************************************************************/
{
Word result;

/* ----------------- */
/* We call the API : */
/* ----------------- */

#ifdef SALFORD
  set_io_permission(1);
#endif

result = API_Function(api_struc);

#ifdef SALFORD
  set_io_permission(0);
#endif

return (result);
}

/****************************************************************************/
RET_ FAR_ CALL_ HL_SELECT(HL_API DATAFAR_ *hl_ptr)
/****************************************************************************/
{
if (!hl_ptr)
  api_struc = &hl_struc;
 else
  api_struc = hl_ptr;

return (STATUS_OK);
}

/****************************************************************************/
RET_ FAR_ CALL_ HL_LOGOUT(void)
/****************************************************************************/
{
Word i, result;

GETSTRUC;
if (api_struc->Remote == NET_DEVICE)
  {
  api_struc->Function = API_LOGOUT;
  CALL_API(api_struc);
  }

api_struc->Function = API_DOWN;
CALL_API(api_struc);
if (api_struc->Status)
  {
  api_struc->Function = API_FORCE_DOWN;
  CALL_API(api_struc);
  }

result = api_struc->Status;

/* Set the API structure back to zero: */
/* ----------------------------------- */
for (i = 0; i < sizeof(HL_API); i++)
  ((char *) api_struc)[i] = 0;

return (result);
}

/****************************************************************************/
RET_ FAR_ CALL_ HL_PORTINF(void)
/****************************************************************************/
{
GETSTRUC;

if (api_struc->PortFlags & USB_DEVICE)
  return (0);

api_struc->Function = API_AVAIL;
CALL_API(api_struc);

if (api_struc->Status) return (0xffff);
return (api_struc->Port);
}

/****************************************************************************/
RET_ FAR_ CALL_ HL_USERINF(void)
/****************************************************************************/
{
GETSTRUC;

if (api_struc->Slot_ID == 0)
  api_struc->Function = API_INFO;
 else
  api_struc->Function = API_AVAIL;

api_struc->Application = 0;
CALL_API(api_struc);

if (api_struc->Status) return (0xffff);
return (api_struc->NetUsers);
}

/****************************************************************************/
RET_ FAR_ CALL_ HL_MAXUSER(void)
/****************************************************************************/
{
GETSTRUC;

if (api_struc->Slot_ID == 0)
  api_struc->Function = API_INFO;
 else
  api_struc->Function = API_AVAIL;

CALL_API(api_struc);

if (api_struc->Status) return (0xffff);
return (api_struc->MaxUsers);
}

/****************************************************************************/
RET_ FAR_ CALL_ HL_ACCINF(void)
/****************************************************************************/
{
GETSTRUC;
api_struc->Function = API_AVAIL;
CALL_API(api_struc);

if (api_struc->Status) return (0xffff);
return (api_struc->Remote);
}

/****************************************************************************/
RET_ FAR_ CALL_ HL_VERSION(void)
/****************************************************************************/
{
GETSTRUC;
api_struc->Function = API_AVAIL;
CALL_API(api_struc);

if (api_struc->Status) return (0xffff);
return ((api_struc->API_Version_ID[0] * 100) + api_struc->API_Version_ID[1]);
}

/****************************************************************************/
RET_ FAR_ CALL_ HL_HLSVERS(void)
/****************************************************************************/
{
Word result;

GETSTRUC;
if (api_struc->Remote == NET_DEVICE)
  {
  api_struc->Function  = API_AVAIL;
  result               = CALL_API(api_struc);
  if (result == STATUS_OK)
    return (api_struc->ShortLife);
  }
return 0;
}

/****************************************************************************/
RET_ FAR_ CALL_ HL_AVAIL(void)
/****************************************************************************/
{
GETSTRUC;

api_struc->Function = API_AVAIL;
CALL_API(api_struc);

if ((api_struc->Status == NO_DONGLE) && (api_struc->ShortLife == 1) && (api_struc->Remote == LOCAL_DEVICE))
  return(SELECT_DOWN);

return (api_struc->Status);
}

/****************************************************************************/
RET_ FAR_ CALL_ HL_CODE(void DATAFAR_ *Data, Word Cnt)
/****************************************************************************/
{
GETSTRUC;
set_data_ptr(api_struc,Data);
api_struc->Bcnt     = Cnt;
api_struc->Function = API_KEYE;
CALL_API(api_struc);
api_struc->Bcnt     = 0;

return (api_struc->Status);
}

/****************************************************************************/
RET_ FAR_ CALL_ HL_READ(Word Reg, Word DATAFAR_ *Value)
/****************************************************************************/
{
GETSTRUC;

if(Reg <= 63)
  {
  api_struc->Module.Eye.Reg = Reg;
  api_struc->Function       = API_READ;

  CALL_API(api_struc);
  if(api_struc->Status)
    return (api_struc->Status);

  *Value = api_struc->Module.Eye.Value;
  return (STATUS_OK);
  }

return (INVALID_PARAM);
}

#ifdef WITH_HLM

/****************************************************************************/
RET_ FAR_ CALL_ HLM_WRITELICENSE (Long BufLen, Byte DATAFAR_ * RTBBuffer, Word Access, Byte DATAFAR_ * SearchStr, Word Options)
/****************************************************************************/
{
Long   xlen;
HL_API local_struc;
Word   i, j, result;
Byte   SString[80];
Word   sslen = 0;
Word   blocks = 0;

if (BufLen > 0x7ffff)
  return (INVALID_PARAM); /* max. size possible */

/* Set the API structure back to zero: */
/* ----------------------------------- */
for (i = 0; i < sizeof(HL_API); i++)
  ((char *)&local_struc)[i] = 0;

/* Parse RTB block: */
/* ---------------- */
xlen = BufLen >> 3;
if (BufLen & 7) xlen++;
local_struc.Remote     = LOCAL_DEVICE;
local_struc.ShortLife  = (xlen << 3) - BufLen;
local_struc.Data       = RTBBuffer;
local_struc.Bcnt       = (Word)xlen;
local_struc.Function   = API_FFS_PARSERTB;
CALL_API(&local_struc);
if (local_struc.Status != STATUS_OK)
  return (local_struc.Status);

local_struc.Data       = 0;
local_struc.Bcnt       = 0;

/* --------------------------------- */
/* Check and prepare search string : */
/* --------------------------------- */
if (SearchStr != 0)
 if(SearchStr[0] != '\0')
  {
  /* Get length of search string : */
  for (sslen = 0; SearchStr[sslen] != '\0'; sslen++);

  /* Add one for zero : */
  sslen++;

  /* count number of 8 bytes blocks we get : */
  blocks = sslen / 8;
  if ((sslen % 8) != 0)
    blocks++;
  if (blocks > 10)
    return (INVALID_ENV);

  /* Copy search string to stack : */
  for (i = 0; i < sslen; i++)
     SString[i] = SearchStr[i];

  /* Add spaces to fill up last block : */
  for (j = 0; j < (blocks * 8 - sslen); j++)
     SString[i + j] = 0x20;

  /* Now put it into api structure : */
  local_struc.Bcnt = blocks;
  local_struc.Data = SString;
  }

/* --------------------------------------------- */
/* Call the INIT (search for Hardlock) function :*/
/* --------------------------------------------- */
local_struc.VendorKey = 0;
local_struc.Remote    = Access;
local_struc.Function  = API_FFS_INIT;
result = CALL_API(&local_struc);
local_struc.Data      = 0;
local_struc.Bcnt      = 0;
/* Old Low-API returns NOT_INIT error, if function not */
/* known and API structure hasn't been initialized yet */
if (result == NOT_INIT)
  return (VERSION_MISMATCH);
if (local_struc.Status != STATUS_OK)
  return (local_struc.Status);

/* ----------------------------- */
/* No further checks necessary : */
/* ----------------------------- */
if ((result == INVALID_ENV) || (result == CANNOT_OPEN_DRIVER))
  return (result);

/* ----------------------------- */
/* Check, if HL-Server is full : */
/* ----------------------------- */
if (result != STATUS_OK)
  {
  if ((Access & NET_DEVICE) == NET_DEVICE)
    {
    local_struc.Function = API_LOGIN_INFO;
    if (CALL_API(&local_struc) == HLS_FULL)
      result = TOO_MANY_USERS;
    }
  return (result);
  }

/* ------------------------------------------ */
/* Login to HL-Server if Hardlock is remote : */
/* ------------------------------------------ */
if (local_struc.Remote == NET_DEVICE)
  {
  local_struc.Function = API_LOGIN; /* don't do API_FFS_LOGIN, we don't have a VK! */
  result = CALL_API(&local_struc);
  if(result)
    {
    local_struc.Function = API_DOWN;
    CALL_API(&local_struc);
    }
  }

/* Now write it: */
/* ------------- */
xlen = BufLen >> 3;
if (BufLen & 7) xlen++;
local_struc.ShortLife   = (xlen << 3) - BufLen;
local_struc.Data        = RTBBuffer;
local_struc.Bcnt        = (Word)xlen;
local_struc.FuncOptions = 0; /* write mode */
if (Options & 1) local_struc.FuncOptions = 2; /* write mode with create flag */
local_struc.Function    = API_FFS_WRITE_LIC;
result = CALL_API(&local_struc);
local_struc.Bcnt        = 0;
local_struc.Data        = 0;
if (local_struc.Status != STATUS_OK) return (local_struc.Status);

xlen = BufLen >> 3;
if (BufLen & 7) xlen++;
local_struc.ShortLife   = (xlen << 3) - BufLen;
local_struc.Data        = RTBBuffer;
local_struc.Bcnt        = (Word)xlen;
local_struc.FuncOptions = 1; /* finish mode */
local_struc.Function    = API_FFS_WRITE_LIC;
result = CALL_API(&local_struc);
local_struc.Bcnt        = 0;
local_struc.ShortLife   = 0;
local_struc.FuncOptions = 0;
local_struc.Data        = 0;

/* Down and Logout: */
/* ---------------- */
if (local_struc.Remote == NET_DEVICE)
  {
  local_struc.Function = API_LOGOUT;
  CALL_API(&local_struc);
  }

local_struc.Function = API_DOWN;
CALL_API(&local_struc);
if (local_struc.Status)
  {
  local_struc.Function = API_FORCE_DOWN;
  CALL_API(&local_struc);
  }

return (result);
}

/****************************************************************************/
RET_ FAR_ CALL_ HLM_GETRUSINFO (Long * BufLen, Byte DATAFAR_ * RTBBuffer, Word Base64)
/****************************************************************************/
{
GETSTRUC;

if (*BufLen > 0x7ffff) return(INVALID_PARAM);
if (RTBBuffer == 0 && *BufLen != 0) return(INVALID_PARAM);

api_struc->ShortLife   = 0;
api_struc->Data        = RTBBuffer;
api_struc->Bcnt        = *BufLen >> 3;
api_struc->Function    = API_FFS_GETRUSINFO;
api_struc->FuncOptions = Base64 ? 1 : 0;
CALL_API(api_struc);

if (api_struc->Status != STATUS_OK)
  {
  if (api_struc->Status == BUFFER_TOO_SMALL)
    {
    *BufLen = ((Long)api_struc->ShortLife) << 3;          /* return needed buffer length */
    }
  api_struc->Bcnt        = 0;
  api_struc->FuncOptions = 0;
  api_struc->ShortLife   = 0;
  return (api_struc->Status);
  }
*BufLen = (api_struc->Bcnt << 3) - api_struc->ShortLife; /* return correct length in bytes */
api_struc->Bcnt        = 0;
api_struc->FuncOptions = 0;
api_struc->ShortLife   = 0;
return (STATUS_OK);
}

#endif /* #ifdef WITH_HLM */

/****************************************************************************/
RET_ FAR_ CALL_ HL_READID(Word DATAFAR_ *IDLow, Word DATAFAR_ *IDHigh)
/****************************************************************************/
{
GETSTRUC;

if (!(api_struc->PortFlags & USB_DEVICE))
  return (NO_SERIALID);

api_struc->Module.Eye.Reg = 14;
api_struc->Function       = API_READ_ID;

CALL_API(api_struc);
if(api_struc->Status)
  return (api_struc->Status);

*IDLow  = api_struc->Module.Eye.Value;
*IDHigh = api_struc->Module.Eye.Reg;

if ((*IDLow == 0x0000) && (*IDHigh == 0x0000))
  return (NO_SERIALID);

return (STATUS_OK);
}

/****************************************************************************/
RET_ FAR_ CALL_ HL_READBL(Byte DATAFAR_ * Eeprom)
/****************************************************************************/
{
GETSTRUC;
api_struc->Bcnt     = 16;
set_data_ptr(api_struc,Eeprom);
api_struc->Function = API_READ_BLOCK;
CALL_API(api_struc);
api_struc->Bcnt     = 0;

return (api_struc->Status);
}

/****************************************************************************/
RET_ FAR_ CALL_ HL_WRITEBL(Byte DATAFAR_ * Eeprom)
/****************************************************************************/
{
GETSTRUC;
api_struc->Bcnt     = 4;
set_data_ptr(api_struc,Eeprom);
api_struc->Function = API_WRITE_BLOCK;
CALL_API(api_struc);
api_struc->Bcnt     = 0;

return (api_struc->Status);
}

/****************************************************************************/
RET_ FAR_ CALL_ HL_ABORT(void)
/****************************************************************************/
{
GETSTRUC;
api_struc->Function = API_LOGOUT;
CALL_API(api_struc);
api_struc->Function = API_ABORT;
CALL_API(api_struc);

return (api_struc->Status);
}

/****************************************************************************/
RET_ FAR_ CALL_ HL_WRITE(Word Reg, Word Value)
/****************************************************************************/
{
GETSTRUC;
if(Reg >= 48 && Reg <= 63)
  {
  api_struc->Module.Eye.Reg   = Reg;
  api_struc->Module.Eye.Value = Value;
  api_struc->Function         = API_WRITE;
  CALL_API(api_struc);
  return (api_struc->Status);
  }

return (INVALID_PARAM);
}

/****************************************************************************/
RET_ FAR_ CALL_ HL_MEMINF(void)
/****************************************************************************/
{
Word newvalue, oldvalue;
Word TestMem = 0;
Byte Memory[128];
int i;

/* -------------------------- */
/* Read memory in one block : */
/* -------------------------- */
GETSTRUC;
api_struc->Bcnt     = 16;
set_data_ptr(api_struc,Memory);
api_struc->Function = API_READ_BLOCK;
CALL_API(api_struc);
api_struc->Bcnt     = 0;
if (api_struc->Status != STATUS_OK)
  return (NO_ACCESS);

/* -------------------------------------- */
/* Check, if every value is zero or one : */
/* -------------------------------------- */
for (i = 0; i < 128; i++)
  TestMem |= Memory[i];
if (TestMem != 0)
  {
  for (i = 0; i < 128; i++)
    {
    if (Memory[i] != 0xff)
      return (STATUS_OK);
    }
  }

/* ---------------------- */
/* Save memory contents : */
/* ---------------------- */
api_struc->Module.Eye.Reg = 48;
api_struc->Function       = API_READ;
CALL_API(api_struc);
if(api_struc->Status) return (NO_ACCESS);
oldvalue                 = api_struc->Module.Eye.Value;

/* ---------------------------------------------------------------- */
/* XOR of the read value to exclude random returns from interface : */
/* ---------------------------------------------------------------- */
newvalue = oldvalue ^ 0x0d0e;

/* ------------------------ */
/* Write new memory value : */
/* ------------------------ */
api_struc->Module.Eye.Value = newvalue;
api_struc->Function         = API_WRITE;
CALL_API(api_struc);
if(api_struc->Status)
 {
 api_struc->Module.Eye.Value = oldvalue;
 api_struc->Function         = API_WRITE;
 CALL_API(api_struc);
 return(NO_ACCESS);
 }

/* ------------------------- */
/* Read and compare memory : */
/* ------------------------- */
api_struc->Function = API_READ;
CALL_API(api_struc);
if(api_struc->Status)
 {
 api_struc->Module.Eye.Value = oldvalue;
 api_struc->Function         = API_WRITE;
 CALL_API(api_struc);
 return(NO_ACCESS);
 }
if (api_struc->Module.Eye.Value != newvalue)
 {
 api_struc->Module.Eye.Value = oldvalue;
 api_struc->Function         = API_WRITE;
 CALL_API(api_struc);
 return(NO_ACCESS);
 }

/* ------------------ */
/* Write old memory : */
/* ------------------ */
api_struc->Module.Eye.Value = oldvalue;
api_struc->Function         = API_WRITE;
CALL_API(api_struc);
if(api_struc->Status)
 {
 api_struc->Module.Eye.Value = oldvalue;
 api_struc->Function         = API_WRITE;
 CALL_API(api_struc);
 return(NO_ACCESS);
 }

return (STATUS_OK);
}

#ifdef WITH_HLM

/****************************************************************************/
RET_ FAR_ CALL_ HLM_OCCUPYSLOT (Long Slot)
/****************************************************************************/
{
GETSTRUC;

api_struc->Bcnt         = 0;
api_struc->Slot_ID      = (Word) (0xffff & Slot);
api_struc->Slot_ID_HIGH = (Word) (Slot >> 16);
api_struc->Function     = API_FFS_OCCUPY_SLOT;
CALL_API(api_struc);

return (api_struc->Status);
}

/****************************************************************************/
RET_ FAR_ CALL_ HLM_FREESLOT (Long Slot)
/****************************************************************************/
{
GETSTRUC;

api_struc->Slot_ID      = (Word) (0xffff & Slot);
api_struc->Slot_ID_HIGH = (Word) (Slot >> 16);
api_struc->Function     = API_FFS_FREE_SLOT;
CALL_API(api_struc);

return (api_struc->Status);
}

/****************************************************************************/
RET_ FAR_ CALL_ HLM_CHECKSLOT (Long Slot, Long * MaxUser, Long * CurrentUser)
/****************************************************************************/
{
Word result;

GETSTRUC;

api_struc->Slot_ID      = (Word) (0xffff & Slot);
api_struc->Slot_ID_HIGH = (Word) (Slot >> 16);
api_struc->Function     = API_FFS_QUERY_SLOT;
result = CALL_API(api_struc);
if (result != STATUS_OK)
  return (api_struc->Status);

*MaxUser     = api_struc->RUS_MaxInfo;
*CurrentUser = api_struc->RUS_CurInfo;

return (STATUS_OK);
}

/****************************************************************************/
RET_ FAR_ CALL_ HLM_CHECKEXPDATE (Long Slot, Word * Year, Word * Month, Word * Day)
/****************************************************************************/
{
Word result;
Long scalar;
Word n;

GETSTRUC;

/* preinitialize result */
*Month = *Day = *Year = 0;

api_struc->Slot_ID      = (Word) (0xffff & Slot);
api_struc->Slot_ID_HIGH = (Word) (Slot >> 16);
api_struc->RUS_ExpDate  = 0;
api_struc->Function     = API_FFS_QUERY_SLOT;
api_struc->FuncOptions  = 1;
result = CALL_API(api_struc);
api_struc->FuncOptions  = 0;
if (result != STATUS_OK && result != RUS_INVALID_SLOT)
 return (api_struc->Status);

if (result == RUS_INVALID_SLOT || !api_struc->RUS_ExpDate)
 {
 return (STATUS_OK);
 }

scalar = api_struc->RUS_ExpDate + 730120 - 365;

for (n = (unsigned)((scalar * 400L) / 146097L); years_to_days(n) < scalar; n++);

*Year = n;
n = (unsigned)(scalar - years_to_days(n-1));
if (n > 59)
  {
  n += 2;
  if (isleap(*Year))
    n -= n > 62 ? 1 : 2;
  }
*Month = (n * 100 + 3007) / 3057;
*Day = n - months_to_days(*Month);

if (api_struc->RUS_ExpDate < api_struc->ShortLife)
 result = RUS_DATE_EXPIRED;
api_struc->ShortLife=0;
api_struc->RUS_ExpDate=0;

return (result);
}

/****************************************************************************/
RET_ FAR_ CALL_ HLM_ISRUSHL (Long * ID)
/****************************************************************************/
{
Word result;
Byte MemBuff[128];
RUS_FIB  *myFIB = (RUS_FIB *) MemBuff;

GETSTRUC;

result = HL_READBL(MemBuff);
if (result != STATUS_OK)
  return (api_struc->Status);

api_struc->Bcnt     = 16;
api_struc->Data     = MemBuff;
api_struc->Function = API_FFS_ISRUSHL;
result = CALL_API(api_struc);
api_struc->Bcnt     = 0;
if (result != STATUS_OK)
  return (api_struc->Status);

*ID = myFIB->SERIAL_ID;

return (STATUS_OK);
}

/****************************************************************************/
RET_ FAR_ CALL_ HLM_CHECKCOUNTER (Word IncVal, Long * MaxCounter, Long * CurrentCounter)
/****************************************************************************/
{
Word result;

GETSTRUC;

api_struc->ShortLife   = IncVal;
api_struc->Function    = API_FFS_INC_CNTR;
result = CALL_API(api_struc);
if (result != STATUS_OK)
  return (api_struc->Status);

*CurrentCounter = api_struc->RUS_CurInfo;
*MaxCounter     = api_struc->RUS_MaxInfo;

return (STATUS_OK);
}

#endif /* #ifdef WITH_HLM */

/****************************************************************************/
RET_ FAR_ CALL_ HLM_LOGIN (Word ModAd, Word Access, Byte DATAFAR_ * RefKey, Byte DATAFAR_ * VerKey, Byte DATAFAR_ * VKey, Long RUSOptions, Byte DATAFAR_ * SearchStr)
/****************************************************************************/
{
Word  result;
Long  OldTimerID;
Word  OldBcnt, OldRemote, OldPort;
Word  i, j;
Byte  SString[80];
Word  sslen = 0;
Word  blocks = 0;

GETSTRUC;
OldRemote  = api_struc->Remote;
OldTimerID = api_struc->Task_ID;
OldPort    = api_struc->Port;
OldBcnt    = api_struc->Bcnt;

api_struc->ModID            = EYE_DONGLE;
api_struc->Remote           = Access;
api_struc->Module.Eye.ModAd = ModAd;
api_struc->Bcnt             = 0;

/* --------------------- */
/* Get TaskID from API : */
/* --------------------- */
#ifndef DOS386
if (api_struc->Task_ID == 0)
  {
  api_struc->Function = API_GET_TASKID;
  CALL_API(api_struc);
  }
#endif

api_struc->Protocol = 0;
api_struc->Timeout  = 0;
api_struc->PM_Host  = API_XTD_DETECT;

/* -------------------------------------------- */
/* We generated a verify key with TESTAPI.EXE : */
/* -------------------------------------------- */
for (i = 0; i < 8; i++)
  {
  api_struc->ID_Ref[i]    = RefKey[i];
  api_struc->ID_Verify[i] = VerKey[i];
  }

/* ------------------------- */
/* Don't check for HL_SEARCH */
/* ------------------------- */
#ifdef NO_ENVCHK
api_struc->EnvMask |= IGNORE_ENVIRONMENT;
#endif

/* --------------------------------- */
/* Check and prepare search string : */
/* --------------------------------- */
if (SearchStr != 0)
 if(SearchStr[0] != '\0')
  {
  /* Get length of search string : */
  for (sslen = 0; SearchStr[sslen] != '\0'; sslen++);

  /* Add one for zero : */
  sslen++;

  /* count number of 8 bytes blocks we get : */
  blocks = sslen / 8;
  if ((sslen % 8) != 0)
    blocks++;
  if (blocks > 10)
        {
    api_struc->Remote  = OldRemote;
    api_struc->Task_ID = OldTimerID;
    api_struc->Port    = OldPort;
    api_struc->Bcnt    = OldBcnt;
    return (INVALID_ENV);
        }

  /* Copy search string to stack : */
  for (i = 0; i < sslen; i++)
     SString[i] = SearchStr[i];

  /* Add spaces to fill up last block : */
  for (j = 0; j < (blocks * 8 - sslen); j++)
     SString[i + j] = 0x20;

  /* Now put it into api structure : */
  api_struc->Bcnt = blocks;
  set_data_ptr(api_struc,SString);
  }

/* --------------------------------------------- */
/* Call the INIT (search for Hardlock) function :*/
/* --------------------------------------------- */
api_struc->FuncOptions = (RUSOptions >> 1) & 1;
if ((VKey == 0) && (!(RUSOptions & FORCE_RUS)))
  {
  api_struc->Function = API_INIT;
  result = CALL_API(api_struc);
  api_struc->FuncOptions = 0;
  api_struc->Bcnt = 0;
  set_data_ptr(api_struc,0);
  }
 else
  {
  api_struc->VendorKey = VKey;
  api_struc->Function = API_FFS_INIT;
  result = CALL_API(api_struc);
  /* Old Low-API returns NOT_INIT error, if function not */
  /* known and API structure hasn't been initialized yet */
  api_struc->FuncOptions = 0;
  api_struc->Bcnt = 0;
  set_data_ptr(api_struc,0);
  if (result == NOT_INIT)
    {
    api_struc->Remote  = OldRemote;
    api_struc->Task_ID = OldTimerID;
    api_struc->Port    = OldPort;
    api_struc->Bcnt    = OldBcnt;
    return (VERSION_MISMATCH);
    }
  }

if (result == ALREADY_INIT)
  {
  api_struc->Remote  = OldRemote;
  api_struc->Task_ID = OldTimerID;
  api_struc->Port    = OldPort;
  api_struc->Bcnt    = OldBcnt;
  return (result);
  }

/* ----------------------------- */
/* No further checks necessary : */
/* ----------------------------- */
if ((result == INVALID_ENV) || (result == CANNOT_OPEN_DRIVER))
  {
  api_struc->Remote  = OldRemote;
  api_struc->Task_ID = OldTimerID;
  api_struc->Port    = OldPort;
  api_struc->Bcnt    = OldBcnt;
  return (result);
  }

/* ----------------------------- */
/* Check, if HL-Server is full : */
/* ----------------------------- */
if (result != STATUS_OK)
  {
  if ((Access & NET_DEVICE) == NET_DEVICE)
    {
    api_struc->Function = API_LOGIN_INFO;
    if (CALL_API(api_struc) == HLS_FULL)
      result = TOO_MANY_USERS;
    }

  api_struc->Remote  = OldRemote;
  api_struc->Task_ID = OldTimerID;
  api_struc->Port    = OldPort;
  api_struc->Bcnt    = OldBcnt;
  return (result);
  }

/* ------------------------------------------ */
/* Login to HL-Server if Hardlock is remote : */
/* ------------------------------------------ */
if (api_struc->Remote == NET_DEVICE)
  {
  if ((VKey == 0)) /* && (!(RUSOptions & FORCE_RUS)))*/
    api_struc->Function = API_LOGIN;
   else
    api_struc->Function = API_FFS_LOGIN;

  result = CALL_API(api_struc);
  if(result)
    {
    api_struc->Function = API_DOWN;
    CALL_API(api_struc);
    }
  }

if (result != STATUS_OK)
  {
  api_struc->Remote  = OldRemote;
  api_struc->Task_ID = OldTimerID;
  api_struc->Port    = OldPort;
  api_struc->Bcnt    = OldBcnt;
  }

return (result);
}

/****************************************************************************/
RET_ FAR_ CALL_ HL_LOGIN(Word ModAd, Word Access, Byte DATAFAR_ * RefKey, Byte DATAFAR_ * VerKey)
/****************************************************************************/
{
/* Just call the new function...; */
return HLM_LOGIN(ModAd, Access, RefKey, VerKey, 0, 0, 0);
}

/****************************************************************************/
RET_ FAR_ CALL_ HL_LMLOGIN(Word ModAd, Word Access, Byte DATAFAR_ * RefKey, Byte DATAFAR_ * VerKey, Word Slot_ID, Byte DATAFAR_ * SearchStr)
/****************************************************************************/
{
Word  result;
Long  OldTimerID;
Word  OldSlot_ID, OldBcnt, OldRemote, OldPort, OldResult;
Word  i, j;
Byte  SString[80];
Word  sslen = 0;
Word  blocks = 0;

GETSTRUC;
OldRemote  = api_struc->Remote;
OldTimerID = api_struc->Task_ID;
OldPort    = api_struc->Port;
OldSlot_ID = api_struc->Slot_ID;
OldBcnt    = api_struc->Bcnt;

api_struc->ModID            = EYE_DONGLE;
api_struc->Remote           = Access;
api_struc->Slot_ID          = Slot_ID;
api_struc->Module.Eye.ModAd = ModAd;

/* --------------------- */
/* Get TaskID from API : */
/* --------------------- */
#ifndef DOS386
if (api_struc->Task_ID == 0)
  {
  api_struc->Function = API_GET_TASKID;
  CALL_API(api_struc);
  }
#endif

api_struc->Protocol = 0;
api_struc->Timeout  = 0;
api_struc->PM_Host  = API_XTD_DETECT;

/* -------------------------------------------- */
/* We generated a verify key with TESTAPI.EXE : */
/* -------------------------------------------- */
for (i = 0; i < 8; i++)
  {
  api_struc->ID_Ref[i]    = RefKey[i];
  api_struc->ID_Verify[i] = VerKey[i];
  }

/* ------------------------- */
/* Don't check for HL_SEARCH */
/* ------------------------- */
#ifdef NO_ENVCHK
api_struc->EnvMask |= IGNORE_ENVIRONMENT;
#endif

/* --------------------------------- */
/* Check and prepare search string : */
/* --------------------------------- */
if (SearchStr != 0)
  {
  /* Get length of search string : */
  for (sslen = 0; SearchStr[sslen] != '\0'; sslen++);

  /* Add one for zero : */
  sslen++;

  /* count number of 8 bytes blocks we get : */
  blocks = sslen / 8;
  if ((sslen % 8) != 0)
    blocks++;
  if (blocks > 10)
    return (INVALID_ENV);

  /* Copy search string to stack : */
  for (i = 0; i < sslen; i++)
     SString[i] = SearchStr[i];

  /* Add spaces to fill up last block : */
  for (j = 0; j < (blocks * 8 - sslen); j++)
     SString[i + j] = 0x20;

  /* Now put it into api structure : */
  api_struc->Bcnt = blocks;
  set_data_ptr(api_struc,SString);
  }

/* --------------------------------------------- */
/* Call the INIT (search for Hardlock) function :*/
/* --------------------------------------------- */
if (Slot_ID == 0)
  {
  api_struc->Function = API_INIT;
  result = CALL_API(api_struc);
  api_struc->Bcnt = 0;
  set_data_ptr(api_struc,0);
  }
 else
  {
  api_struc->Function = API_LMINIT;
  result = CALL_API(api_struc);
  api_struc->Bcnt = 0;
  set_data_ptr(api_struc,0);
  /* Old Low-API returns NOT_INIT error, if function not */
  /* known and API structure hasn't been initialized yet */
  if (result == NOT_INIT)
    return(VERSION_MISMATCH);
  }

if (result == ALREADY_INIT)
  {
  api_struc->Remote  = OldRemote;
  api_struc->Task_ID = OldTimerID;
  api_struc->Port    = OldPort;
  api_struc->Bcnt    = OldBcnt;
  api_struc->Slot_ID = OldSlot_ID;
  return (result);
  }

/* ----------------------------- */
/* No further checks necessary : */
/* ----------------------------- */
if ((result == INVALID_ENV) || (result == CANNOT_OPEN_DRIVER))
  return (result);

/* ----------------------------- */
/* Check, if HL-Server is full : */
/* ----------------------------- */
if (result != STATUS_OK)
  {
  if ((Access & NET_DEVICE) == NET_DEVICE)
    {
    if (Slot_ID == 0)
      {
      /* Check the old way... : */
      /* ---------------------- */
      api_struc->Function = API_LOGIN_INFO;
      if (CALL_API(api_struc) == HLS_FULL)
        return(TOO_MANY_USERS);
      }
     else
      {
      /* Check using LM functions... : */
      /* ----------------------------- */
      OldResult = result;
      api_struc->Function = API_LMPING;
      result = CALL_API(api_struc);

      if (result == HLS_FULL)
        return(TOO_MANY_USERS);

      if ((result == NO_LICENSE) || (OldResult == NO_LICENSE))
        return(NO_LICENSE);

      if (OldResult == INVALID_LIC)
        return(OldResult);

      if (result == NO_DONGLE)
        return(OldResult);

      return (result);
      }
    }
  return (result);
  }

/* ------------------------------------------ */
/* Login to HL-Server if Hardlock is remote : */
/* ------------------------------------------ */
if (api_struc->Remote == NET_DEVICE)
  {
  api_struc->Function = API_LOGIN;
  result              = CALL_API(api_struc);
  if(result)
    {
    api_struc->Function = API_DOWN;
    CALL_API(api_struc);
    }
  }

return (result);
}

/****************************************************************************/
/****************************************************************************/
/* The following functions map the old Hardlock Calls on the new API. These */
/* functions are defined only for compatibility reasons.                    */
/* !!! Don't mix old and new functions. Don't use if it is not necessary.!!!*/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void FAR_ CALL_ INT_ON (void)
/****************************************************************************/
{return;}

/****************************************************************************/
void FAR_ CALL_ INT_OFF (void)
/****************************************************************************/
{return;}

/****************************************************************************/
void FAR_ CALL_ HL_ON (Word Port, Word ModAd)
/****************************************************************************/
{
GETSTRUC;
api_struc->ModID            = EYE_DONGLE;
api_struc->Remote           = LOCAL_DEVICE;
api_struc->Module.Eye.ModAd = ModAd;
api_struc->Port             = Port;
api_struc->Function         = API_INIT;
CALL_API(api_struc);
if(api_struc->Status) return;

api_struc->Function = API_LOGIN;
CALL_API(api_struc);
if(api_struc->Status)
 {
 api_struc->Function = API_DOWN;
 CALL_API(api_struc);
 }

return;
}

/****************************************************************************/
void FAR_ CALL_ HL_OFF (Word Port)
/****************************************************************************/
{
GETSTRUC;
api_struc->Port     = Port;
api_struc->Function = API_LOGOUT;
CALL_API(api_struc);

api_struc->Function = API_DOWN;
CALL_API(api_struc);
if (api_struc->Status)
  {
  api_struc->Function = API_FORCE_DOWN;
  CALL_API(api_struc);
  }
return;
}

/****************************************************************************/
Word FAR_ CALL_ K_EYE (Word Port, char  DATAFAR_ *Inp, Word BlkCnt)
/****************************************************************************/
{
GETSTRUC;
api_struc->Port     = Port;
set_data_ptr(api_struc,Inp);
api_struc->Bcnt     = BlkCnt;
api_struc->Function = API_KEYE;
CALL_API(api_struc);
api_struc->Bcnt     = 0;

if (api_struc->Status)
  return (0);
return (1);
}

/****************************************************************************/
void FAR_ CALL_ HL_WR (Word Port, Word Reg, Word Val)
/****************************************************************************/
{
GETSTRUC;
if(Reg >= 48 && Reg <= 63)
  {
  api_struc->Port             = Port;
  api_struc->Module.Eye.Reg   = Reg;
  api_struc->Module.Eye.Value = Val;
  api_struc->Function         = API_WRITE;
  CALL_API(api_struc);
  }
return;
}

/****************************************************************************/
Word FAR_ CALL_ HL_RD (Word Port, Word Reg)
/****************************************************************************/
{
GETSTRUC;
if(Reg <= 63)
  {
  api_struc->Port           = Port;
  api_struc->Module.Eye.Reg = Reg;
  api_struc->Function       = API_READ;
  CALL_API(api_struc);
  if (api_struc->Status == STATUS_OK)
    return (api_struc->Module.Eye.Value);
  }

return (0);
}

/****************************************************************************/
RET_ FAR_ CALL_ HL_CALC (Word i1, Word i2, Word i3, Word i4)
/****************************************************************************/
{
unsigned short Shift1, Shift2;

GETSTRUC;
api_struc->Function = 16;   /* Function: query Hardlock in compatible mode */

Shift1 = i1 | (i2 << 8);
Shift2 = i3 | (i4 << 8);
api_struc->Timeout = (Long) Shift1 | (Long) (Shift2 * 0x10000L);

CALL_API(api_struc);
if(api_struc->Status)
  return 0;
 else
  return api_struc->ShortLife;
}


