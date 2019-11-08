#include "mp.h"

extern CPU_STATE gCpuState;

VOID FindAllApics()
{
    XSDT *xsdt = NULL;
    RSDP_DESC_EXT *rsdpStruct = (RSDP_DESC_EXT*)0x0E0000;
    for (; (BYTE*)rsdpStruct <= (BYTE*)0x0FFFFF; rsdpStruct = (RSDP_DESC_EXT *)((BYTE*)rsdpStruct + 1))
    {
        if (rsdpStruct->FirstPart.Signature == RSDP_MAGIC)
        {
            xsdt = (XSDT*)rsdpStruct->XsdtAddress;
        }
    }

    DWORD entires = (xsdt->Header.Length - sizeof(xsdt->Header)) / 8;

    MADT *madt = NULL;
    for (DWORD i = 0; i < entires; i++)
    {
        ACPI_SDT_HEADER *h = (ACPI_SDT_HEADER*)xsdt->Pointers[i];
        if (h->Signature == MADT_MAGIC)
        {
            madt = (MADT *)h;
        }
    }

    MADT_START *first = (MADT_START*)((BYTE*)madt + sizeof(MADT));
    DWORD length = sizeof(MADT);

    gCpuState.NrOfIoApics = 0;
    gCpuState.NrOfLapics = 0;
    while (length < madt->Header.Length)
    {
        if (first->Type == 0)
        {
            gCpuState.Lapics[gCpuState.NrOfLapics] = (QWORD)first;
            gCpuState.NrOfLapics++;
        }

        if (first->Type == 1)
        {
            gCpuState.IoApics[gCpuState.NrOfIoApics] = (QWORD)first;
            gCpuState.NrOfIoApics++;
        }

        length += first->Length;
        first = (MADT_START*)((BYTE*)first + first->Length);
    }
}