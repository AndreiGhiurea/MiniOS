#include "screen.h"

static PSCREEN gVideo = (PSCREEN)(0x000B8000);
static DWORD gCurrLine = 0;

void CursorMove(int row, int col)
{
    unsigned short location = (row * MAX_COLUMNS) + col;       /* Short is a 16bit type , the formula is used here*/

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

    if (pos > MAX_OFFSET)
    {
        pos = pos % MAX_OFFSET;
    }

    row = pos / MAX_COLUMNS;
    col = pos % MAX_COLUMNS;

    CursorMove(row, col);
}

void HelloBoot()
{
    int i, len;
	char boot[] = "Hello Boot! Greetings from C...";

	len = 0;
	while (boot[len] != 0)
	{
		len++;
	}

	for (i = 0; (i < len) && (i < MAX_OFFSET); i++)
	{
		gVideo[i].color = 10;
		gVideo[i].c = boot[i];
	}

    CursorPosition(i);
}

void ScrollScreen()
{
    for (int i = CHARS_PER_LINE; i < MAX_OFFSET; i++)
    {
        gVideo[i - CHARS_PER_LINE] = gVideo[i];
    }

    gCurrLine--;
}

void PrintLine(char* text)
{
    int len = 0;

    if (gCurrLine == MAX_LINES)
    {
        ScrollScreen();
    }

    while (text[len] != 0)
    {
        len++;
    }
    
    int j = 0;
    for (int i = CHARS_PER_LINE * gCurrLine; (j < len) && (i < MAX_OFFSET); i++, j++)
    {
        gVideo[i].color = 10;
        gVideo[i].c = text[j];
    }

    gCurrLine++;
}

void ClearScreen()
{
    int i;

    for (i = 0; i < MAX_OFFSET; i++)
    {
        gVideo[i].color = 10;
        gVideo[i].c = ' ';
    }

    CursorMove(0, 0);
}

