#include "types.h"
#include "lib.h"
#include "gdt.h"


/*
 * 'init_desc' initialise un descripteur de segment situe en gdt ou en ldt.
 * 'desc' est l'adresse lineaire du descripteur a initialiser.
 */
void init_gdt_desc(u32 base, u32 limit, u8 access, u8 infos, struct gdtdesc *desc)
{
	desc->lim15_0 = (limit & 0xffff);
	desc->base15_0 = (base & 0xffff);
	desc->base23_16 = (base & 0xff0000) >> 16;
	desc->access = access;
	desc->lim19_16 = (limit & 0xf0000) >> 16;
	desc->infos = (infos & 0xf);
	desc->base31_24 = (base & 0xff000000) >> 24;
	return;
}

/*
 * Cette fonction initialise la GDT apres que le kernel soit charge
 * en memoire. Une GDT est deja operationnelle, mais c'est celle qui
 * a ete initialisee par le secteur de boot et qui ne correspond
 * pas forcement a celle que l'on souhaite.
 */
void init_gdt(void)
{
    default_tss.debug_flag = 0x00;
    default_tss.io_map = 0x00;
    default_tss.esp0 = 0x20000;
    default_tss.ss0 = 0x18;

	/* initialisation des descripteurs de segment */
	init_gdt_desc(0x0, 0x0, 0x0, 0x0, &kgdt[0]);
	init_gdt_desc(0x0, 0xFFFFF, 0x9B, 0x0D, &kgdt[1]);	/* code */
	init_gdt_desc(0x0, 0xFFFFF, 0x93, 0x0D, &kgdt[2]);	/* data */
	init_gdt_desc(0x0, 0x0, 0x97, 0x0D, &kgdt[3]);		/* stack */

    init_gdt_desc(0x30000, 0x1, 0xFF, 0x0D, &kgdt[4]);	/* ucode */
    init_gdt_desc(0x30000, 0x1, 0xF3, 0x0D, &kgdt[5]);	/* udata */
    init_gdt_desc(0x0, 0x0, 0xF7, 0x0D, &kgdt[6]);		/* ustack */

    init_gdt_desc((u32) & default_tss, 0x67, 0xE9, 0x00, &kgdt[7]);	/* descripteur de tss */


	/* initialisation de la structure pour GDTR */
	kgdtr.limit = GDTSIZE * 8;
	kgdtr.base = GDTBASE;

	/* recopie de la GDT a son adresse */
	memcpy((char *) kgdtr.base, (char *) kgdt, kgdtr.limit);

	/* chargement du registre GDTR */
	asm("lgdtl (kgdtr)");

	/* initialisation des segments */
    asm("   movw $0x10, %ax \n \
            movw %ax, %ds   \n \
            movw %ax, %es   \n \
            movw %ax, %fs   \n \
            movw %ax, %gs   \n \
            ljmp $0x08, $next   \n \
            next:       \n");

}

