#pragma once
#include "main.h"

#define HEAP_START_ADDRESS              0x40000000 // 1GB
#define HEAP_NR_OF_PAGES                100
#define HEAP_SIZE                       HEAP_NR_OF_PAGES * PAGE_SIZE
#define HEAP_END_ADDRESS                HEAP_START_ADDRESS + HEAP_SIZE  


#define malloc                          _malloc

VOID InitHeap();
VOID* _malloc(DWORD Size);