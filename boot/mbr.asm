%define break xchg bx, bx  ; magic breakpoint in bochs
 
[bits 16]                   
[org 7C00h]                ; because the MBR is loaded at 7C00h and runs in 16-bit Real Mode            
BootMain:                  
    ;break                   
    xor    ax,    ax       ; start setting up a context
    mov    ds,    ax      
    mov    es,    ax      
    mov    gs,    ax      
    mov    ss,    ax      
    mov    fs,    ax      
 
    mov    sp,    BootMain ; prepare a stack - the memory from 7BFFh down to 500h is unused - see http://www.brokenthorn.com/Resources/OSDev7.html
 
    mov    ah,    0       
    int    13h             ; reset the boot drive
    
    mov    ah,    02h      ; parameters for calling int13 (ReadSectors)
    mov    al,    9        ; read 9 sectors (hardcoded space for SSL in the floppy)
    mov    ch,    00h     
    mov    cl,    02h      ; starting from sector 2 - skip sector 1 (the MBR)
    mov    dh,    00h     
    mov    bx,    0x7E00   ; memory from 0x7E00 - 0x9FFFF is unused
    int    13h             
    jnc    .success        
  
    cli                    ; we should reset drive and retry, but we hlt
    hlt                     
 
.success:                   
    jmp    00:0x7E00       ; start executing the Second Stage Loader (dl MUST contain the boot drive id)
 
times 510 - ($-$$) db 'B'  ; fill the rest (to 510 bytes) of the sector with padding (we chose B letter from Boot)
db 0x55, 0xAA              ; define the signature of a bootable sector (+2 bytes -> 512)
