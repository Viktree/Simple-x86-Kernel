;; -------------------------------------------------------------
;; kernel.asm
;; -------------------------------------------------------------
;; This code will enable us to write the rest of the start 
;; sequence in C.
;;
;; -------------------------------------------------------------

;; We are using a 32 bit system.
;; This nasm directive is not necessary
bits 32


;; ---( Code Section )------------------------------------------
section .text
	;multiboot spec
	align 4
	dd 0x1BADB002 				;magic
	dd 0x00                  	;flags
	dd - (0x1BADB002 + 0x00)	;checksum. m+f+c should be zero

;; global start tells the linker that start can be found here
;; extern tells the linker the file kmain is found elsewhere
global start
extern kmain					;kmain is defined in the c file

start:
	cli							;block interrupts
	mov esp, stack_space		;set stack pointer
	call kmain
	hlt		 					;halt the CPU, enable interrupts

;; ---( BSS Section )-------------------------------------------
section .bss

;; Reserve some money for the stack pointer (esp)
resb 8192					;8KB for stack
stack_space:

;; -------------------------------------------------------------
