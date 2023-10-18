CFLAGS = -ansi -c -I./src/


.phony: all
all: obj/kernel obj/bootloader
	mkdir -p bin
	dd if=/dev/zero of=bin/diskc.img bs=512 count=1000
	dd if=obj/bootloader of=bin/diskc.img bs=512 count=1 conv=notrunc
	dd if=obj/kernel of=bin/diskc.img bs=512 conv=notrunc seek=3
	

obj/kernel: obj/kernel_asm.o obj/kernel_c.o obj/strings.o
	ld86 -o obj/kernel -d obj/kernel_c.o obj/kernel_asm.o obj/strings.o

obj/kernel_asm.o: src/kernel.asm
	mkdir -p obj
	as86 src/kernel.asm -o obj/kernel_asm.o

obj/kernel_c.o: src/kernel.c
	mkdir -p obj
	bcc $(CFLAGS) -o obj/kernel_c.o src/kernel.c
	# bcc -ansi -c -o obj/kernel_c.o src/kernel.c

# Kernel Modules

obj/strings.o: src/strings.c
	mkdir -p obj
	bcc $(CFLAGS) -o obj/strings.o src/strings.c
# End Kernel Modules



obj/bootloader: src/bootloader.asm
	nasm -o obj/bootloader  src/bootloader.asm

.phony: clean
clean:
	-rm -r obj bin

.phony: qemu
qemu: all
	qemu-system-x86_64 -drive format=raw,file=bin/diskc.img




