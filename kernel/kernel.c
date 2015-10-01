#include "types.h"
#include "lib.h"
#include "gdt.h"
#include "screen.h"
#include "io.h"
#include "idt.h"
#include "mm.h"
#include "process.h"

void init_pic(void);
int main(void);

extern char cursorY;
extern char cursorAttr;
extern struct process p_list[32];
extern struct process *current;
extern int n_proc;

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
char *msg = (char *) 0x40001000;
    int i;

    msg[0] = 't';
    msg[1] = 'a';
    msg[2] = 's';
    msg[3] = 'k';
    msg[4] = '1';
    msg[5] = '\n';
    msg[6] = 0;

    while (1) {
        asm("mov %0, %%ebx; mov $0x01, %%eax; int $0x30":: "m"(msg));
        for (i = 0; i < 1000000; i++);
    }

    return;                 /* never go there */
}

void task2(void)
{
    char *msg = (char *) 0x40001000;
    int i;

    msg[0] = 't';
    msg[1] = 'a';
    msg[2] = 's';
    msg[3] = 'k';
    msg[4] = '2';
    msg[5] = '\n';
    msg[6] = 0;

    while (1) {
        asm("mov %0, %%ebx; mov $0x01, %%eax; int $0x30":: "m"(msg));
        for (i = 0; i < 1000000; i++);
    }

    return;                 /* never go there */
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


    load_task((u32 *) 0x100000, (u32 *) & task1, 0x2000);
    load_task((u32 *) 0x200000, (u32 *) & task2, 0x2000);

    cursorAttr = 0x47;
    printk("kernel : scheduler enable\n");
    cursorAttr = 0x07;
    sti;

    while (1);
}
