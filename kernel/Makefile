CC=gcc -Wall -m32 -g
LD=/home/clement/work/efl/OS/minux/binutils/i386-unknown-linux-gnu/bin/ld

all: kernel

kernel: kernel.o screen.o lib.o gdt.o idt.o int.o interrupt.o pic.o syscalls.o mm.o
	$(LD) --oformat binary -Ttext 1000 $^ -o $@

int.o: int.asm
	nasm -f elf -o $@ $^

%.o: %.c
	$(CC)  -c $^

clean:
	rm -f *.o kernel *~
