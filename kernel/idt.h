#ifndef __IDT__
#define __IDT__

#define IDTBASE 0x800
#define IDTSIZE 0xFF

#define INTGATE 0x8E00
#define TRAPGATE 0xEF00

/* segment descriptor */
/* task
 * |RESERVED            |P|DPL|0|0|1|0|1|RESERVED|
 * |SEGMENT SELECTOR TSS|RESERVED                |

 * interrupt
 * |SHIFT                    |P|DPL|0|1|1|1|0|0|0|0|RESERVED|
 * |SEGMENT SELECTOR TSS     |SHIFT                         |

 * trap
 * |SHIFT                    |P|DPL|0|1|1|1|1|0|0|0|RESERVED|
 * |SEGMENT SELECTOR TSS     |SHIFT                         |
 */
struct idtdesc {
    u16 offset15_0;
    u16 seg;
    u16 type;
    u16 offset31_24;
} __attribute__((packed));

/* IDTR register */
struct idtr {
    u16 limit;
    u32 base;
} __attribute__((packed));

struct idtdesc kidt[IDTSIZE];
struct idtr kidtr;

void init_idt(void);

#endif
