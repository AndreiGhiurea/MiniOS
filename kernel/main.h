#ifndef _MAIN_H_
#define _MAIN_H_

#include <intrin.h>
//
// default types
//
typedef unsigned __int8     BYTE, *PBYTE;
typedef unsigned __int16    WORD, *PWORD;
typedef unsigned __int32    DWORD, *PDWORD;
typedef unsigned __int64    QWORD, *PQWORD;
typedef signed __int8       CHAR, *PCHAR;
typedef signed __int8       INT8;
typedef signed __int16      INT16;
typedef signed __int32      INT32;
typedef signed __int64      INT64;
typedef void                VOID, *PVOID;
typedef unsigned __int8     BOOL, *PBOOL;

#define TRUE                ((BOOL)0x1)
#define FALSE               ((BOOL)0x0)

#define MIN(x,y)            (((x) < (y)) ? (x) : (y))
#define MAX(x,y)            (((x) < (y)) ? (y) : (x))

#define PAGE_SIZE           0x1000

typedef struct _CPU_STATE {
    BOOL x2ApicSupported;
    DWORD BspLapicId;
    QWORD LapicBase;
    QWORD IoApics[16];
    DWORD NrOfIoApics;
    QWORD Lapics[16];
    DWORD NrOfLapics;
} CPU_STATE, *PCPU_STATE;

//
// exported functions from __init.asm
//
void __cli(void);
void __sti(void);
void __magic(void);         // MAGIC breakpoint into BOCHS (XCHG BX,BX)
void __enableSSE(void);
void __load_idt(QWORD IdtAddress);
void __halt(void);
void __interrupt(void);
void __dumpTrapFrame(void);

#endif // _MAIN_H_