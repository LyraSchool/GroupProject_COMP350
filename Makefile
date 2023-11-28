# Makefile maintained by Lyra

CFLAGS = -ansi -c -Isrc/


.phony: all
all: bin/kernel bin/bootloader tools/bin/loadfile bin/tstpr1 bin/tstpr2 bin/shell
	dd if=/dev/zero of=diskc.img bs=512 count=1000
	dd if=bin/bootloader of=diskc.img bs=512 count=1 conv=notrunc
	./tools/bin/loadfile bin/kernel diskc.img
	./tools/bin/loadfile message.txt diskc.img
	./tools/bin/loadfile bin/tstpr1 diskc.img
	./tools/bin/loadfile bin/tstpr2 diskc.img
	./tools/bin/loadfile bin/shell diskc.img
#dd if=kernel of=diskc.img bs=512 conv=notrunc seek=3
#dd if=message.txt of=diskc.img bs=512 count=1 seek=30 conv=notrunc
	
# Tools:
tools/bin/loadfile: tools/src/loadfile.c
	gcc -o tools/bin/loadfile tools/src/loadfile.c


# sources:

obj/userlib.o: src/userlib.asm
	mkdir -p obj
	as86 src/userlib.asm -o obj/userlib.o

bin/shell: obj/shell.o obj/userlib.o obj/numbers.o obj/iotools.o
	mkdir -p bin
	ld86 -o bin/shell -d obj/shell.o obj/userlib.o obj/numbers.o obj/iotools.o

obj/shell.o: src/shell.c
	mkdir -p obj
	bcc $(CFLAGS) -o obj/shell.o src/shell.c

bin/tstpr1: obj/tstpr1.o obj/userlib.o
	mkdir -p bin
	ld86 -o bin/tstpr1 -d obj/tstpr1.o obj/userlib.o

bin/tstpr2: obj/tstpr2.o obj/userlib.o
	mkdir -p bin
	ld86 -o bin/tstpr2 -d obj/tstpr2.o obj/userlib.o

obj/tstpr1.o: src/tstpr1.c
	mkdir -p obj
	bcc $(CFLAGS) -o obj/tstpr1.o src/tstpr1.c

obj/tstpr2.o: src/tstpr2.c
	mkdir -p obj
	bcc $(CFLAGS) -o obj/tstpr2.o src/tstpr2.c

obj/iotools.o: src/iotools.c src/iotools.h src/numbers.h
	mkdir -p obj
	bcc $(CFLAGS) -o obj/iotools.o src/iotools.c

obj/numbers.o: src/numbers.c src/numbers.h
	mkdir -p obj
	bcc $(CFLAGS) -o obj/numbers.o src/numbers.c

obj/files.o: src/files.c src/files.h src/asm.h
	mkdir -p obj
	bcc $(CFLAGS) -o obj/files.o src/files.c 

obj/program.o: src/program.c src/program.h src/asm.h
	mkdir -p obj
	bcc $(CFLAGS) -o obj/program.o src/program.c

bin/kernel: obj/kernel_asm.o obj/kernel_c.o obj/files.o obj/numbers.o obj/program.o
	mkdir -p bin
	ld86 -o bin/kernel -d obj/kernel_c.o obj/kernel_asm.o obj/files.o obj/numbers.o obj/program.o

obj/kernel_asm.o: src/kernel.asm
	mkdir -p obj
	as86 src/kernel.asm -o obj/kernel_asm.o

obj/kernel_c.o: src/kernel.c
	mkdir -p obj
	bcc $(CFLAGS) -o obj/kernel_c.o src/kernel.c

bin/bootloader: src/bootloader.asm
	mkdir -p bin
	nasm -o bin/bootloader  src/bootloader.asm

.phony: clean
clean:
	-rm obj/*
	-rm bin/*

.phony: qemu
qemu: all
	qemu-system-x86_64 -drive format=raw,file=diskc.img

.phony: simulator
simulator: all
	java -jar simulator.jar



