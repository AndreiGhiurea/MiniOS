@echo off

copy %2 ".\..\bin\kernel.exe"
cd ..
".\utils\makeFloppy.py" ".\boot\mbr.asm" ".\boot\ssl.asm" ".\boot\ap.asm"
echo Compile Complete

