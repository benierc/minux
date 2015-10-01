#ifndef __SCREEN__
#define __SCREEN__

#include "types.h"

#define RAMSCREEN 0xB8000   /* debut de la memoire video */
#define LINES 25
#define COLUMS 80
#define CASE 2 /* octets */
#define SIZESCREEN LINES*COLUMS*CASE /* 25 lines * 80 columns */
#define SCREENLIM (RAMSCREEN + SIZESCREEN)
#define SIZE_LINE CASE*COLUMS


void scrollup(unsigned int);
void putcar(uchar);
void print(char*);
void show_cursor(void);
void hide_cursor(void);
void dump(uchar *addr, int n);

#endif
