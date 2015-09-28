#include "process.h"
#include "gdt.h"

extern struct process p_list[32];
extern struct process *current;
extern int n_proc;

void schedule(void)
{
    u32 * stack_ptr;
    u32 esp0, eflags;
    u16 ss, cs;

    /* stock in stack_ptr the pointer to the saved regs */
    asm("mov (%%ebp), %%eax; mov %%eax, %0" : "=m" (stack_ptr) : );

    /* if no charged process and one is ready */
    if (current == 0 && n_proc) {
        current = &p_list[0];
    }
    /*if there is only one process or none */
    else if(n_proc <= 1) {
        return;
    }
    /* if there are at least two process */
    else if(n_proc >= 1) {
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

        current->regs.esp = stack_ptr[17];
        current->regs.ss = stack_ptr[18];

        default_tss.esp0 = (u32) (stack_ptr + 19);

        /* round robin */
        if (n_proc > current->pid + 1)
            current = &p_list[current->pid + 1];
        else
            current = &p_list[0];
    }
    ss = current->regs.ss;
    cs = current->regs.cs;
    eflags = (current->regs.eflags | 0x200) & 0xFFFFBFFF;
    esp0 = default_tss.esp0;

    asm("   mov %0, %%esp; \
            push %1; \
            push %2; \
            push %3; \
            push %4; \
            push %5; \
            push %6; \
            ljmp $0x08, $do_switch" \
            :: \
            "m" (esp0), \
            "m" (ss), \
            "m" (current->regs.esp), \
            "m" (eflags), \
            "m" (cs), \
            "m" (current->regs.eip), \
            "m" (current));
}
