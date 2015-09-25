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

/* Task State Segment - TSS */
struct tss {
    u16 previous_task, __previous_task_unused;
    u32 esp0;
    u16 ss0, __ss0_unused;
    u32 esp1;
    u16 ss1, __ss1_unused;
    u32 esp2;
    u16 ss2, __ss2_unused;
    u32 cr3;
    u32 eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
    u16 es, __es_unused;
    u16 cs, __cs_unused;
    u16 ss, __ss_unused;
    u16 ds, __ds_unused;
    u16 fs, __fs_unused;
    u16 gs, __gs_unused;
    u16 ldt_select, __ldt_select_unused;
    u16 debug_flag, io_map;
} __attribute__((packed));

void init_gdtdesc(u32, u32, u8, u8, struct gdtdesc *);
void init_gdt(void);

struct gdtdesc kgdt[GDTSIZE];
struct gdtr kgdtr;
struct tss default_tss;

#endif
