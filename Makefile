all: kernel

kernel: kasm.o kc.o
	ld -m elf_i386 -T link.ld -o kernel kasm.o kc.o

kasm.o: kernel.asm
	nasm -f elf kernel.asm -o kasm.o

kc.o: kernel.c kernel.h keyboard_map.h
	gcc -m32 -fstack-protector -c kernel.c -o kc.o

clean:
	rm -f *.o
	rm -f kernel
