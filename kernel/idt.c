#include "types.h"
#include "lib.h"
#include "idt.h"
#include "io.h"

void _asm_default_int(void);
void _asm_irq_0(void);
void _asm_irq_1(void);

void init_idt_desc(u16 seg, u32 offset, u16 type, struct idtdesc *desc)
{
    desc->offset15_0 = offset & 0xFFFF;
    desc->seg = seg;
    desc->type = type;
    desc->offset31_24 = (offset & 0xFFFF0000) >> 16;
    return;
}

void init_idt(void)
{
    int i;
    for (i = 0; i < IDTSIZE; i++)
        init_idt_desc(0x08, (u32) _asm_default_int, INTGATE, &kidt[i]);

    init_idt_desc(0x08, (u32) _asm_irq_0, INTGATE, &kidt[32]); //horloge
    init_idt_desc(0x08, (u32) _asm_irq_1, INTGATE, &kidt[33]); //clavier

    /* init IDTR structure */
    kidtr.limit = IDTSIZE * 8;
    kidtr.base = IDTBASE;

    /* copy IDT to its address */
    memcpy((char *) kidtr.base, (char *) kidt, kidtr.limit);

    /* load IDTR register */
    asm("lidtl (kidtr)");
}
