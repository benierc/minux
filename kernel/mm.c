#include "types.h"

#define PAGING_FLAG     0x80000000      /* CR0 - bit 31 */

#define PD0_ADDR 0x20000        /* addr. page directory kernel */
#define PT0_ADDR 0x21000        /* addr. page table[0] kernel */

void init_mm(void)
{
    u32 *pd0; /* kernel page directory */
    u32 *pt0; /* kernel page table */
    u32 page_addr = 0;
    int i;

    /* page directory creation */
    pd0 = (u32 *)PD0_ADDR;
    pd0[0] = PT0_ADDR | 3;
    for (i = 1; i < 1024; i++)
        pd0[i] = 0;

    /* page table creation */
    pt0 = (u32 *) PT0_ADDR;
    for (i = 0; i < 1024; i++) {
        pt0[i] = page_addr | 3;
        page_addr += 4096;
    }

    asm("   mov %0, %%eax    \n \
            mov %%eax, %%cr3 \n \
            mov %%cr0, %%eax \n \
            or %1, %%eax     \n \
            mov %%eax, %%cr0" :: "i"(PD0_ADDR), "i"(PAGING_FLAG));
}
