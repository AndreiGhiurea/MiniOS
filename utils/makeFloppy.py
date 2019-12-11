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
for i in range(1, params):
    #print "Compiling %s"%(sys.argv[i])
    os.system(nasmCommand%(sys.argv[i]))
    out.write(open(tmpPath, "rb").read())


# add padding until sector 18 (hardcoded start for kernel.exe) offset 5120 | 0x1400
size = out.tell()
print("Size of ssl", size)
for i in range(0, ((512*18) - size)):
    out.write('\0'.encode())

if (os.path.getsize(kernelPath) > 55*512):
    print("\n Warning: Kernel size > 55 sectors!\n")
    
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