#include <iostream>
#include <vector>
#include <cstring>
#include <map>
#include <cmath>
#include <functional>

#include <pdcurses.h>

#include "scripts/map.h"
#include "scripts/color_handling.h"
int x = 0;
int y = 0;
int term_x = 60;
int term_y = 30;
WorldMap world_map;

void RenderGame()
{
    for(int i = 0; i < LINES; i++)
    {
        for(int j = 0; j < COLS; j++)
        {
            int w_y = i + y;
            int w_x = j + x;
            Cell w_cell = world_map.GetCell(w_y,w_x);
            CellObjectInfo w_info = w_cell.get_info();
            color_print(i, j ,w_info.sign,w_info.fg,w_info.bg);
        }    
    }
}
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
void myclear()
{

}
void Start()
{
    
    #ifdef XCURSES
        Xinitscr(argc, argv);
    #else
        initscr();
    #endif
    def_prog_mode();
    raw();
    resize_term(term_y, term_x);
    refresh();
    if(has_colors() == FALSE)
	{	endwin();
		printf("Your terminal does not support color\n");
		exit(1);
	}
    
    start_color();
    init_colorpairs(); 

    printw( "Press Button..\n" );
    keypad(stdscr, TRUE);	
    noecho();

    
    color_print(0,20,"a",1,2);
    color_print(0,30,"a",2,4);
    move(y,x);
}
void GameLoop()
{
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
            case KEY_F(2):
                break;
            default:
                break;
        }
        
        if (key == KEY_F(1))
        {
            break;
        }
        wclear(stdscr);
        
        x = nx;
        y = ny;
        /*if(IsNotOut(ny,nx))
        {
            x = nx;
            y = ny;
        }*/
        RenderGame();
        mvprintw(term_y/2,term_x/2,"@");
        
        std::string txt = std::to_string(x) + "," + std::to_string(y);
        color_print(0,0,txt.c_str(),COLOR_RED,COLOR_WHITE);

        move(term_y/2,term_x/2);
        refresh();
    } while (true);
}

void End()
{
    printw( "End" );
    getch();
    endwin();
}
int main(int argc, char **argv)
{
    
    Start();
    GameLoop();
    End();
    return 0;
}
