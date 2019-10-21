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
    global _%1W
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
    lidt [rcx]
    ret
    
__irq0:
    call Irq0Handler
    iretq

__irq1:
    call Irq1Handler
    iretq

__irq2:
    call Irq2Handler
    iretq

__irq3:
    call Irq3Handler
    iretq

__irq4:
    call Irq4Handler
    iretq

__irq5:
    call Irq5Handler
    iretq

__irq6:
    call Irq6Handler
    iretq

__irq7:
    call Irq7Handler
    iretq
    
__irq8:
    call Irq8Handler
    iretq

__irq9:
    call Irq9Handler
    iretq

__irq10:
    call Irq10Handler
    iretq

__irq11:
    call Irq11Handler
    iretq

__irq12:
    call Irq12Handler
    iretq

__irq13:
    call Irq13Handler
    iretq

__irq14:
    call Irq14Handler
    iretq

__irq15:
    call Irq15Handler
    iretq

__int3:
    call BreakpointHandler
    iretq

__dumpTrapFrame:
    push rax
    mov rax, cr4
    push rax
    mov rax, cr3
    push rax
    mov rax, cr0
    push rax
    push gs
    push fs
    pushf
    push QWORD __dumpTrapFrame
    push r15
    push r14
    push r13
    push r12
    push r11
    push r10
    push r9
    push r8
    push rsp
    push rbp
    push rdi
    push rsi
    push rdx
    push rcx
    push rbx
    mov rax, [rsp + 8 * 22]
    push rax
    mov rcx, rsp
    sub rsp, 20h
    call DumpTrapFrame
    add rsp, 20h
    add rsp, 8 * 24
    ret

IMPORTFROMC KernelMain, DumpTrapFrame, gIdt, Irq0Handler, Irq1Handler, Irq2Handler, Irq3Handler, Irq4Handler, Irq5Handler, Irq6Handler, Irq7Handler, Irq8Handler, Irq9Handler, Irq10Handler, Irq11Handler, Irq12Handler, Irq13Handler, Irq14Handler, Irq15Handler, BreakpointHandler, DumpTrapFrame
EXPORT2C ASMEntryPoint, __dumpTrapFrame, __interrupt, __halt, __cli, __sti, __magic, __enableSSE, __load_idt, __irq0, __irq1, __irq2, __irq3, __irq4, __irq5, __irq6, __irq7, __irq8, __irq9, __irq10, __irq11, __irq12, __irq13, __irq14, __irq15, __int3