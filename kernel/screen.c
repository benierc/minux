#include "types.h"
#include "screen.h"
#include "io.h"

char cursorX = 0;            /* position courante du curseur a l'ecran */
char cursorY = 8;
char cursorAttr = 0x07;      /* attributs video des caracteres a afficher */

/* ASCII */
#define HT 9 /*Horizontal Tab */
#define LF 10 /* Line Feed */
#define CR 13 /* Carriage Return */

void scrollup(unsigned int n)
{
    unsigned char *video, *tmp;
    for ( video = (unsigned char *)RAMSCREEN;
            video < (unsigned char *)SCREENLIM;
            video += CASE )
    {
        tmp = (unsigned char *)(video + n * SIZE_LINE);
        if (tmp < (unsigned char *)SCREENLIM)
        {
            *video = *tmp;
            *(video + 1) = *(tmp + 1);
        }
        else
        {
            *video = 0;
            *(video + 1) = 0x7;
        }
    }
    cursorY -= n;
    if (cursorY < 0)
        cursorY = 0;
}

void putcar(uchar c)
{
    unsigned char *video;

    if (c == HT)
        cursorX = cursorX + 8 - (cursorX % 8);
    else if (c == LF) {
        cursorY++;
        cursorX = 0;
    } else if (c == CR) {
        cursorX = 0;
    } else {
        video = (unsigned char *)(RAMSCREEN + 2 * cursorX + SIZE_LINE * cursorY);
        *video = c;
        *(video + 1) = cursorAttr;

        cursorX++;
        if (cursorX >= COLUMS)
        {
            cursorX = 0;
            cursorY++;
        }
    }

    if (cursorY >= LINES)
        scrollup(cursorY + 1 - LINES);
}

void print (char *string)
{
    while (*string != '\0')
    {
        putcar(*string);
        string++;
    }
}

void dump(uchar* addr, int n)
{
    char c1, c2;
    char *tab = "0123456789ABCDEF";

    while(n--) {
        c1 = tab[(*addr & 0xF0)>>4];
        c2 = tab[*addr & 0x0F];
        addr++;
        putcar(c1);
        putcar(c2);
    }
}

void move_cursor(u8 x, u8 y)
{
    u16 c_pos;

    c_pos = y * 80 + x;

    outb(0x3d4, 0x0f);
    outb(0x3d5, (u8) c_pos);
    outb(0x3d4, 0x0e);
    outb(0x3d5, (u8) (c_pos >> 8));
}

void show_cursor(void)
{
    move_cursor(cursorX, cursorY);
}

void hide_cursor(void)
{
    move_cursor(-1, -1);
}
