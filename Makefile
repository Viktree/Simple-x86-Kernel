all: kernel

kernel: kasm.o kc.o
	ld -m elf_i386 -T link.ld -o kernel kasm.o kc.o

kasm.o: kernel.asm
	nasm -f elf kernel.asm -o kasm.o

kc.o: kernel.c
	gcc -m32 -c kernel.c -o kc.o

clean:
	rm *.o
	rm kernel
