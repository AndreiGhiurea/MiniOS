#ifndef _SCREEN_H_
#define _SCREEN_H_

#include "main.h"

#define MAX_LINES       25
#define MAX_COLUMNS     80
#define CHARS_PER_LINE  MAX_COLUMNS
#define MAX_OFFSET      2000 // 25 lines * 80 chars


#pragma pack(push)
#pragma pack(1)
typedef struct _SCREEN
{
    char c;
    BYTE color;
}SCREEN, *PSCREEN;
#pragma pack(pop)

void HelloBoot();

void SetColor(BYTE Color);
void ClearScreen();
void PutChar(char C, int Pos);
void PutString(char* String, int Pos);
void PutStringLine(char* String, int Line);
void PrintLine(char* text);
void ScrollScreen();


#endif // _SCREEN_H_