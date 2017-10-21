/*
*  link.ld
*/

/**
 * We are producing 32 bit Executable and Linkable Format (ELF)
 */
OUTPUT_FORMAT(elf32-i386)
ENTRY(start)
SECTIONS
 {
   . = 0x100000;
   .text : { *(.text) }
   .data : { *(.data) }
   .bss  : { *(.bss)  }
 }
