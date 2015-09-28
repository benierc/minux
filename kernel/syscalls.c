#include "types.h"
#include "lib.h"

void do_syscalls(int sys_num)
{
    char *u_str;

    if (sys_num == 1) {
        asm("mov %%ebx, %0": "=m"(u_str) :);
        printk(u_str);
    } else {
        printk("unknown syscall %d\n", sys_num);
    }

    return;
}
