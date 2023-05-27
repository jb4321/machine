#include <iostream>
#include <vector>
#include <cstring>
#include <map>
#include <cmath>
#include <functional>
#include <typeinfo>

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

int game_state = 2;
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
    reset_prog_mode();
    cbreak();
    noecho();
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
    
    curs_set(0);
    move(player_char.y,player_char.x);
}
void RenderGame()   
{
    for(int i = 0; i < LINES; i++)
    {
        for(int j = 0; j < COLS; j++)
        {
            int w_y = i + player_char.y - LINES/2;
            int w_x = j + player_char.x - COLS/2;
            Cell w_cell = world_map.GetCell(w_y,w_x);
            
            RenderInfo w_info = w_cell.get_info().render;
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

    InventoryMenu inv_menu = InventoryMenu(24,20);
    inv_menu.center_menu();
    inv_menu.free_scrool = true;
    inv_menu.keep_centered = false;

    ui_menu.render_menu();

    bool mining_mode = false;
    doupdate();
    do
    {   
        int ny=0;
        int nx=0;
        
        int key = getch();
        //std::cout << key;
        sleep(15);
        switch (key)
        {
            case ' ':
                break;
            case 'f':
                mining_mode = !mining_mode;
                break;
            case KEY_UP:
                ny--;
                break;
            case ctl(KEY_UP):
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
                if (key == 10)
                {
                    switch (ui_menu.selected_option)
                    {
                        case 0:
                            game_state = 2;
                            RenderGame();
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
                    
                    if(key == 10)
                    {
                        //std::cout << inv_menu.selected_option;
                        Item item = player_char.inv.inv[inv_menu.selected_option];
                        /*if(typeid(item) == typeid(Tool))
                        {
                            Tool tool = (Tool)item;
                            std::cout << 
                        }*/
                        
                        /*if(player_char.inv.canRemove(inv_menu.selected_option))
                        {
                            int result = player_char.inv.removeItem(inv_menu.selected_option); 
                            if(result==2)
                            {
                                inv_menu.scroll_option_list(-1);
                            }
                        }*/
                        inv_menu.option_inventory(player_char.inv);
                        inv_menu.render_menu();
                    }
                    if(key == 105|| key==27)
                    {
                        inv_menu.unfocus();
                        inv_menu.hide();
                    }
                }
                else if(pause_menu.is_focused)
                {
                    
                    pause_menu.scroll_option_list(ny);
                    pause_menu.render_menu();
                    std::cout << key;
                    if (key == 10) //ENTER character
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
                else if(key == 105) // "i" character
                {
                    inv_menu.option_inventory(player_char.inv);
                    inv_menu.focus();
                }
                else 
                {
                    player_char.Move(ny,nx,&world_map,mining_mode);
                      

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
