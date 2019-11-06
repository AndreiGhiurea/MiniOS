#ifndef _SCREEN_H_
#define _SCREEN_H_

#include "main.h"

#define SCREEN_BASE_ADDRESS         0x000B8000
#define MAX_LINES                   25 // Reserve 1 line for header
#define MAX_COLUMNS                 80
#define CHARS_PER_LINE              MAX_COLUMNS
#define MAX_OFFSET                  (MAX_LINES * MAX_COLUMNS) // 24 lines * 80 chars

#define BLACK_COLOR                 0x0
#define RED_COLOR                   0x4
#define GREY_COLOR                  0x7
#define GREEN_COLOR                 0x2
#define BLUE_COLOR                  0x1
#define CYAN_COLOR                  0x3
#define PURPLE_COLOR                0x5

#define CURRENT_COLOR               GREY_COLOR

#pragma pack(push)
#pragma pack(1)
typedef struct _SCREEN
{
    char c;
    BYTE color;
}SCREEN, *PSCREEN;
#pragma pack(pop)

void SetColor(BYTE Color);
void ClearScreen();
void PutChar(char C, int Pos);
void PutString(char* String, int Pos);
void PutStringLine(char* String, int Line);
void PrintLine(char* text);
void ScrollScreen();
void WriteChar(CHAR ch);
void EnterPressed();
void ScreenNewCmdLine();

VOID 
ScreenSetClock(
    QWORD Seconds,
    BYTE ForegroundColor,
    BYTE BackgroundColor
    );

VOID
ScreenSetHeader(
    CHAR *Buffer,
    BYTE ForegroundColor,
    BYTE BackgroundColor
    );
#endif // _SCREEN_H_