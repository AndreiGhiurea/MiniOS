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
    CHAR text[256];
    CHAR auxText[64];
    LAPIC* pLapic = NULL;
    QWORD sendVal = 0x0;

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

    volatile BYTE *cpus = (BYTE*)0x200;
    gActiveCpuCount = *cpus;
    for (DWORD i = 0; i < gCpuState.NrOfLapics; i++)
    {
        pLapic = (LAPIC*)gCpuState.Lapics[i];

        if ((QWORD)pLapic->ApicID == gCpuState.BspLapicId)
        {
            PrintLine("Don't try to wake BSP");
            continue;
        }

        sendVal = ICR_VALUE_IPI | ((QWORD)(pLapic->ApicID) << 32); // Set IPI target
        __writemsr(APIC_ICR_MSR, sendVal); // Send IPI
        Sleep(20);

        sendVal = ICR_VALUE_SIPI | ((QWORD)(pLapic->ApicID) << 32) | 8; // Set SIPI target and page 8 (0x8000)
        __writemsr(APIC_ICR_MSR, sendVal); // Send SIPI
        Sleep(120);

        if (*cpus == gActiveCpuCount)
        {
            // One more SIPI
            PrintLine("One More SIPI");
            __writemsr(APIC_ICR_MSR, sendVal); // Send SIPI
            Sleep(1200);
        }

        if (*cpus == gActiveCpuCount)
        {
            PrintLine("CPU didn't wake abort whole process");
            break; // Abort wakeup routine
        }


        text[0] = '\0';
        strcat(text, "CPU woke up. ID: ", 256);
        itoa(pLapic->ProcessorID, auxText, 16, FALSE);
        strcat(text, auxText, 256);
        PrintLine(text);

        gActiveCpuCount = *cpus;
    }

    text[0] = '\0';
    strcat(text, "Total Nr Or Procs: ", 256);
    itoa(gActiveCpuCount, auxText, 16, FALSE);
    strcat(text, auxText, 256);
    PrintLine(text);

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
