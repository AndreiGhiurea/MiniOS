[org 0x8000] ; This is linked at 0x8000, one sector after SSL
[bits 16]

TOP_OF_STACK                equ 0x200000
KERNEL_BASE_PHYSICAL        equ 0x200000

APStartUpStart:
    xor    ax,    ax       ; start setting up a context
    mov    ds,    ax      
    mov    es,    ax      
    mov    gs,    ax      
    mov    ss,    ax      
    mov    fs,    ax      
 
    mov    sp,    0x7C00 ; prepare a stack - the memory from 7BFFh down to 500h is unused - see http://www.brokenthorn.com/Resources/OSDev7.html

    cli
    lgdt [0x7900]
    mov eax, cr0
    or al, 1
    mov cr0, eax

    jmp SEL_CODE32:.bits32

.bits32:
[bits 32]
    mov al, [0x200]
    add al, 1
    mov [0x200], al
    mov esp, TOP_OF_STACK 

    mov ax, SEL_DATA32
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov ss, ax
    mov gs, ax

    mov eax, cr4
    or eax, 1 << 5               ; Set PAE bit in CR4
    mov cr4, eax

    mov ecx, 0xC0000080          ; Set the C-register to 0xC0000080, which is the EFER MSR.
    rdmsr                        ; Read from the model-specific register.
    or eax, 1 << 8               ; Set the LM-bit which is the 9th bit (bit 8).
    wrmsr 

    mov eax, 0x205000            ; PML4 Table Address
    mov cr3, eax

    mov eax, cr0
    or eax, 1 << 31              ; Set PG-bit
    mov cr0, eax

    ; We're now in 32 compatiblity mode
    cli
    jmp SEL_CODE64:Realm64

    hlt
[BITS 64]
Realm64:
    ; We're now in 64 long mode
    mov     ax, SEL_DATA64
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax
    mov     ss, ax

    mov al, 1
    mov [0x200], al

    call 0x201000
    
    CLI
    HLT
APStartUpEnd:


SEL_NULL                equ 0*8
SEL_CODE32              equ 1*8
SEL_DATA32              equ 2*8
SEL_CODE16              equ 3*8
SEL_DATA16              equ 4*8
SEL_CODE64              equ 5*8
SEL_DATA64              equ 6*8