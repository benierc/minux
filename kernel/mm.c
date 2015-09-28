#include "types.h"
#include "mm.h"

char *get_page_frame(void) {
    int byte, bit;
    int page = -1;

    for (byte = 0; byte < RAM_MAXPAGE / 8; byte++) {
        if (mem_bitmap[byte] != 0xFF) {
            for (bit = 0; bit < 8; bit++) {
                if (!(mem_bitmap[byte] & (1 << bit))) {
                    page = 8 * byte + bit;
                    set_page_frame_used(page);
                    return (char *) (page * PAGESIZE);
                }
            }
        }
    }
    return (char *) -1;
}

void init_mm(void)
{
    u32 page_addr = 0;
    int i, pg;

    /* Initialisation du bitmap de pages physiques */
    for (pg = 0; pg < RAM_MAXPAGE / 8; pg++)
                mem_bitmap[pg] = 0;

    /* Pages reservees pour le noyau */
    for (pg = PAGE(0x0); pg < PAGE(0x20000); pg++)
                set_page_frame_used(pg);

    /* Pages reservees pour le hardware */
    for (pg = PAGE(0xA0000); pg < PAGE(0x100000); pg++)
                set_page_frame_used(pg);

    /* set a page for directory table and for page table[0] */
    pd0 = (u32 *)get_page_frame();
    pt0 = (u32 *)get_page_frame();

    /* page directory creation */
    pd0[0] = (u32)pt0 | 3;
    for (i = 1; i < 1024; i++)
        pd0[i] = 0;

    /* page table creation */
    for (i = 0; i < 1024; i++) {
        pt0[i] = page_addr | 3;
        page_addr += 4096;
    }

    asm("   mov %0, %%eax      \n \
            mov %%eax, %%cr3   \n \
            mov %%cr0, %%eax   \n \
            or %1, %%eax       \n \
            mov %%eax, %%cr0" :: "m"(pd0), "i"(PAGING_FLAG));
}

u32 *pd_create_task1(void)
{
    u32 *pd, *pt;
    u32 i;

    pd = (u32 *)get_page_frame();
    for (i = 0; i < 1024; i++)
        pd[i] = 0;

    pt = (u32 *)get_page_frame();
    for (i = 0; i < 1024; i++)
        pt[i] = 0;

    /* Kernel Space */
    pd[0] = pd0[0] | 3;

    /* User Space */
    pd[USER_OFFSET >> 22] = (u32) pt | 7;

    pt[0] = 0x100000 | 7;

    return pd;
}
