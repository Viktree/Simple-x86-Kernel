#include "keyboard_map.h"


#define LINES         25
#define COL           80
#define BYTES_PER_COL 2

#define LINESIZE      COL * BYTES_PER_COL
#define SCREENSIZE    LINES * LINESIZE

#define BYTE_COLOR_LGREY    0x07


#define KEYBOARD_DATA_PORT   0x60
#define KEYBOARD_STATUS_PORT 0x64

#define IDT_KEYBOARD 0x21
#define IDT_SIZE     256

#define DISABLE_IRQ		  0xFF
#define IRQ1_ENABLE 	  0xFD // Keyboard interrupts

#define INTERRUPT_GATE             0x8e
#define KERNEL_CODE_SEGMENT_OFFSET 0x08


#define PIC1_CMD  0x20
#define PIC2_CMD  0xA0
#define PIC1_DATA 0x21
#define PIC2_DATA 0xA1

#define ICW1 0x11 // PIC wait for 3 more words
#define ICW2 0x20 // Remap offset address of IDT
#define ICW3 0x00 // Setup cascading
#define ICW4 0x01 // Environment info


#define UPPER_BITS(addr) ((addr & 0xffff0000) >> 16)
#define LOWER_BITS(addr) (addr & 0xffff)


// Get the initializations from kernel.asm
extern unsigned char keyboard_map[128];
extern void keyboard_handler(void);
extern char read_port(unsigned short port);
extern void write_port(unsigned short port, unsigned char data);
extern void load_idt(unsigned long *idt_ptr);


// These two functions are used by kernel.asm
void kmain(void);
void keyboard_handler_main(void);


struct idt_entry{
    unsigned short int offset_higherbits;
    unsigned short int offset_lowerbits;
    unsigned short int selector;
    unsigned char zero;
    unsigned char type_attr;
};

struct idt_entry idt[IDT_SIZE];


