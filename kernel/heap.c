#include "heap.h"

VOID* gHeapPages[HEAP_NR_OF_PAGES];

VOID InitHeap()
{
    for (QWORD i = 0; i < HEAP_NR_OF_PAGES; i++)
    {
        gHeapPages[i] = (VOID*)(HEAP_START_ADDRESS + PAGE_SIZE * i);
    }
}

VOID* _malloc(DWORD Size)
{
    DWORD NrOfPages = 0;
    BOOL found = TRUE;
    DWORD pageInfo = 0;
    VOID* allocAddress = NULL;
    
    NrOfPages = Size / PAGE_SIZE;

    if (Size % PAGE_SIZE)
    {
        NrOfPages++;
    }

    if (NrOfPages > HEAP_NR_OF_PAGES)
    {
        return NULL;
    }

    for (DWORD i = 0; i < HEAP_NR_OF_PAGES - NrOfPages; i++)
    {
        found = TRUE;

        for (DWORD j = 0; j < NrOfPages; j++)
        {
            pageInfo = (QWORD)(gHeapPages[i + j]) & 0xFFFF;
            if (pageInfo)
            {
                found = FALSE;
            }
        }

        if (found)
        {
            allocAddress = gHeapPages[i];
            gHeapPages[i] = (VOID*)((QWORD)(gHeapPages[i]) | (NrOfPages & 0xFFFF));
            for (DWORD j = 1; j < NrOfPages; j++)
            {
                gHeapPages[i + j] = (VOID*)((QWORD)(gHeapPages[i + j]) | 0xFFFF);
            }

            return allocAddress;
        }
    }

    return NULL;
}

VOID free(VOID* StartAddress)
{
    DWORD nrOfPages;
    QWORD startIndex;

    if ((QWORD)StartAddress < HEAP_START_ADDRESS ||
        (QWORD)StartAddress > HEAP_END_ADDRESS)
    {
        return;
    }

    if ((QWORD)StartAddress & 0xFFF)
    {
        return;
    }

    startIndex = ((QWORD)StartAddress - HEAP_START_ADDRESS) / PAGE_SIZE;
    nrOfPages = (QWORD)(gHeapPages[startIndex]) & 0xFFFF;

    if (nrOfPages == 0xFFFF)
    {
        return;
    }

    for (DWORD i = 0; i < nrOfPages; i++)
    {
        gHeapPages[i] = (VOID*)((QWORD)(gHeapPages[i]) & 0xFFFFFFFFFFFF0000);
    }

    return;
}