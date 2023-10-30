# Makefile maintained by Lyra

CFLAGS = -ansi -c -I.


.phony: all
all: kernel bootloader
	dd if=/dev/zero of=diskc.img bs=512 count=1000
	dd if=bootloader of=diskc.img bs=512 count=1 conv=notrunc
	dd if=kernel of=diskc.img bs=512 conv=notrunc seek=3
	dd if=message.txt of=diskc.img bs=512 count=1 seek=30 conv=notrunc
	

kernel: kernel_asm.o kernel_c.o
	ld86 -o kernel -d kernel_c.o kernel_asm.o

kernel_asm.o: kernel.asm
	as86 kernel.asm -o kernel_asm.o

kernel_c.o: kernel.c
	bcc $(CFLAGS) -o kernel_c.o kernel.c

bootloader: bootloader.asm
	nasm -o bootloader  bootloader.asm

.phony: clean
clean:
	-rm *.o

.phony: qemu
qemu: all
	qemu-system-x86_64 -drive format=raw,file=diskc.img
