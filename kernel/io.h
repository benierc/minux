#ifndef __IO__
#define __IO__

/* deactivate interruptions */
#define cli asm("cli"::)

/* activate interruptions */
#define sti asm("sti"::)

/* write an byte on a port */
#define outb(port,value) \
	asm volatile ("outb %%al, %%dx" :: "d" (port), "a" (value));

/* write an byte on a port and temporise*/
#define outbp(port,value) \
	asm volatile ("outb %%al, %%dx; jmp 1f; 1:" :: "d" (port), "a" (value));

/* read a byte on a port */
#define inb(port) ({    \
	unsigned char _v;       \
	asm volatile ("inb %%dx, %%al" : "=a" (_v) : "d" (port)); \
        _v;     \
})

#endif
