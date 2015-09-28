#include "types.h"
#include "gdt.h"
#include "screen.h"
#include "io.h"
#include "idt.h"
#include "lib.h"
#include "mm.h"

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
    print("gdt configured\n");
    asm("   movw $0x38, %ax \n \
            ltr %ax");
    print("ldt configured\n");

    /* Initialisation du pointeur de pile %esp */
    asm("   movw $0x18, %ax \n \
            movw %ax, %ss \n \
            movl $0x20000, %esp");

    init_mm();

    main();
}

void task1(void)
{
    char *msg = (char *) 0x100; /* message at 0x30100 */
    msg[0] = 't';
    msg[1] = 'a';
    msg[2] = 's';
    msg[3] = 'k';
    msg[4] = '1';
    msg[5] = '\n';
    msg[6] = '\0';

    asm("mov %0, %%ebx; mov $0x01, %%eax; int $0x30"::"m"(msg)); /* call syscalls, num 1 */
    while(1);
    return;
}

int main(void)
{
    //hide_cursor();

    memcpy((char *)0x30000, &task1, 100);

    ///* EFLAGS
    // * bit9 Interupt flags(IF) à 1
    // * bit14 Nested Task (NT) à 0 */
    ///* CS */
    ///* update default_tss.esp0 */
    ///* data segment user mode: 0x28 + 3*/
    asm("   cli \n \
            push $0x33 \n \
            push $0x30000 \n \
            pushfl \n \
            popl %%eax \n \
            orl $0x200, %%eax \n \
            and $0xFFFFBFFF, %%eax \n \
            push %%eax \n \
            push $0x23 \n \
            push $0x0 \n \
            movl $0x20000, %0 \n \
            movw $0x2B, %%ax \n \
            movw %%ax, %%ds \n \
            iret": "=m"(default_tss.esp0):);

        /* never reached ! */
        print("Critical error, halting system\n");
    asm("hlt");
}
