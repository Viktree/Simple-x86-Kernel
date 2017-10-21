/*
*  kernel.c
*/

#define LINES 25
#define COL 80
#define BYTES_PER_COL 2

/** Attribute Byte Colour Map
 * 	0		Black
 * 	1		Blue
 *	2		Green
 * 	3		Cyan
 *	4		Red
 * 	5		Magenta
 * 	6		Brown
 * 	7		Light Grey
 * 	8		Dark Grey
 * 	9		Light Blue
 * 	10/a 	Light Green
 * 	11/b 	Light Cyan
 * 	12/c 	Light Red
 * 	13/d 	Light Magenta
 * 	14/e 	Light Brown
 * 	15/f 	White
 */


void kmain(void){

	const char *str = "Vikram's first kernel";
	char *vidptr = (char*)0xb8000; // Start of video memory in protected mode.
	unsigned int i = 0, j = 0;

	// Loop that clears the screen
	while(j < LINES * COL * BYTES_PER_COL) {
		vidptr[j] = ' '; 		// Blank Character
		vidptr[j+1] = 0x07; 	// attribute-byte - light grey on black screen
		j = j + 2;
	}

	j = 0;

	// this loop writes the string to video memory
	while(str[j] != '\0') {
		// the character's ascii
		vidptr[i] = str[j];
		// attribute-byte: give character black bg and light grey fg
		vidptr[i+1] = 0x07;
		++j;
		i = i + 2;
	}
}