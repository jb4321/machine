#ifndef COLOR_HAND_H
#define COLOR_HAND_H
#include <pdcurses.h>

short curs_color(int fg)
{
    switch (7 & fg) {           /* RGB */
    case 0:                     /* 000 */
        return (COLOR_BLACK);
    case 1:                     /* 001 */
        return (COLOR_BLUE);
    case 2:                     /* 010 */
        return (COLOR_GREEN);
    case 3:                     /* 011 */
        return (COLOR_CYAN);
    case 4:                     /* 100 */
        return (COLOR_RED);
    case 5:                     /* 101 */
        return (COLOR_MAGENTA);
    case 6:                     /* 110 */
        return (COLOR_YELLOW);
    case 7:                     /* 111 */
        return (COLOR_WHITE);
    }
    return COLOR_BLACK;
}

int colornum(int fg, int bg)
{
    int B, bbb, ffff;

    B = 1 << 7;
    bbb = (7 & bg) << 4;
    ffff = 7 & fg;

    return (B | bbb | ffff);
}

void init_colorpairs(void)
{
    int fg, bg;
    int colorpair;

    for (bg = 0; bg <= 7; bg++) {
        for (fg = 0; fg <= 7; fg++) {
            colorpair = colornum(fg, bg);
            init_pair(colorpair, curs_color(fg), curs_color(bg));
        }
    }
}
int is_bold(int fg)
{
    /* return the intensity bit */

    int i;

    i = 1 << 3;
    return (i & fg);
}

void setcolor(int fg, int bg, WINDOW *window_p=stdscr)
{
    /* set the color pair (colornum) and bold/bright (A_BOLD) */

    wattron(window_p,COLOR_PAIR(colornum(fg, bg)));
    /*if (is_bold(fg)) {
        attron(A_BOLD);
    }*/
}

void unsetcolor(int fg, int bg, WINDOW *window_p=stdscr)
{
    /* unset the color pair (colornum) and
       bold/bright (A_BOLD) */

    wattroff(window_p,COLOR_PAIR(colornum(fg, bg)));
    /*if (is_bold(fg)) {
        attroff(A_BOLD);
    }*/
}
void color_print(int y_, int x_, const char* text, int fg, int bg,WINDOW *window_print = stdscr)
{
    setcolor(fg,bg,window_print);
    mvwprintw(window_print,y_,x_,text);
    unsetcolor(fg,bg,window_print);
}
#endif