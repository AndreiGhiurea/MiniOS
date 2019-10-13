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

IMPORTFROMC KernelMain

TOP_OF_STACK                equ 0x200000
KERNEL_BASE_PHYSICAL        equ 0x200000
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
    
    break

    ;TODO!!! define page tables; see https://wiki.osdev.org ,Intel's manual, http://www.brokenthorn.com/Resources/

    ;TODO!!! activate pagging
    
    ;TODO!!! transition to 64bits-long mode

    MOV     EAX, KernelMain     ; after 64bits transition is implemented the kernel must be compiled on x64
    CALL    EAX
    
    break
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
    
__enableSSE:                ;; enable SSE instructions (CR4.OSFXSR = 1)  
    MOV     EAX, CR4
    OR      EAX, 0x00000200
    MOV     CR4, EAX
    RET
    
EXPORT2C ASMEntryPoint, __cli, __sti, __magic, __enableSSE


