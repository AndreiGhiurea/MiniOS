#include "console.h"
#include "screen.h"
#include "stdlib.h"
#include "heap.h"

WORD gReadSector[256];

VOID HandleCommand(CHAR* Buffer)
{
    if (0 == strcmp(Buffer, "read"))
    {
        HandleRead();
    }
    else if (0 == strcmp(Buffer, "trapframe"))
    {
        __dumpTrapFrame();
    }
    else if (0 == strcmp(Buffer, "dump"))
    {
        HandleDump();
    }
    else if (0 == strcmp(Buffer, "memtest"))
    {
        HandleHeapTest();
    }
    else
    {
        PrintLine("[ERROR] Invalid Command");
    }
}


VOID HandleDump()
{
    PrintLine((CHAR*)gReadSector);
}


VOID HandleHeapTest()
{
    DWORD size = 0x12743;
    VOID *addr, *addr2;
    CHAR text[256], addrTxt[64];
    
    PrintLine("Allocating 0x12743 bytes");
    addr = malloc(size);
    if (NULL == addr)
    {
        PrintLine("malloc failed");
    }

    CHAR* test = "Testing the heap";
    memcpy(addr, test, strlen(test));
    PrintLine(addr);
    
    text[0] = '\0';
    strcat(text, "Freeing the heap addr: 0x", 256);
    itoa((QWORD)addr, addrTxt, 16, FALSE);
    strcat(text, addrTxt, 256);
    PrintLine(text);

    free(addr);
    
    PrintLine("Allocate 0x12743 Twice");
    addr = malloc(size);
    addr2 = malloc(size);
    
    text[0] = '\0';
    strcat(text, "First addr: 0x", 256);
    itoa((QWORD)addr, addrTxt, 16, FALSE);
    strcat(text, addrTxt, 256);
    PrintLine(text);
    
    text[0] = '\0';
    strcat(text, "Second addr: 0x", 256);
    itoa((QWORD)addr2, addrTxt, 16, FALSE);
    strcat(text, addrTxt, 256);
    PrintLine(text);

    free(addr);
    free(addr2);
}


VOID HandleRead()
{
    DWORD lba = 0x0; // Starting address on disk

    __outbyte(0x1F6, 0xE0);
    __outbyte(0x1F2, 0x1); // Reading one sector

    __outbyte(0x1F3, (unsigned char)lba);
    __outbyte(0x1F4, (unsigned char)(lba >> 8));
    __outbyte(0x1F5, (unsigned char)(lba >> 16));

    __outbyte(0x1F7, 0x20); // Read

    PrintLine("Reading one sector from disk");
}
