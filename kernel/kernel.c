#include "types.h"
#include "lib.h"
#include "gdt.h"
#include "screen.h"
#include "io.h"
#include "idt.h"
#include "mm.h"

void init_pic(void);
int main(void);

extern char cursorY;
extern char cursorAttr;

void _start(void)
{
    cursorY = 16;
    cursorAttr = 0x0E;

    /* Initialisation de la GDT et des segments */
    init_gdt();

    /* Initialisation du pointeur de pile %esp */
    asm("   movw $0x18, %ax \n \
            movw %ax, %ss \n \
            movl $0x20000, %esp");

    main();
}

void task1(void)
{
    char *msg = (char *) 0x40000100;	/* le message sera stocké en 0x100100 */
    msg[0] = 't';
    msg[1] = 'a';
    msg[2] = 's';
    msg[3] = 'k';
    msg[4] = '1';
    msg[5] = '\n';
    msg[6] = 0;

    asm("mov %0, %%ebx; mov $0x01, %%eax; int $0x30"::"m"(msg));

    while (1);
    return;			/* never go there */
}

int main(void)
{
    u32 *pd;

    printk("kernel : gdt loaded\n");

    init_idt();
    printk("kernel : idt loaded\n");

    init_pic();
    printk("kernel : pic configured\n");

    hide_cursor();

    /* Initialisation du TSS */
    asm("	movw $0x38, %ax \n \
            ltr %ax");
    printk("kernel : tr loaded\n");

    init_mm();
    printk("kernel : paging enable\n");

    pd = pd_create_task1();
    memcpy((char *) 0x100000, (char *) &task1, 100);	/* copie de 100 instructions */
    printk("kernel : task created\n");

    cursorAttr = 0x47;
    printk("kernel : trying switch to user task...\n");
    cursorAttr = 0x07;
    asm ("   cli \n \
            movl $0x20000, %0 \n \
            movl %1, %%eax \n \
            movl %%eax, %%cr3 \n \
            push $0x33 \n \
            push $0x40000F00 \n \
            pushfl \n \
            popl %%eax \n \
            orl $0x200, %%eax \n \
            and $0xFFFFBFFF, %%eax \n \
            push %%eax \n \
            push $0x23 \n \
            push $0x40000000 \n \
            movw $0x2B, %%ax \n \
            movw %%ax, %%ds \n \
            iret" : "=m"(default_tss.esp0) : "m"(pd));

    while (1);
}
