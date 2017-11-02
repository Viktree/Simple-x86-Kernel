/*
*  kernel.c
*/

#include "kernel.h"


// current cursor location
unsigned int current_loc = 0;

// Start of video memory in protected mode.
char *vidptr = (char*)0xb8000;

/** setup_PICs
 *
 * ICW1 - PIC wait for 3 more initialization words
 * ICW2 - remap offset address of IDT
 * ICW3 - setup cascading
 * ICW4 - environment info
 *
 */
void setup_pics(void){

    write_port(PIC1_CMD,  ICW1);
    write_port(PIC2_CMD,  ICW1);

    write_port(PIC1_DATA, ICW2);
    write_port(PIC2_DATA, ICW2);

    write_port(PIC1_DATA, ICW3);
    write_port(PIC2_DATA, ICW3);

    write_port(PIC1_DATA, ICW4);
    write_port(PIC2_DATA, ICW4);

    write_port(PIC1_DATA , DISABLE_IRQ);
    write_port(PIC2_DATA , DISABLE_IRQ);
}


void enable_kbord_idt(void){
    unsigned long keyboard_address;

    keyboard_address = (unsigned long) keyboard_handler;
    idt[IDT_KEYBOARD].offset_lowerbits  = LOWER_BITS(keyboard_address);
    idt[IDT_KEYBOARD].offset_higherbits = UPPER_BITS(keyboard_address);
    idt[IDT_KEYBOARD].selector = KERNEL_CODE_SEGMENT_OFFSET;
    idt[IDT_KEYBOARD].zero = 0;
    idt[IDT_KEYBOARD].type_attr = INTERRUPT_GATE;
}


void fill_idt_descriptor(void){
    unsigned long idt_address;
    unsigned long idt_ptr[2];

    idt_address = (unsigned long) idt;
    idt_ptr[0] = (sizeof (struct idt_entry) * IDT_SIZE) + (LOWER_BITS(idt_address) << 16);
    idt_ptr[1] = idt_address >> 16 ;

    load_idt(idt_ptr);
}


void clear_screen(void){
    unsigned int i = 0;
    while(i < SCREENSIZE) {
        vidptr[i++] = ' ';
        vidptr[i++] = BYTE_COLOR_LGREY;
    }
}


void kprint(const char *str){
    unsigned int i = 0;
    while (str[i] != '\0') {
        vidptr[current_loc++] = str[i++];
        vidptr[current_loc++] = BYTE_COLOR_LGREY;
    }
}


void kprint_newline(void){
    current_loc += current_loc + (LINESIZE - current_loc % (LINESIZE));
}


void keyboard_handler_main(void){
    unsigned char status;
    unsigned int buffer_has_stuff;
    char keycode;

    // write End of Interrupt Acknowledgement EOI
    write_port(PIC1_CMD, ICW2);

    status = read_port(KEYBOARD_STATUS_PORT);

    // Lowest bit of status will be set if buffer is not empty
    buffer_has_stuff = status & 0x01;

    if (buffer_has_stuff) {
        keycode = read_port(KEYBOARD_DATA_PORT);

        if(keycode < 0)
            return;

        if(keycode == ENTER_KEY_CODE) {
            kprint_newline();
            return;
        }

        vidptr[current_loc++] = keyboard_map[(unsigned char) keycode];
        vidptr[current_loc++] = BYTE_COLOR_LGREY;
    }
}



void kmain(void){
    const char *mssg = "Vikram's first kernel";

    clear_screen();
    kprint(mssg);

    kprint_newline();
    kprint_newline();

    enable_kbord_idt();
    setup_pics();
    fill_idt_descriptor();

    write_port(PIC1_DATA , IRQ1_ENABLE);

    while(1); // Spin
}