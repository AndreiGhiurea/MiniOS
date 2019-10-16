#include "interrupts.h"
#include "keyboard_map.h"
#include "screen.h"

IDT_ENTRY gIdtEntries[256];

IDT gIdt;

///*
// * init the PICs and remap them
// */
static void InitPics(BYTE pic1, BYTE pic2)
{
    /* send ICW1 */
    __outbyte(PIC1, ICW1);
    __outbyte(PIC2, ICW1);

    /* send ICW2 */
    __outbyte(PIC1 + 1, pic1);	/* remap */
    __outbyte(PIC2 + 1, pic2);	/*  pics */

    /* send ICW3 */
    __outbyte(PIC1 + 1, 4);	/* IRQ2 -> connection to slave */
    __outbyte(PIC2 + 1, 2);

    /* send ICW4 */
    __outbyte(PIC1 + 1, ICW4);
    __outbyte(PIC2 + 1, ICW4);

    __outbyte(PIC1 + 1, 0xFD);
    __outbyte(PIC2 + 1, 0xFD);
}

// Sets the PIT to a fixed frequency to measue time in milliseconds
void InitPit()
{
    __outbyte(0x43, 0x34);

    __outbyte(0x40, 0xFF); // Set low byte of PIT reload value
    __outbyte(0x40, 0xFF); // Set high byte of PIT reload value
}

void breakpoint_handler(void)
{
    PrintLine("Breakpoint interrupt!");
}

void irq0_handler(void)
{
    PrintLine("PIT irq");
    __outbyte(0x20, 0x20); //EOI
}

void irq1_handler(void)
{
    BYTE scanCode = 0x0;
    CHAR key = 0x0;

    scanCode = __inbyte(0x60);

    // Check if a key was released and ignore for now
    if (scanCode > 0x80)
    {
        goto leave;
    }

    key = GetCharFromScancode(scanCode);
    WriteChar(key);

leave:
    __outbyte(0x20, 0x20); //EOI
}

void irq2_handler(void)
{
    __outbyte(0x20, 0x20); //EOI
}

void irq3_handler(void)
{
    __outbyte(0x20, 0x20); //EOI
}

void irq4_handler(void)
{
    __outbyte(0x20, 0x20); //EOI
}

void irq5_handler(void)
{
    __outbyte(0x20, 0x20); //EOI
}

void irq6_handler(void)
{
    __outbyte(0x20, 0x20); //EOI
}

void irq7_handler(void) 
{
    __outbyte(0x20, 0x20); //EOI
}

void irq8_handler(void)
{
    __outbyte(0xA0, 0x20);
    __outbyte(0x20, 0x20); //EOI   
}

void irq9_handler(void)
{
    __outbyte(0xA0, 0x20);
    __outbyte(0x20, 0x20); //EOI   
}

void irq10_handler(void)
{
    __outbyte(0xA0, 0x20);
    __outbyte(0x20, 0x20); //EOI   
}

void irq11_handler(void)
{
    __outbyte(0xA0, 0x20);
    __outbyte(0x20, 0x20); //EOI   
}

void irq12_handler(void)
{
    __outbyte(0xA0, 0x20);
    __outbyte(0x20, 0x20); //EOI   
}

void irq13_handler(void)
{
    __outbyte(0xA0, 0x20);
    __outbyte(0x20, 0x20); //EOI   
}

void irq14_handler(void)
{
    __outbyte(0xA0, 0x20);
    __outbyte(0x20, 0x20); //EOI   
}

void irq15_handler(void)
{
    __outbyte(0xA0, 0x20);
    __outbyte(0x20, 0x20); //EOI   
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
    InitPics(0x20, 0x28);

    SetupIdtEntry(3, (QWORD)breakpoint);
    SetupIdtEntry(32, (QWORD)irq0);
    SetupIdtEntry(33, (QWORD)irq1);
    SetupIdtEntry(34, (QWORD)irq2);
    SetupIdtEntry(35, (QWORD)irq3);
    SetupIdtEntry(36, (QWORD)irq4);
    SetupIdtEntry(37, (QWORD)irq5);
    SetupIdtEntry(38, (QWORD)irq6);
    SetupIdtEntry(39, (QWORD)irq7);
    SetupIdtEntry(40, (QWORD)irq8);
    SetupIdtEntry(41, (QWORD)irq9);
    SetupIdtEntry(42, (QWORD)irq10);
    SetupIdtEntry(43, (QWORD)irq11);
    SetupIdtEntry(44, (QWORD)irq12);
    SetupIdtEntry(45, (QWORD)irq13);
    SetupIdtEntry(46, (QWORD)irq14);
    SetupIdtEntry(47, (QWORD)irq15);

    gIdt.Limit = sizeof(IDT_ENTRY) * 256;
    gIdt.Offset = (QWORD)gIdtEntries;

    __load_idt((QWORD)&gIdt);
}