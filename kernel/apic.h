#pragma once
#include "main.h"

#define IOAPICID          0x00
#define IOAPICVER         0x01
#define IOAPICARB         0x02
#define IOAPICREDTBL(n)   (0x10 + 2 * n) // lower-32bits (add +1 for upper 32-bits)

#define IA32_APIC_BASE_MSR  0x1B
#define APIC_ICR_MSR        0x830
#define APIC_BASE_MASK      0xFFFFFF000
#define APIC_EOI_REG        0x80B
#define APIC_SVR_REG        0x80F
#define APIC_LVT_TMR        0x832
#define APIC_LVT_PERF       0x834
#define APIC_LVT_LINT0      0x835
#define APIC_LVT_LINT1      0x836
#define APIC_LVT_ERR        0x837
#define APIC_TMRINITCNT     0x838
#define APIC_TMRCURRCNT     0x839
#define APIC_TMRDIV         0x83E

#define ICR_VALUE_SIPI      0xFF000000000C4609 // SIPI to all processors excluding self, vector number 9 (starting page for SIPI)

void WriteIOApicRegister(const QWORD apic_base, const BYTE offset, const DWORD val);
DWORD ReadIOApicRegister(const QWORD apic_base, const BYTE offset);
VOID EnableX2Apic();
VOID InitApicIrqs();