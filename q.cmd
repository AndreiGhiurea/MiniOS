@echo off
".\utils\makeFloppy.py" ".\boot\mbr.asm" ".\boot\ssl.asm" ".\boot\ap.asm"

".\utils\Bochs-2.6.8\bochsdbg.exe" -q -f floppy.bxrc