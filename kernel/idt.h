#ifndef __IDT__
#define __IDT__

#define IDTBASE 0x800
#define IDTSIZE 0xFF

#define INTGATE 0x8E00

/* segment descriptor */
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
