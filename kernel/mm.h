#ifndef __MM__
#define __MM__

#define PAGESIZE 4096
#define RAM_MAXPAGE 0x10000

#define PAGE(addr) addr >> 12

#define PAGING_FLAG     0x80000000      /* CR0 - bit 31 */
#define USER_OFFSET 0x40000000
#define USER_STACK  0xE0000000

u32 *pd0; /* kernel page directory */
u32 *pt0; /* kernel page table */
u8 mem_bitmap[RAM_MAXPAGE / 8]; /* bitmap allocation de pages */

/* set a page as used/free in bitmap */
#define set_page_frame_used(page) mem_bitmap[((u32)page) / 8] |= (1 << (((u32)page) % 8))
#define release_page_frame(p_addr) mem_bitmap[(((u32) p_addr)/PAGESIZE) / 8] &= ~(1 << (((u32) p_addr) / PAGESIZE) % 8)

void init_mm(void);

/* select a free page in bitmap */
char *get_page_frame(void);

/* create a table directory for a task */
u32 *pd_create_task1(void);

#endif
