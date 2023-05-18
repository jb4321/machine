#include <iostream>
#include <vector>
#include <string>

#include <pdcurses.h>
//zmienne
int x = 20;
int y = 10;
int term_x = 60;
int term_y = 30;

bool IsNotOut(int y_, int x_)
{
    if(x_ >= term_x || x_ < 0)
    {
        return false;
    }
    if(y_ >= term_y || y_ < 0)
    {
        return false;
    }
    return true;
}
int main()
{
    //start
    std::cout << "\x1b]20;" << "10x20" << "\a" << std::flush;
    std::cout << "test";
    initscr();
    raw();
    resize_term(term_y,term_x);
    if(has_colors() == FALSE)
	{	endwin();
		printf("Your terminal does not support color\n");
		exit(1);
	}
    start_color();
    init_pair(1,COLOR_RED,COLOR_BLACK);

    printw( "Press Button..\n" );
    keypad(stdscr, TRUE);	
    noecho();
    move(y,x);
    do
    {
        int ny=y;
        int nx=x; 
        int key = getch();
        switch (key)
        {
            case KEY_UP:
                ny--;
                break;
            case KEY_DOWN:
                ny++;
                break;
            case KEY_LEFT:
                nx--;
                break;
            case KEY_RIGHT:
                nx++;
                break;
            default:
                break;
        }
        if (key == KEY_F(1))
        {
            break;
        }
        
        
        if(IsNotOut(ny,nx))
        {
            x = nx;
            y = ny;
        }
        mvprintw(y,x,"@");
        
        std::string txt = std::to_string(x) + "," + std::to_string(y) + " " ;
        attron(A_BOLD);
        attron(COLOR_PAIR(1));
        mvprintw(0,0,txt.c_str());
        attroff(A_BOLD);
        attroff(COLOR_PAIR(1));
        
        move(y,x);
    } while (true);
    
    printw( "End" );
    getch();
    endwin();
   
}