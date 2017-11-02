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


;; global start tells the linker that following functions can be
;; found here
global start
global keyboard_handler
global read_port
global write_port
global load_idt

;; extern tells the linker the function is declared elsewhere
; The more complicated parts of the code are written in C.
extern kmain
extern keyboard_handler_main

;; This is the minimal start-up sequence
start:
	cli							;block interrupts
	mov esp, stack_space		;set stack pointer
	call kmain
	hlt		 					;halt CPU, enable interrupts


; === ReadPort(edx) ===
; This function will allow us to read I/O ports.
;
; Args
;	edx			port number in lower 16 bits
;
read_port:
	mov edx, [esp + 4]
	in al, dx					; reads dx into al
	ret


; === WritePort(edx, al) ===
; This function will allow us to write to I/O ports.
;
; Args
;	edx			port number in lower 16 bits
;	al			data to be written
;
write_port:
	mov edx, [esp + 4]
	mov al, [esp + 4 + 4]
	out dx, al 					;Writes data to ports
	ret


; === LoadIdt ===
; This function will load an idt struct
;
; Args
;	edx			port number in lower 16 bits
;
load_idt:
	mov edx, [esp + 4]
	lidt [edx]
	sti 						;turn on interrupts
	ret


;; ---( Interrupt Handlers )------------------------------------

; === KeyboardHandler ===
; This function will call keyboard_handler_main on key press
keyboard_handler:
	call    keyboard_handler_main
	iretd


;; ---( BSS Section )-------------------------------------------
section .bss

;; Reserve some money for the stack pointer (esp)
resb 8192					;8KB for stack
stack_space:

;; -------------------------------------------------------------
