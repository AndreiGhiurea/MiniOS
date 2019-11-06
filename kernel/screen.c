#include "screen.h"
#include "stdlib.h"
#include "console.h"

static PSCREEN gVideo = (PSCREEN)(SCREEN_BASE_ADDRESS);
static DWORD gCurrLine = 0;
static DWORD gCurrColumn = 2;
static DWORD gCurrCursorPos = CHARS_PER_LINE + 2;
static CHAR gCommBuffer[1024];
static DWORD gCommIndex = 0;

void CursorMove(int row, int col)
{
    DWORD location = (row * MAX_COLUMNS) + col;       /* Short is a 16bit type , the formula is used here*/
    gCurrLine = row;
    gCurrColumn = col;
    gCurrCursorPos = CHARS_PER_LINE * row + col;

    //Cursor Low port
    __outbyte(0x3D4, 0x0F);                                    //Sending the cursor low byte to the VGA Controller
    __outbyte(0x3D5, (unsigned char)(location & 0xFF));

    //Cursor High port
    __outbyte(0x3D4, 0x0E);                                    //Sending the cursor high byte to the VGA Controller
    __outbyte(0x3D5, (unsigned char)((location >> 8) & 0xFF)); //Char is a 8bit type
}

void CursorPosition(int pos)
{
    int row, col;

    if (pos >= MAX_OFFSET)
    {
        pos = (pos % MAX_OFFSET) + ((MAX_LINES - 1) * CHARS_PER_LINE);
        ScrollScreen();
    }

    row = pos / MAX_COLUMNS;
    col = pos % MAX_COLUMNS;

    CursorMove(row, col);
}

void WriteChar(CHAR ch)
{
    gVideo[gCurrCursorPos].color = CURRENT_COLOR;
    gVideo[gCurrCursorPos].c = ch;

    gCommBuffer[gCommIndex] = ch;
    gCommIndex++;

    CursorPosition(gCurrCursorPos + 1);
}

void ScreenNewCmdLine()
{
    gCurrLine++;
    gCurrColumn = 2;

    if (gCurrLine == MAX_LINES)
    {
        ScrollScreen();
    }

    CursorPosition(gCurrLine * CHARS_PER_LINE + gCurrColumn);
    gVideo[gCurrLine * CHARS_PER_LINE].color = CURRENT_COLOR;
    gVideo[gCurrLine * CHARS_PER_LINE].c = '>';
}

void EnterPressed()
{
    gCommBuffer[gCommIndex] = '\0';

    if (gCommIndex != 0)
    {
        HandleCommand(gCommBuffer);
    }

    gCommIndex = 0; // Rewrite the buffer

    ScreenNewCmdLine();
}

void ScrollScreen()
{
    for (int i = CHARS_PER_LINE * 2; i < MAX_OFFSET; i++)
    {
        gVideo[i - CHARS_PER_LINE] = gVideo[i];
    }

    for (int i = CHARS_PER_LINE * (MAX_LINES - 1); i < MAX_OFFSET; i++)
    {
        gVideo[i].c = ' ';
    }

    gCurrLine--;
}

void PrintLine(char* text)
{
    int len = 0;

    gCurrLine++;

    if (gCurrLine == MAX_LINES)
    {
        ScrollScreen();
    }

    while (text[len] != 0)
    {
        len++;
    }

    int j = 0, i = 0;
    for (i = CHARS_PER_LINE * gCurrLine; (j < len) && (i < MAX_OFFSET); i++, j++)
    {
        if (text[j] == '\n')
        {
            gCurrLine++;
            gCurrColumn = 0;
            CursorMove(gCurrLine, gCurrColumn);
        }
        else
        {
            gVideo[i].color = CURRENT_COLOR;
            gVideo[i].c = text[j];
        }

    }

    CursorPosition(i);
}


void ClearScreen()
{
    int i;

    for (i = 0; i < MAX_OFFSET; i++)
    {
        gVideo[i].color = CURRENT_COLOR;
        gVideo[i].c = ' ';
    }

    ScreenNewCmdLine();
}

VOID
ScreenSetHeader(
    CHAR *Buffer,
    BYTE ForegroundColor,
    BYTE BackgroundColor
    )
{
    BYTE color = (BackgroundColor << 4) | ForegroundColor;
    DWORD size = strlen(Buffer);
    DWORD remainingLeft = (CHARS_PER_LINE - size) / 2;
    DWORD remainingRight = (CHARS_PER_LINE - size) / 2;

    PSCREEN header = (PSCREEN)(SCREEN_BASE_ADDRESS);

    for (DWORD i = 0; i < remainingLeft; i++)
    {
        header->c = ' ';
        header->color = color;
        header++;
    }

    for (DWORD i = 0; i < size; i++)
    {
        
        header->c = Buffer[i];
        header->color = color;
        header++;
    }

    for (DWORD i = 0; i < remainingRight; i++)
    {
        header->c = ' ';
        header->color = color;
        header++;
    }
}

VOID
ScreenSetClock(
    QWORD Seconds,
    BYTE ForegroundColor,
    BYTE BackgroundColor
    )
{
    BYTE color = (BackgroundColor << 4) | ForegroundColor;
    CHAR clock[64] = { 0 };
    DWORD clockSize = 0;
    PSCREEN header = (PSCREEN)(SCREEN_BASE_ADDRESS) + CHARS_PER_LINE - 1;

    itoa(Seconds, clock, 10, TRUE);
    clockSize = strlen(clock);

    header->c = 's';
    header->color = color;
    header--;

    for (DWORD i = 0; i < clockSize; i++)
    {
        header->c = clock[i];
        header->color = color;
        header--;
    }
}