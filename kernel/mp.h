#pragma once
#include "main.h"

#define RSDP_MAGIC      0x2052545020445352
#define MADT_MAGIC      0x43495041

VOID FindAllApics();

#pragma pack(push, 1)
typedef struct _RSDP_DESC {
    QWORD Signature;
    BYTE Checksum;
    CHAR OEMID[6];
    BYTE Revision;
    DWORD RsdtAddress;
} RSDP_DESC, *PRSDP_DESC;

typedef struct _RSDP_DESC_EXT {
    RSDP_DESC FirstPart;
    DWORD Length;
    QWORD XsdtAddress;
    BYTE ExtendedChecksum;
    BYTE Reserved[3];
} RSDP_DESC_EXT, *PRSDP_DESC_EXT;

typedef struct _ACPI_SDT_HEADER {
    DWORD Signature;
    DWORD Length;
    BYTE Revision;
    BYTE Checksum;
    CHAR OEMID[6];
    CHAR OEMTableID[8];
    DWORD OEMRevision;
    DWORD CreatorID;
    DWORD CreatorRevision;
} ACPI_SDT_HEADER, *PACPI_SDT_HEADER;

typedef struct _XSDT {
    ACPI_SDT_HEADER Header;
    QWORD Pointers[64];
} XSDT, *PXSDT;

typedef struct _MADT_START {
    BYTE Type;
    BYTE Length;
} MADT_START, *PMADT_START;

typedef struct _LAPIC {
    MADT_START H;
    BYTE ProcessorID;
    BYTE ApicID;
    DWORD Flags;
} LAPIC, *PLAPIC;

typedef struct _IOAPIC {
    MADT_START H;
    BYTE IOapicID;
    BYTE Reserved;
    DWORD IOApicAddr;
    DWORD GlobalSysInterruptBase;
} IOAPIC, *PIOAPIC;

typedef struct _MADT {
    ACPI_SDT_HEADER Header;
    DWORD LAPICAddr;
    DWORD Flags;
} MADT, *PMADT;
#pragma pack(pop)