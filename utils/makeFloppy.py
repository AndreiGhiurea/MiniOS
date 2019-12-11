import os, sys, time

floppyPath    = ".\\bin\\floppy.img"
flpSize       = 1474560
tmpPath       = ".\\bin\\tmp.bin"
nasmCommand   = ".\\utils\\nasm -fbin %s -o " + tmpPath
kernelPath    = ".\\bin\\kernel.exe"

params = len(sys.argv)
if (params < 2):
    #print "Usage: \n %s file1.asm [file2.asm] [... fileN.asm]\n"%(sys.argv[0])
    exit(1)

#we have cmd line, open the result file (floppy)
out = open(floppyPath, "wb")
    
#iterate files and compile them and add them to the floppy
for i in range(1, params - 1):
    os.system(nasmCommand%(sys.argv[i]))
    out.write(open(tmpPath, "rb").read())
    print("Assembling: ", sys.argv[i])

# add padding until sector 10 (hardcoded start for kernel.exe) offset 0x9000
# we now have 8 more sectors free for AP code, because in SSL we read the kernel starting from sector 18
size = out.tell()
for i in range(0, ((512*2) - size)):
    out.write('\0'.encode())

for i in range(params - 1, params):
    os.system(nasmCommand%(sys.argv[i]))
    out.write(open(tmpPath, "rb").read())
    print("Assembling: ", sys.argv[i])

# add 8 more sectors after AP code to reach 18
size = out.tell()
for i in range(0, ((512*18) - size)):
    out.write('\0'.encode())

if (os.path.getsize(kernelPath) > 180*512):
    print("\n Warning: Kernel size > 180 sectors!\n")
    
#write the kernel
out.write(open(kernelPath, "rb").read())


# add padding to the end of floppy
time.sleep(1)  
size = out.tell()
#print "Adding padding from %d to %d ...\n"%(size, flpSize)
for i in range(0, (flpSize - size)):
    out.write('\0'.encode())

#cleanup
out.close()
os.remove(tmpPath)