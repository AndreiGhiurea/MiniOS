#include "main.h"
#include "screen.h"
#include "interrupts.h"
#include "stdlib.h"
#include "mp.h"
#include "apic.h"
#include "heap.h"

CPU_STATE gCpuState;
DWORD gActiveCpuCount;

void KernelMain()
{
    __enableSSE();  // only for demo; in the future will be called from __init.asm

    ClearScreen();

    ScreenSetHeader("MiniOS", BLACK_COLOR, CURRENT_COLOR);
    ScreenSetClock(0, BLACK_COLOR, CURRENT_COLOR);

    EnableX2Apic();

    FindAllApics();
         
    if (gCpuState.x2ApicSupported)
    {
        // Remap PIC and mask all interrupt to disable them
        InitPics(0x20, 0x28, 0xFF, 0xFF);
        InitApicIrqs();
    }
    else
    {
        InitPics(0x20, 0x28, 0x00, 0x00);
    }

    InitIdt();

    InitPit(50); // Set PIT to 50hz
    
    InitHeap();

    __sti();

    //__writemsr(APIC_ICR_MSR, 0x00000000000C4500); // Broadcast IPI
    // Sleep(10);
    //__writemsr(APIC_ICR_MSR, 0x00000000000C4609); // Broadcast SIPI
    // Sleep(200);
    //__writemsr(APIC_ICR_MSR, 0x00000000000C4609); // Broadcast 2nd SIPI

    // Sleep(200);

    // __writemsr(APIC_ICR_MSR, ICR_VALUE_SIPI); // Send a SIPI to all APs

    switch (gActiveCpuCount)
    {
    case 0:
        PrintLine("0");
        break;
    case 1:
        PrintLine("1");
        break;
    case 2:
        PrintLine("2");
        break;
    case 3:
        PrintLine("3");
        break;
    case 4:
        PrintLine("4");
        break;
    default:
        PrintLine("Hmmm");
    }

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

    // Read disk sectors using PIO mode ATA

    // TODO!!! Memory management: virtual, physical and heap memory allocatosr
}
