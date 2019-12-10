#include "main.h"
#include "screen.h"
#include "interrupts.h"
#include "stdlib.h"
#include "heap.h"

CPU_STATE gCpuState;
DWORD gActiveCpuCount;

void KernelMain()
{
    __enableSSE();  // only for demo; in the future will be called from __init.asm

    ClearScreen();

    ScreenSetHeader("MiniOS", BLACK_COLOR, CURRENT_COLOR);
    ScreenSetClock(0, BLACK_COLOR, CURRENT_COLOR);

    InitPics(0x20, 0x28, 0x00, 0x00);

    InitIdt();

    InitPit(50); // Set PIT to 50hz
    
    InitHeap();

    __sti();

    for (;;)
    {
        __halt();
    }

    // PIC programming; see http://www.osdever.net/tutorials/view/programming-the-pic

    // Define interrupt routines and dump trap frame
    
    // Timer programming

    // Keyboard programming

    // Implement a simple console

    // Read disk sectors using PIO mode ATA

    // Memory management: virtual, physical and heap memory allocatosr
}
