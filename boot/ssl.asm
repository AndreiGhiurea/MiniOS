%define break xchg bx, bx

KERNEL_SECTORS equ 18

[org 0x7E00]
[bits 16]
SSL: 
    mov    ah,    02h               ; parameters for calling int13 (ReadSectors) - read the Kernel
    mov    al,    KERNEL_SECTORS    ; read KERNEL_SECTORS sectors (hardcoded space for Kernel)
    mov    ch,    [cylinder]     
    mov    cl,    01h               ; starting from sector 1 - skip first 18 sectors (the MBR + SSL)
    mov    dh,    [head]     
    mov    bx,    0xA200            ; memory from 0x7E00 - 0xA200 used by SLL;  0xA200 - 0x9FFFF is unused
    int    13h           
    jnc    .success        
    
    cli                    ; we should reset drive and retry, but we hlt
    hlt                     

.success:                  
    cli                    ; starting RM to PM32 transition
    lgdt   [GDT]
    mov    eax,    cr0
    or     al,     1
    mov    cr0,    eax     ; we are in protected mode but we need to set the CS register  
    jmp    SEL_CODE32:.bits32       ; we change the CS to 8 (index of FLAT_DESCRIPTOR_CODE32 entry)

.bits32:
[bits 32]
    mov    ax,    SEL_DATA32       ; index of FLAT_DESCRIPTOR_DATA32 entry
    mov    ds,    ax
    mov    es,    ax      
    mov    gs,    ax      
    mov    ss,    ax      
    mov    fs,    ax 
    
    cld
    mov    ecx,    KERNEL_SECTORS*512  ; KERNEL_SECTORS * sector_size
    mov    esi,    0xA200              ; source
    mov    edi,    [kernelAddr]        ; destination
    rep    movsb                       ; copy the kernel from 0x9200 to 0x200000

    sub    [nrOfReads], BYTE 1
    mov    al, [nrOfReads]
    cmp    al, 0
    je     .continue

    ; Setup next read
    add    [kernelAddr], DWORD 0x2400
    mov    al, [head]
    cmp    al, 1
    je     .incCylinder

    mov    [head], BYTE 1
    jmp    .leaveCylinder

.incCylinder:
    add    [cylinder], BYTE 1
    mov    [head], BYTE 0

.leaveCylinder:
    ; Jump back to 16 bits
    jmp SEL_CODE16:.bits16

.bits16:
[bits 16]
    mov eax, cr0
    and al, 0xFE
    mov cr0, eax

    jmp SEL_NULL:.rmBits16

.rmBits16:  
    mov ax, SEL_NULL
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp SSL

.continue:
[bits 32]
    mov    [ds:0xb8000], BYTE 'O'
    mov    [ds:0xb8002], BYTE 'K'

is_A20_on?:   
    pushad
    mov edi,0x112345  ;odd megabyte address.
    mov esi,0x012345  ;even megabyte address.
    mov [esi],esi     ;making sure that both addresses contain diffrent values.
    mov [edi],edi     ;(if A20 line is cleared the two pointers would point to the address 0x012345 that would contain 0x112345 (edi)) 
    cmpsd             ;compare addresses to see if the're equivalent.
    popad
    jne A20_on        ;if not equivalent , A20 line is set.

    in al, 0x92
    or al, 2
    out 0x92, al        ;enable a20 line
    jmp A20_on

    cli               ;if equivalent , the A20 line is cleared.
    hlt
 
A20_on:
    
    mov    eax, 0x201000    ; the hardcoded ASMEntryPoint of the Kernel
    call   eax
    
    cli                     ; this should not be reached 
    hlt

;
; Data
;
kernelAddr dd 0x200000
nrOfReads  db 10
cylinder   db 0
head       db 1

GDT:
    .limit  dw  GDTTable.end - GDTTable - 1
    .base   dd  GDTTable

FLAT_DESCRIPTOR_CODE32  equ 0x00CF9A000000FFFF  ; Code: Execute/Read
FLAT_DESCRIPTOR_DATA32  equ 0x00CF92000000FFFF  ; Data: Read/Write
FLAT_DESCRIPTOR_CODE16  equ 0x00009B000000FFFF  ; Code: Execute/Read, accessed
FLAT_DESCRIPTOR_DATA16  equ 0x000093000000FFFF  ; Data: Read/Write, accessed
FLAT_DESCRIPTOR_CODE64  equ 0x002F9A000000FFFF  ; Code: Execute/Read
FLAT_DESCRIPTOR_DATA64  equ 0x00CF92000000FFFF  ; Data: Read/Write
    
SEL_NULL                equ 0*8
SEL_CODE32              equ 1*8
SEL_DATA32              equ 2*8
SEL_CODE16              equ 3*8
SEL_DATA16              equ 4*8
SEL_CODE64              equ 5*8
SEL_DATA64              equ 6*8


GDTTable:
    .null     dq 0                         ;  0
    .code32   dq FLAT_DESCRIPTOR_CODE32    ;  8
    .data32   dq FLAT_DESCRIPTOR_DATA32    ; 16
    .code16   dq FLAT_DESCRIPTOR_CODE16    ; 24
    .data16   dq FLAT_DESCRIPTOR_DATA16    ; 32
    .code64   dq FLAT_DESCRIPTOR_CODE64    ; 40
    .data64   dq FLAT_DESCRIPTOR_DATA64    ; 48
    .end: