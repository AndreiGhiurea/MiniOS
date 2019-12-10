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

typedef struct _TRAP_FRAME {
    QWORD Rax;
    QWORD Rbx;
    QWORD Rcx;
    QWORD Rdx;
    QWORD Rsi;
    QWORD Rdi;
    QWORD Rbp;
    QWORD Rsp;
    QWORD R8;
    QWORD R9;
    QWORD R10;
    QWORD R11;
    QWORD R12;
    QWORD R13;
    QWORD R14;
    QWORD R15;
    QWORD Rip;
    QWORD Flags;
    QWORD Fs;
    QWORD Gs;
    QWORD Cr0;
    QWORD Cr3;
    QWORD Cr4;
} TRAP_FRAME, *PTRAP_FRAME;

#define PIC1 0x20
#define PIC2 0xA0

#define ICW1 0x11
#define ICW4 0x01

void InitPics(BYTE pic1, BYTE pic2, BYTE mask1, BYTE mask2);
void InitIdt();
void InitPit(DWORD frequency);

void Irq0Handler(void);
void Irq1Handler(void);
void Irq2Handler(void);
void Irq3Handler(void);
void Irq4Handler(void);
void Irq5Handler(void);
void Irq6Handler(void);
void Irq7Handler(void);
void Irq8Handler(void);
void Irq9Handler(void);
void Irq10Handler(void);
void Irq11Handler(void);
void Irq12Handler(void);
void Irq13Handler(void);
void Irq14Handler(void);
void Irq15Handler(void);
void BreakpointHandler(void);
void PageFaultHandler(void);
void GenericInt(void);

// imported from __init.asm
int __int3(void);
int __int14(void);
int __irq0(void);
int __irq1(void);
int __irq2(void);
int __irq3(void);
int __irq4(void);
int __irq5(void);
int __irq6(void);
int __irq7(void);
int __irq8(void);
int __irq9(void);
int __irq10(void);
int __irq11(void);
int __irq12(void);
int __irq13(void);
int __irq14(void);
int __irq15(void);
int __genericInt(void);