#include "main.h"
#include "screen.h"
#include "interrupts.h"

void KernelMain()
{
    __enableSSE();  // only for demo; in the future will be called from __init.asm

    ClearScreen();

    InitIdt();

    InitPit();

    __sti();

    for (int i = 0; i< 10;i++)
    {
        __halt();
    }

    // TODO!!! PIC programming; see http://www.osdever.net/tutorials/view/programming-the-pic
    // TODO!!! define interrupt routines and dump trap frame
    
    // TODO!!! Timer programming

    // TODO!!! Keyboard programming

    // TODO!!! Implement a simple console

    // TODO!!! read disk sectors using PIO mode ATA

    // TODO!!! Memory management: virtual, physical and heap memory allocators
}
