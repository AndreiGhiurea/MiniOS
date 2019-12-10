#pragma  once
#include "main.h"

#define strlen      _strlen
#define itoa        _itoa
#define strcmp      _strcmp
#define strcat      _strcat
#define memcpy      _memcpy

DWORD _strlen(CHAR *_Str);
DWORD _strcmp(CHAR* Buf, CHAR* Src);
VOID _memcpy(VOID* Dest, VOID* Source, DWORD Size);
VOID _itoa(QWORD num, CHAR* str, DWORD base, BOOL reversed);
VOID _strcat(CHAR* Dest, CHAR *Buf, DWORD Size);
VOID Sleep(DWORD Miliseconds);