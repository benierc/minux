#include "types.h"
#include "gdt.h"
#include "screen.h"

void do_syscalls(int sys_num)
{
    u16 ds_select;
    u32 ds_base;
    struct gdtdesc *ds;
    uchar *message;

    if (sys_num == 1) {
        asm("   mov 44(%%ebp), %%eax    \n \
                mov %%eax, %0           \n \
                mov 24(%%ebp), %%ax     \n \
                mov %%ax, %1" : "=m"(message), "=m" (ds_select) : );

        ds = (struct gdtdesc *)(GDTBASE + (ds_select & 0xF8)); /* mask 0xF8 because of RPL */
        ds_base = ds->base15_0 + (ds->base23_16 << 16) + (ds->base31_24 << 24);

        print((char *)(ds_base + message));
    }
    else
        print("syscalls\n");
    return;
}
