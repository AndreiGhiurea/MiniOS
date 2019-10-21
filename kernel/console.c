#include "console.h"
#include "screen.h"
#include "stdlib.h"

VOID HandleCommand(CHAR* Buffer)
{
    if (0 == strcmp(Buffer, "read"))
    {
        HandleRead();
    }
    else
    {
        PrintLine("[ERROR] Invalid Command");
    }
}


VOID HandleRead()
{
    return;
}
