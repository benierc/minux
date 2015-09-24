CC=gcc -m32
OBJ=floppyA
LD=/home/clement/work/efl/OS/BootSector_kernelC/binutils/i386-unknown-linux-gnu/bin/ld

all: $(OBJ)

floppyA: bootsect kern
	cat boot/bootsect kernel/kernel /dev/zero | dd of=floppyA bs=512 count=2880

bootsect:
	make -C boot

kern:
	make -C kernel

clean:
	rm -f $(OBJ) *~
	make clean -C boot
	make clean -C kernel

run:
	qemu-system-i386 -boot a -fda floppyA
