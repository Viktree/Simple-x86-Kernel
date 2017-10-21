all: kernel

kernel: kasm.o kc.o
	ld -macosx_version_min 10.12 -arch i386 -o kernel kasm.o kc.o

kasm.o: kernel.asm
	nasm -f elf kernel.asm -o kasm.o

kc.o: kernel.c
	gcc -m32 -c kernel.c -o kc.o

clean:
	rm *.o
