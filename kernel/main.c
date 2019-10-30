#include "main.h"
#include "screen.h"
#include "interrupts.h"

void KernelMain()
{
    __enableSSE();  // only for demo; in the future will be called from __init.asm

    ClearScreen();

    ScreenSetHeader("MiniOS", BLACK_COLOR, GREEN_COLOR);
    ScreenSetClock(0, BLACK_COLOR, GREEN_COLOR);

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
