#include "types.h"
#include "gdt.h"
#include "screen.h"
#include "io.h"
#include "idt.h"

int main(void);
void init_pic(void);
extern char cursorY;
extern char cursorAttr;

void _start(void)
{
    cursorY = 18;
    cursorAttr = 0x5E;

    init_idt();
	print("kernel : idt loaded\n");

	init_pic();
	print("kernel : pic configured\n");


    /* initialisation de la GDT et des segments */
    init_gdt();

    /* Initialisation du pointeur de pile %esp */
    asm("   movw $0x18, %ax \n \
            movw %ax, %ss \n \
            movl $0x20000, %esp");

    main();
}

int main(void)
{
    cursorAttr = 0x4E;
    print("kernel : new gdt loaded !\n");

    show_cursor();

	sti;

	cursorAttr = 0x47;
	print("kernel : allowing interrupt\n");
	cursorAttr = 0x07;


    while (1);
}
