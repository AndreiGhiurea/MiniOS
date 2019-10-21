#include "stdlib.h"

DWORD _strlen(CHAR *_Str)
{
    DWORD len = 0;
    for (len = 0; _Str[len] != '\0'; len++);

    return len;
}

/* A utility function to reverse a string  */
static VOID reverse(CHAR *str, DWORD length)
{
    CHAR temp = '\0';
    DWORD start = 0;
    DWORD end = length - 1;
    while (start < end)
    {
        temp = *(str + start);
        *(str + start) = *(str + end);
        *(str + end) = temp;

        start++;
        end--;
    }
}

// Implementation of itoa() 
VOID _itoa(QWORD num, CHAR* str, DWORD base, BOOL reversed)
{
#pragma warning(disable:4244)
    INT32 i = 0;

    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str[i] = '0';
        str[i + 1] = '\0';
        return;
    }
    
    // Process individual digits 
    while (num != 0)
    {
        INT32 rem = num % base;
        str[i] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        i++;
        num = num / base;
    }

    str[i] = '\0'; // Append string terminator 
    
    if (reversed)
    {
        return;
    }
    
    // Reverse the string 
    reverse(str, i);

    return;
}

DWORD _strcmp(CHAR* Buf, CHAR* Src)
{
    DWORD lenBuf = strlen(Buf);
    DWORD lenSrc = strlen(Src);
    DWORD len = MIN(lenBuf, lenSrc);

    if (lenBuf != lenSrc)
    {
        return len;
    }
    
    for (DWORD i = 0; i < len; i++)
    {
        if (Buf[i] != Src[i])
        {
            return i;
        }
    }

    return 0;
}

VOID _strcat(CHAR* Dest, CHAR *Buf, DWORD Size)
{
    DWORD i = strlen(Dest);
    for (DWORD j = 0; i < Size && j < strlen(Buf); i++, j++)
    {
        Dest[i] = Buf[j];
    }

    Dest[i] = '\0';

    return;
}

VOID _memset(BYTE* Dest, BYTE Val, DWORD Size)
{
    for (DWORD i = 0; i < Size; i++)
    {
        Dest[i] = Val;
    }
}