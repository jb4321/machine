#include <iostream>
#include <vector>
#include <string>

#include <pdcurses.h>

//zmienne
int x = 0;
int y = 0;

int main()
{
    //start
    initscr();
    printw( "Press Button..\n" );
    keypad(stdscr, TRUE);	
    noecho();
    do
    {  
        int key = getch();
        switch (key)
        {
            case KEY_UP:
                y--;
                break;
            case KEY_DOWN:
                y++;
                break;
            case KEY_LEFT:
                x--;
                break;
            case KEY_RIGHT:
                x++;
                break;
            default:
                break;
        }
        if (key == KEY_F(1))
        {
            break;
        }
        move(y,x);
    } while (true);
    
    printw( "End" );
    getch();
    endwin();
   
}