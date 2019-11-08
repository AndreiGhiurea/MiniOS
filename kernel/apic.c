#include "apic.h"
#include "mp.h"

extern CPU_STATE gCpuState;

void WriteIOApicRegister(const QWORD apicBase, const BYTE offset, const DWORD val)
{
    /* tell IOREGSEL where we want to write to */
    *(volatile DWORD*)(apicBase) = offset;
    /* write the value to IOWIN */
    *(volatile DWORD*)(apicBase + 0x10) = val;
}

DWORD ReadIOApicRegister(const QWORD apicBase, const BYTE offset)
{
    /* tell IOREGSEL where we want to read from */
    *(volatile DWORD*)(apicBase) = offset;
    /* return the data from IOWIN */
    return *(volatile DWORD*)(apicBase + 0x10);
}

VOID InitApicIrqs()
{
    QWORD val;
    QWORD ioApicAddress = 0;

    for (DWORD i = 0; i < gCpuState.NrOfIoApics; i++)
    {
        ioApicAddress = ((IOAPIC*)gCpuState.IoApics[i])->IOApicAddr;

        // IRQ0 - PIT
        val = 0xff000000000822;
        // WriteIOApicRegister(ioapicAddress, 0x10, val & 0xFFFFFFFF);
        // WriteIOApicRegister(ioapicAddress, 0x11, val >> 32);

        // IRQ1 - Keyboard
        val = 0xff00000000000821;
        WriteIOApicRegister(ioApicAddress, 0x12, val & 0xFFFFFFFF);
        WriteIOApicRegister(ioApicAddress, 0x13, val >> 32);

        // IRQ2 - Why is PIT on IRQ2 ???
        val = 0xFF00000000000820;
        WriteIOApicRegister(ioApicAddress, 0x14, val & 0xFFFFFFFF);
        WriteIOApicRegister(ioApicAddress, 0x15, val >> 32);
    }
}

VOID EnableX2Apic()
{
    unsigned long long msrVal;
    int cpuInfo[4];
    __cpuid(cpuInfo, 0x01);
    if (cpuInfo[3] & (1 << 9) &&
        cpuInfo[2] & (1 << 21))
    {
        msrVal = __readmsr(IA32_APIC_BASE_MSR);
        if (!(msrVal & (1 << 11)))
        {
            // Local APIC not enabled, should be enabled at boot
            gCpuState.x2ApicSupported = FALSE;
            return;
        }
        __writemsr(IA32_APIC_BASE_MSR, msrVal | (1 << 10)); // x2APIC mode enabled

        // Write the Spurious Interrupt Vector Register (SVR) with the good value
        msrVal = __readmsr(APIC_SVR_REG);
        __writemsr(APIC_SVR_REG, msrVal | 0x100); // Set bit 8

        gCpuState.x2ApicSupported = TRUE;
    }
    else
    {
        gCpuState.x2ApicSupported = FALSE;
    }
}