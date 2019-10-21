;;-----------------_DEFINITIONS ONLY_-----------------------
;; IMPORT FUNCTIONS FROM C
%macro IMPORTFROMC 1-*
%rep  %0
    %ifidn __OUTPUT_FORMAT__, win32 ; win32 builds from Visual C decorate C names using _ 
    extern _%1
    %1 equ _%1
    %else
    extern %1
    %endif
%rotate 1 
%endrep
%endmacro

;; EXPORT TO C FUNCTIONS
%macro EXPORT2C 1-*
%rep  %0
    %ifidn __OUTPUT_FORMAT__, win32 ; win32 builds from Visual C decorate C names using _ 
    global _%1
    _%1 equ %1
    %else
    global %1
    %endif
%rotate 1 
%endrep
%endmacro

%define break xchg bx, bx

TOP_OF_STACK                equ 0x200000
KERNEL_BASE_PHYSICAL        equ 0x200000
SEL_NULL                    equ 0*8
SEL_CODE32                  equ 1*8
SEL_DATA32                  equ 2*8
SEL_CODE16                  equ 3*8
SEL_DATA16                  equ 4*8
SEL_CODE64                  equ 5*8
SEL_DATA64                  equ 6*8
;;-----------------^DEFINITIONS ONLY^-----------------------

segment .text
[BITS 32]
ASMEntryPoint:
    cli
    MOV     DWORD [0x000B8000], 'O1S1'
%ifidn __OUTPUT_FORMAT__, win32
    MOV     DWORD [0x000B8004], '3121'                  ; 32 bit build marker
%else
    MOV     DWORD [0x000B8004], '6141'                  ; 64 bit build marker
%endif

    MOV     ESP, TOP_OF_STACK                           ; just below the kernel

    ;; Define page-tables here
    mov edi, 0x00205000                         ;; PML4
    mov [edi], DWORD 0x00206007                 ;; 0-512GB
    mov [edi + 4], DWORD 0x00000000
    add edi, 8

    xor eax, eax                                ;; Fill the rest entries with 0x0
    mov ecx, 1022
    rep stosd

    mov edi, 0x00206000                         ;; PDPT
    mov [edi], DWORD 0x00207007                 ;; 0-1GB
    mov [edi + 4], DWORD 0x00000000
    add edi, 8

    xor eax, eax                                ;; Fil the remaining entries with 0x0
    mov ecx, 1022
    rep stosd

    mov edi, 0x00207000                         ;; PDT - with bit 7 set, pages are now 2MB
    mov [edi], DWORD 0x00000087                 ;; 0-2MB identity mapped
    mov [edi + 4], DWORD 0x00000000
    add edi, 8

    mov [edi], DWORD 0x00200087                 ;; 2-4MB identity mapped
    mov [edi + 4], DWORD 0x0
    add edi, 8
     
    mov [edi], DWORD 0x00400087                 ;; 4-6MB identity mapped
    mov [edi + 4], DWORD 0x00000000
    add edi, 8

    xor eax, eax                                ;; Fill the rest of entries with 0x0
    mov ecx, 1018
    rep stosd

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

[BITS 64]
Realm64:
    ; We're now in 64 long mode

    mov     ax, SEL_DATA64
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    call KernelMain
    
    CLI
    HLT

;;--------------------------------------------------------

__cli:
    CLI
    RET

__sti:
    STI
    RET

__magic:
    XCHG    BX,BX
    RET

__interrupt:
    int3
    ret
    
__halt:
    hlt
    ret

__enableSSE:                ;; enable SSE instructions (CR4.OSFXSR = 1)  
    XOR     RAX, RAX
    MOV     RAX, CR4
    OR      RAX, 0x00000200
    MOV     CR4, RAX
    RET

__load_idt:
    break
    lidt [rcx]
    ret
    
irq0:
    call irq0_handler
    iretq

irq1:
    call irq1_handler
    iretq

irq2:
    call irq2_handler
    iretq

irq3:
    call irq3_handler
    iretq

irq4:
    call irq4_handler
    iretq

irq5:
    call irq5_handler
    iretq

irq6:
    call irq6_handler
    iretq

irq7:
    call irq7_handler
    iretq
    
irq8:
    call irq8_handler
    iretq

irq9:
    call irq9_handler
    iretq

irq10:
    call irq10_handler
    iretq

irq11:
    call irq11_handler
    iretq

irq12:
    call irq12_handler
    iretq

irq13:
    call irq13_handler
    iretq

irq14:
    call irq14_handler
    iretq

irq15:
    call irq15_handler
    iretq

breakpoint:
    ; call breakpoint_handler <- This breaks the stack
    iretq

IMPORTFROMC KernelMain, gIdt, irq0_handler, irq1_handler, irq2_handler, irq3_handler, irq4_handler, irq5_handler, irq6_handler, irq7_handler, irq8_handler, irq9_handler, irq10_handler, irq11_handler, irq12_handler, irq13_handler, irq14_handler, irq15_handler, breakpoint_handler
EXPORT2C ASMEntryPoint, __interrupt, __halt, __cli, __sti, __magic, __enableSSE, __load_idt, irq0, irq1, irq2, irq3, irq4, irq5, irq6, irq7, irq8, irq9, irq10, irq11, irq12, irq13, irq14, irq15, breakpoint