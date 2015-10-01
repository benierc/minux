#include "process.h"
#include "gdt.h"

extern struct process p_list[32];
extern struct process *current;
extern int n_proc;

void switch_to_task(int n, int mode)
{
    u16 kss, ss, cs;
    u32 kesp, eflags;

    current = &p_list[n];

    /* load tss */
    default_tss.ss0 = current->kstack.ss0;
    default_tss.esp0 = current->kstack.esp0;

    ss = current->regs.ss;
    cs = current->regs.cs;
    eflags = (current->regs.eflags | 0x200) & 0xFFFFBFFF;

    if (mode == USERMODE) {
        kss = current->kstack.ss0;
        kesp = current->kstack.esp0;
    } else { /* KERNELMODE */
        kss = current->regs.ss;
        kesp = current->regs.esp;
    }
            printk("aa:%d\n",current->pid);

    asm("   mov %0, %%ss; \
            mov %1, %%esp; \
            cmp %[KMODE], %[mode]; \
            je next; \
            push %2; \
            push %3; \
            next: \
            push %4; \
            push %5; \
            push %6; \
            push %7; \
            ljmp $0x08, $do_switch" \
            :: \
            "m" (kss), \
            "m" (kesp), \
            "m" (ss), \
            "m" (current->regs.esp), \
            "m" (eflags), \
            "m" (cs), \
            "m" (current->regs.eip), \
            "m" (current), \
            [KMODE] "i" (KERNELMODE), \
            [mode] "g" (mode));
}

void schedule(void)
{
    struct process *p;
    u32 * stack_ptr;
    u32 esp0, eflags;
    u16 ss, cs;

    /* stock in stack_ptr the pointer to the saved regs */
    asm("mov (%%ebp), %%eax; mov %%eax, %0" : "=m" (stack_ptr) : );

    /* if no charged process and one is ready */
    if (current == 0 && n_proc) {
        switch_to_task(0, USERMODE);
    }
    /*if there is only one process or none */
    else if(n_proc <= 1) {
        return;
    }
    /* if there are at least two process */
    else if(n_proc > 1) {
        /* save current registers */
        current->regs.eflags = stack_ptr[16];
        current->regs.cs  = stack_ptr[15];
        current->regs.eip = stack_ptr[14];
        current->regs.eax = stack_ptr[13];
        current->regs.ecx = stack_ptr[12];
        current->regs.edx = stack_ptr[11];
        current->regs.ebx = stack_ptr[10];
        current->regs.ebp = stack_ptr[8];
        current->regs.esi = stack_ptr[7];
        current->regs.edi = stack_ptr[6];
        current->regs.ds = stack_ptr[5];
        current->regs.es = stack_ptr[4];
        current->regs.fs = stack_ptr[3];
        current->regs.gs = stack_ptr[2];

        if (current->regs.cs != 0x08) {
            current->regs.esp = stack_ptr[17];
            current->regs.ss = stack_ptr[18];
        } else {
            current->regs.esp = stack_ptr[9] + 12;
            current->regs.ss = default_tss.ss0;
        }

        current->kstack.ss0 = default_tss.esp0;
        current->kstack.esp0 = default_tss.ss0;

        /* round robin */
        if (n_proc > current->pid + 1)
            p = &p_list[current->pid + 1];
        else
            p = &p_list[0];

        /* commute */
        if(p->regs.cs != 0x08)
        {
            switch_to_task(p->pid, USERMODE);
        }
        else
        {
            switch_to_task(p->pid, KERNELMODE);
        }
    }
}
