#include "main.h"
#include "screen.h"
#include "interrupts.h"

CPU_STATE gCpuState;

void KernelMain()
{
    __enableSSE();  // only for demo; in the future will be called from __init.asm

    ClearScreen();

    ScreenSetHeader("MiniOS", BLACK_COLOR, CURRENT_COLOR);
    ScreenSetClock(0, BLACK_COLOR, CURRENT_COLOR);

    int cpuInfo[4];
    __cpuid(cpuInfo, 0x01);
    if (cpuInfo[3] & (1 << 9))
    {
        gCpuState.ApicSupported = TRUE;
    }
    else
    {
        gCpuState.ApicSupported = FALSE;
    }

    if (gCpuState.ApicSupported)
    {
        // Remap PIC and mask all interrupt to disable them
        InitPics(0x20, 0x28, 0xFF, 0xFF);

    }
    else
    {
        InitPics(0x20, 0x28, 0x00, 0x00);
    }

    InitIdt();

    InitPit(50); // Set PIT to 50hz
    
    __sti();

    // __interrupt();

    for (;;)
    {
        __halt();
    }

    // PIC programming; see http://www.osdever.net/tutorials/view/programming-the-pic

    // Define interrupt routines and dump trap frame
    
    // Timer programming

    // Keyboard programming

    // Implement a simple console

    // TODO!!! read disk sectors using PIO mode ATA

    // TODO!!! Memory management: virtual, physical and heap memory allocatosr
}
