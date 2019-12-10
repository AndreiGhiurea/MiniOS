#include "interrupts.h"
#include "keyboard_map.h"
#include "stdlib.h"
#include "apic.h"
#include "screen.h"

IDT_ENTRY gIdtEntries[256];
IDT gIdt;

QWORD gSeconds;
BYTE gSecondFractions;
extern WORD gReadSector[256];
extern CPU_STATE gCpuState;
extern volatile BOOL gSleeping;
extern volatile QWORD gSleepingMs;

static void SignalEoi(BOOL Slave)
{
    if (!gCpuState.x2ApicSupported)
    {
        if (Slave)
        {
            __outbyte(0xA0, 0x20);
        }

        __outbyte(0x20, 0x20); //EOI
    }
    else
    {
        __writemsr(APIC_EOI_REG, 0x0);
    }
}

// *
// * init the PICs and remap them
// *
void InitPics(BYTE pic1, BYTE pic2, BYTE mask1, BYTE mask2)
{
    /* send ICW1 */
    __outbyte(PIC1, ICW1);
    __outbyte(PIC2, ICW1);

    /* send ICW2 */
    __outbyte(PIC1 + 1, pic1);  /* remap */
    __outbyte(PIC2 + 1, pic2);  /*  pics */

    /* send ICW3 */
    __outbyte(PIC1 + 1, 4);	/* IRQ2 -> connection to slave */
    __outbyte(PIC2 + 1, 2);

    /* send ICW4 */
    __outbyte(PIC1 + 1, ICW4);
    __outbyte(PIC2 + 1, ICW4);

    __outbyte(PIC1 + 1, mask1);
    __outbyte(PIC2 + 1, mask2);
}

// Init PIT timer
void InitPit(DWORD frequency)
{
    // The value we send to the PIT is the value to divide it's input clock
    // (1193180 Hz) by, to get our required frequency. Important to note is
    // that the divisor must be small enough to fit into 16-bits.
    DWORD divisor = 1193180 / frequency;

    // Send the command byte.
    __outbyte(0x43, 0x36);

    // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
    BYTE l = (BYTE)(divisor & 0xFF);
    BYTE h = (BYTE)((divisor >> 8) & 0xFF);

    // Send the frequency divisor.
    __outbyte(0x40, l);
    __outbyte(0x40, h);
}

void DumpTrapFrame(TRAP_FRAME* TrapFrame)
{
    CHAR text[64] = { 0 };
    CHAR reg[16] = { 0 };

    PrintLine("Dumping Trap Frame");

    strcat(text, "RAX:", 64);
    itoa(TrapFrame->Rax, reg, 16, FALSE);
    strcat(text, reg, 64);
    PrintLine(text);
    memset((BYTE*)text, '\0', 64);

    strcat(text, "RBX:", 64);
    itoa(TrapFrame->Rbx, reg, 16, FALSE);
    strcat(text, reg, 64);
    PrintLine(text);
    memset((BYTE*)text, '\0', 64);

    strcat(text, "RCX:", 64);
    itoa(TrapFrame->Rcx, reg, 16, FALSE);
    strcat(text, reg, 64);
    PrintLine(text);
    memset((BYTE*)text, '\0', 64);

    strcat(text, "RDX:", 64);
    itoa(TrapFrame->Rdx, reg, 16, FALSE);
    strcat(text, reg, 64);
    PrintLine(text);
    memset((BYTE*)text, '\0', 64);

    strcat(text, "RSP:", 64);
    itoa(TrapFrame->Rsp, reg, 16, FALSE);
    strcat(text, reg, 64);
    PrintLine(text);
    memset((BYTE*)text, '\0', 64);

    strcat(text, "RIP:", 64);
    itoa(TrapFrame->Rip, reg, 16, FALSE);
    strcat(text, reg, 64);
    PrintLine(text);
    memset((BYTE*)text, '\0', 64);

    strcat(text, "FLAGS:", 64);
    itoa(TrapFrame->Flags, reg, 16, FALSE);
    strcat(text, reg, 64);
    PrintLine(text);
    memset((BYTE*)text, '\0', 64);

    strcat(text, "CR3:", 64);
    itoa(TrapFrame->Cr3, reg, 16, FALSE);
    strcat(text, reg, 64);
    PrintLine(text);
    memset((BYTE*)text, '\0', 64);
}

void BreakpointHandler(void)
{
    PrintLine("INT3 - Breakpoint Interrupt");
    __dumpTrapFrame();
 }

