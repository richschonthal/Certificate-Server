/* $Id: hlapi_c.h 1.14 1999/11/17 10:41:57 Henri Exp $ */

#include "fastapi.h"

/* --------------------- */
/* Function prototypes : */
/* --------------------- */
#ifdef __cplusplus
  extern "C" {
#endif

/* ------------------ */
/* Basic Hardlock API */
/* ------------------ */
RET_ FAR_ CALL_ HL_LOGIN     (Word ModAd, Word Access, Byte DATAFAR_ *RefKey, Byte DATAFAR_ *VerKey);
RET_ FAR_ CALL_ HL_LOGOUT    (void);
RET_ FAR_ CALL_ HL_AVAIL     (void);
RET_ FAR_ CALL_ HL_PORTINF   (void);
RET_ FAR_ CALL_ HL_ACCINF    (void);
RET_ FAR_ CALL_ HL_USERINF   (void);
RET_ FAR_ CALL_ HL_MAXUSER   (void);
RET_ FAR_ CALL_ HL_MEMINF    (void);
RET_ FAR_ CALL_ HL_CODE      (void DATAFAR_ *Data, Word Count);
RET_ FAR_ CALL_ HL_WRITE     (Word Reg, Word Value);
RET_ FAR_ CALL_ HL_READ      (Word Reg, Word DATAFAR_ *Value);
RET_ FAR_ CALL_ HL_READBL    (Byte DATAFAR_ *Eeprom);
RET_ FAR_ CALL_ HL_WRITEBL   (Byte DATAFAR_ *Eeprom);
RET_ FAR_ CALL_ HL_ABORT     (void);
RET_ FAR_ CALL_ HL_VERSION   (void);
RET_ FAR_ CALL_ HL_HLSVERS   (void);
RET_ FAR_ CALL_ HL_SELECT    (HL_API DATAFAR_ *hl_ptr);
RET_ FAR_ CALL_ HL_READID    (Word DATAFAR_ *IDLow, Word DATAFAR_ *IDHigh);

/* ---------------- */
/* Hardlock RUS API */
/* ---------------- */
RET_ FAR_ CALL_ HLM_LOGIN         (Word ModAd, Word Access, Byte DATAFAR_ *RefKey, Byte DATAFAR_ *VerKey, Byte DATAFAR_ * VKey, Long RUSOptions, Byte DATAFAR_ * SearchStr);
RET_ FAR_ CALL_ HLM_OCCUPYSLOT    (Long Slot);
RET_ FAR_ CALL_ HLM_FREESLOT      (Long Slot);
RET_ FAR_ CALL_ HLM_CHECKSLOT     (Long Slot, Long * MaxUser, Long * CurrentUser);
RET_ FAR_ CALL_ HLM_CHECKCOUNTER  (Word IncVal, Long * MaxCounter, Long * CurrentCounter);
RET_ FAR_ CALL_ HLM_CHECKEXPDATE  (Long Slot, Word * Year, Word * Month, Word * Day);
RET_ FAR_ CALL_ HLM_GETRUSINFO    (Long * BufLen, Byte DATAFAR_ * RTBBuffer, Word Base64);
RET_ FAR_ CALL_ HLM_WRITELICENSE  (Long BufLen, Byte DATAFAR_ * RTBBuffer, Word Access, Byte DATAFAR_ * SearchStr,Word Options);
RET_ FAR_ CALL_ HLM_ISRUSHL       (Long * ID);

/* ---------------------- */
/* Hardlock Error Routine */
/* ---------------------- */
const char * FAR_ CALL_ HL_ERRMSG (Word num, Long options, Byte ** errdefine, Byte ** errextmsg);

/* ------------------------------------------- */
/* Obsolete functions, for compatiblity only!! */
/* ------------------------------------------- */
#ifndef __OS2__
void FAR_ CALL_ HL_ON        (Word Port, Word ModAd);
void FAR_ CALL_ HL_OFF       (Word Port);
Word FAR_ CALL_ K_EYE        (Word Port, char DATAFAR_ *Inp, Word BlkCnt);
void FAR_ CALL_ HL_WR        (Word Port, Word Reg, Word Val);
Word FAR_ CALL_ HL_RD        (Word Port, Word Reg);
void FAR_ CALL_ INT_ON       (void);
void FAR_ CALL_ INT_OFF      (void);
#endif
RET_ FAR_ CALL_ HL_CALC      (Word i1, Word i2, Word i3, Word i4);
RET_ FAR_ CALL_ HL_LMLOGIN   (Word ModAd, Word Access, Byte DATAFAR_ *RefKey, Byte DATAFAR_ *VerKey, Word SlotID, Byte DATAFAR_ *SearchStr);

#ifdef __cplusplus
  };
#endif
/* eof */
