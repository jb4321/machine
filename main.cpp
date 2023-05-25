#include <iostream>
#include <vector>
#include <cstring>
#include <map>
#include <cmath>
#include <functional>

#include <pdcurses.h>
#include <panel.h>

#include "scripts/FastNoise.h"

#include "scripts/map.h"
#include "scripts/color_handling.h"
#include "scripts/player.h"
#include "scripts/custom_ui.h"
#ifdef _WIN32
    #include <windows.h>

    void sleep(unsigned milliseconds)
    {
        Sleep(milliseconds);
    }
#else
    #include <unistd.h>
    
    void sleep(unsigned milliseconds)
    {
        usleep(milliseconds * 1000); // takes microseconds
    }
#endif


Player player_char;
int term_x = 60;
int term_y = 30;
WorldMap world_map;
int game_state = 0;
int sel_option = 0;

bool is_paused = false;

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


void End()
{
    printw( "End" );
    getch();
    endwin();
    exit(0);
}
void MainMenu()
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

    keypad(stdscr, TRUE);	
    noecho();
    curs_set(0);
    move(player_char.y,player_char.x);
}
void RenderGame()   
{
    for(int i = 0; i < LINES; i++)
    {
        for(int j = 0; j < COLS; j++)
        {
            int w_y = i + player_char.y;
            int w_x = j + player_char.x;
            Cell w_cell = world_map.GetCell(w_y,w_x);
            InfoGenericObject w_info = w_cell.get_info();
            color_print(i, j ,w_info.sign,w_info.fg,w_info.bg);
        }    
    }
}

void GameLoop()
{
    UIMenu ui_menu = UIMenu(12,14);
    ui_menu.options = {"START GAME","OPTIONS","QUIT"};
    ui_menu.center_menu();

    UIMenu pause_menu = UIMenu(12,14);
    pause_menu.options = {"CONTINUE","OPTIONS","MAIN MENU","QUIT"};
    pause_menu.center_menu();

    InventoryMenu inv_menu = InventoryMenu(20,14);
    inv_menu.center_menu();

    ui_menu.render_menu();
    doupdate();
    do
    {   
        int ny=0;
        int nx=0; 
        int key = getch();
        std::cout << key;
        sleep(15);
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
        switch(game_state)
        {
            case 0:
            {
                ui_menu.scroll_option_list(ny);
                ui_menu.render_menu();
                if (key == 13)
                {
                    
                    switch (ui_menu.selected_option)
                    {
                        case 0:
                            game_state = 2;
                            break;
                        case 1:
                            break;
                        case 2:
                            End();
                        default:
                            break;
                    }
                }
                break;
            }
            case 2:
                if(inv_menu.is_focused)
                {
                    inv_menu.scroll_option_list(ny);
                    inv_menu.option_inventory(player_char.inv);
                    inv_menu.render_menu();
                    if(key == 13)
                    {
                        inv_menu.unfocus();
                        inv_menu.hide();
                    }
                }
                else if(pause_menu.is_focused)
                {
                    pause_menu.scroll_option_list(ny);
                    pause_menu.render_menu();
                    if (key == 13) //ENTER character
                    {
                        switch (pause_menu.selected_option)
                        {
                            case 0:
                                pause_menu.hide();
                                break;
                            case 1:
                                break;
                            case 2:
                                game_state = 0;
                                pause_menu.hide();
                                wclear(stdscr);
                                ui_menu.render_menu();
                                break;
                            case 3:
                                End();
                            default:
                                break;
                        }
                        pause_menu.unfocus();
                        pause_menu.hide();
                    }
                }
                else if(key ==27) //ESC character
                {
                    
                    pause_menu.focus();
                }
                else if(key == 105) //ESC character
                {
                    inv_menu.option_inventory(player_char.inv);
                    inv_menu.focus();
                }
                else 
                {
                    player_char.x += nx;
                    player_char.y += ny;
                    /*if(IsNotOut(ny,nx))
                    {
                        x = nx;
                        y = ny;
                    }*/
                    RenderGame();
                    mvprintw(term_y/2,term_x/2,player_char.sign);
                
                    std::string txt = std::to_string(player_char.x) + "," + std::to_string(player_char.y);
                    color_print(0,0,txt.c_str(),COLOR_RED,COLOR_WHITE);

                    move(term_y/2,term_x/2);
                    
                    break;
                }
                
        }
        
        refresh();
	    doupdate();

    } while (true);
}


int main(int argc, char **argv)
{
    Start();

    GameLoop();
    End();
    return 0;
}