void PageFaultHandler(void)
{
    PrintLine("PageFault");
    __dumpTrapFrame();
}

void GenericInt(void)
{
    PrintLine("BSOD");
    __dumpTrapFrame();
}

void Irq0Handler(void)
{
    gSecondFractions++;

    if (gSleeping)
    {
        gSleepingMs += 20;
    }

    if (gSecondFractions == 50)
    {
        gSecondFractions = 0;
        gSeconds++;
        ScreenSetClock(gSeconds, BLACK_COLOR, CURRENT_COLOR);
    }

    SignalEoi(FALSE);
}

void Irq1Handler(void)
{
    BYTE scanCode = 0x0;
    CHAR key = 0x0;

    scanCode = __inbyte(0x60);

    // Check if a key was released and ignore for now
    if (scanCode > 0x80)
    {
        goto leave;
    }

    // If Enter was pressed
    if (scanCode == 0x1C)
    {
        EnterPressed();
        goto leave;
    }

    key = GetCharFromScancode(scanCode);
    WriteChar(key);

leave:
    SignalEoi(FALSE);
}

void Irq2Handler(void)
{
    PrintLine("222");
    SignalEoi(FALSE);
}

void Irq3Handler(void)
{
    SignalEoi(FALSE);
}

void Irq4Handler(void)
{
    SignalEoi(FALSE);
}

void Irq5Handler(void)
{
    SignalEoi(FALSE);
}

void Irq6Handler(void)
{
    SignalEoi(FALSE);
}

void Irq7Handler(void) 
{
    SignalEoi(FALSE);
}

void Irq8Handler(void)
{
    SignalEoi(TRUE);
}

void Irq9Handler(void)
{
    SignalEoi(TRUE);
}

void Irq10Handler(void)
{
    SignalEoi(TRUE);
}

void Irq11Handler(void)
{
    SignalEoi(TRUE);
}

void Irq12Handler(void)
{
    SignalEoi(TRUE);
}

void Irq13Handler(void)
{
    SignalEoi(TRUE);
}

void Irq14Handler(void)
{
    // Transfer 256 16 - bit values, a uint16_t at a time, into your buffer from I / O port 0x1F0. (In assembler, REP INSW works well for this.)
    for (DWORD i = 0; i < 256; i++)
    {
        gReadSector[i] = (WORD)__inword(0x1F0);
    }

    SignalEoi(TRUE);
}

void Irq15Handler(void)
{
    SignalEoi(TRUE);
}

static void SetupIdtEntry(DWORD Index, QWORD Address)
{
    gIdtEntries[Index].Lowerbits = Address & 0xFFFF;
    gIdtEntries[Index].Selector = 0x28; /* KERNEL_CODE_SEGMENT_OFFSET */
    gIdtEntries[Index].Zero = 0;
    gIdtEntries[Index].Type = 0x8e; /* INTERRUPT_GATE */
    gIdtEntries[Index].Middlebits = (Address & 0xFFFF0000) >> 16;
    gIdtEntries[Index].Highbits = (Address & 0xFFFFFFFF00000000) >> 32;
    gIdtEntries[Index].Zero2 = 0;
}

void InitIdt()
{
    // for (DWORD i = 0; i <= 16; i++)
    // {
    //     SetupIdtEntry(i, (QWORD)__genericInt);
    // }
    
    SetupIdtEntry(32, (QWORD)__irq0);
    SetupIdtEntry(33, (QWORD)__irq1);
    SetupIdtEntry(34, (QWORD)__irq2);
    SetupIdtEntry(35, (QWORD)__irq3);
    SetupIdtEntry(36, (QWORD)__irq4);
    SetupIdtEntry(37, (QWORD)__irq5);
    SetupIdtEntry(38, (QWORD)__irq6);
    SetupIdtEntry(39, (QWORD)__irq7);
    SetupIdtEntry(40, (QWORD)__irq8);
    SetupIdtEntry(41, (QWORD)__irq9);
    SetupIdtEntry(42, (QWORD)__irq10);
    SetupIdtEntry(43, (QWORD)__irq11);
    SetupIdtEntry(44, (QWORD)__irq12);
    SetupIdtEntry(45, (QWORD)__irq13);
    SetupIdtEntry(46, (QWORD)__irq14);
    SetupIdtEntry(47, (QWORD)__irq15);

    gIdt.Limit = sizeof(IDT_ENTRY) * 256;
    gIdt.Offset = (QWORD)gIdtEntries;

    __load_idt((QWORD)&gIdt);
}