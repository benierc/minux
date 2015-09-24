#ifndef __GDT__
#define __GDT__

#define GDTBASE 0x0
#define GDTSIZE 0xFF

/* segment descriptor */
struct gdtdesc {
    u16 lim15_0;
    u16 base15_0;
    u8 base23_16;
    u8 access;
    u8 lim19_16:4;
    u8 infos:4;
    u8 base31_24;
} __attribute__((packed));

/* GDTR register */
struct gdtr {
    u16 limit;
    u32 base;
} __attribute__((packed));

void init_gdtdesc(u32, u32, u8, u8, struct gdtdesc *);
void init_gdt(void);

struct gdtdesc kgdt[GDTSIZE];
struct gdtr kgdtr;

#endif
