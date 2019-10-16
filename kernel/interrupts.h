#pragma once
#include "main.h"

#pragma pack(push, 1)
typedef struct _IDT_ENTRY {
    WORD Lowerbits;
    WORD Selector;
    BYTE Zero;
    BYTE Type;
    WORD Middlebits;
    DWORD Highbits;
    DWORD Zero2;
} IDT_ENTRY, *PIDT_ENTRY;

typedef struct _IDT {
    WORD Limit;
    QWORD Offset;
} IDT, *PIDT;
#pragma pack(pop)

#define PIC1 0x20
#define PIC2 0xA0

#define ICW1 0x11
#define ICW4 0x01

void InitIdt();
void InitPit();

void irq0_handler(void);
void irq1_handler(void);
void irq2_handler(void);
void irq3_handler(void);
void irq4_handler(void);
void irq5_handler(void);
void irq6_handler(void);
void irq7_handler(void);
void irq8_handler(void);
void irq9_handler(void);
void irq10_handler(void);
void irq11_handler(void);
void irq12_handler(void);
void irq13_handler(void);
void irq14_handler(void);
void irq15_handler(void);
void breakpoint_handler(void);

// imported from __init.asm
int breakpoint(void);
int irq0(void);
int irq1(void);
int irq2(void);
int irq3(void);
int irq4(void);
int irq5(void);
int irq6(void);
int irq7(void);
int irq8(void);
int irq9(void);
int irq10(void);
int irq11(void);
int irq12(void);
int irq13(void);
int irq14(void);
int irq15(void);