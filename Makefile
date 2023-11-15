# Makefile maintained by Lyra

CFLAGS = -ansi -c -I.


.phony: all
all: kernel bootloader loadfile tstpr1 tstpr2 shell
	dd if=/dev/zero of=diskc.img bs=512 count=1000
	dd if=bootloader of=diskc.img bs=512 count=1 conv=notrunc
	./loadfile kernel
	./loadfile message.txt
	./loadfile tstpr1
	./loadfile tstpr2
	./loadfile shell
#dd if=kernel of=diskc.img bs=512 conv=notrunc seek=3
#dd if=message.txt of=diskc.img bs=512 count=1 seek=30 conv=notrunc
	
# Tools:
loadfile: loadfile.c
	gcc -o loadfile loadfile.c


# sources:

userlib.o: userlib.asm
	as86 userlib.asm -o userlib.o

shell: shell.o
	ld86 -o shell -d shell.o userlib.o

shell.o: shell.c
	bcc $(CFLAGS) -o shell.o shell.c

tstpr1: tstpr1.o userlib.o
	ld86 -o tstpr1 -d tstpr1.o userlib.o

tstpr2: tstpr2.o userlib.o
	ld86 -o tstpr2 -d tstpr2.o userlib.o

tstpr1.o: tstpr1.c
	bcc $(CFLAGS) -o tstpr1.o tstpr1.c

tstpr2.o: tstpr2.c
	bcc $(CFLAGS) -o tstpr2.o tstpr2.c

numbers.o: numbers.c numbers.h
	bcc $(CFLAGS) -o numbers.o numbers.c

files.o: files.c files.h asm.h
	bcc $(CFLAGS) -o files.o files.c 

program.o: program.c program.h asm.h
	bcc $(CFLAGS) -o program.o program.c

kernel: kernel_asm.o kernel_c.o files.o numbers.o program.o
	ld86 -o kernel -d kernel_c.o kernel_asm.o files.o numbers.o program.o

kernel_asm.o: kernel.asm
	as86 kernel.asm -o kernel_asm.o

kernel_c.o: kernel.c
	bcc $(CFLAGS) -o kernel_c.o kernel.c

bootloader: bootloader.asm
	nasm -o bootloader  bootloader.asm

.phony: clean
clean:
	-rm *.o
	-rm bootloader
	-rm kernel
	-rm loadfile
	-rm tstpr1
	-rm tstpr2
	-rm shell

.phony: qemu
qemu: all
	qemu-system-x86_64 -drive format=raw,file=diskc.img

.phony: simulator
simulator: all
	java -jar simulator.jar



