#include "lib.h"
#include "types.h"
#include "mm.h"

#include "process.h"

struct process p_list[32];
struct process *current = 0;
int n_proc = 0;

void load_task(u32 *code_phys_addr, u32 *fn, unsigned int code_size)
{
    u32 pages, page_base, kstack_base;
    u32 *pd;
    int i;

    memcpy((char *)code_phys_addr,(char *)fn, code_size);

    /* update bitmap */
    page_base = (u32)PAGE(code_phys_addr);

    if (code_size % PAGESIZE)
        pages = code_size / PAGESIZE + 1;
    else
        pages = code_size / PAGESIZE;

    for(i = 0; i < pages; i++)
        set_page_frame_used(page_base + i);

    /* create dir and page tables */
    pd = pd_create(code_phys_addr, code_size);

    kstack_base = (u32) get_page_frame;
    if (kstack_base > 0x400000) {
        printk("not enough memory for kernel stack\n");
        return;
    }

    /* init regs */
    p_list[n_proc].pid = n_proc;
    p_list[n_proc].regs.ss = 0x33;
    p_list[n_proc].regs.esp = 0x40001000;
    p_list[n_proc].regs.eflags = 0x0;
    p_list[n_proc].regs.cs = 0x23;
    p_list[n_proc].regs.eip = 0x40000000;
    p_list[n_proc].regs.ds = 0x2B;
    p_list[n_proc].regs.es = 0x2B;
    p_list[n_proc].regs.fs = 0x2B;
    p_list[n_proc].regs.gs = 0x2B;
    p_list[n_proc].regs.cr3 = (u32) pd;

    p_list[n_proc].kstack.ss0 = 0x18;
    p_list[n_proc].kstack.esp0 = kstack_base + PAGESIZE;

    p_list[n_proc].regs.eax = 0;
    p_list[n_proc].regs.ecx = 0;
    p_list[n_proc].regs.edx = 0;
    p_list[n_proc].regs.ebx = 0;

    p_list[n_proc].regs.ebp = 0;
    p_list[n_proc].regs.esi = 0;
    p_list[n_proc].regs.edi = 0;

    n_proc++;
}

