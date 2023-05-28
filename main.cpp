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
#include "scripts/craft.h"

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
    wnoutrefresh(stdscr);
    
}

void GameLoop()
{
    UIBar ener_bar = UIBar(2,17,0,10);

    
    UIMenu ui_menu = UIMenu(12,14);
    ui_menu.options = {"START GAME","QUIT"};
    ui_menu.center_menu();

    UIMenu pause_menu = UIMenu(12,14);
    pause_menu.options = {"CONTINUE","MAIN MENU","QUIT"};
    pause_menu.center_menu();

    InventoryMenu inv_menu = InventoryMenu(24,20);
    inv_menu.center_menu();
    inv_menu.free_scrool = true;
    inv_menu.keep_centered = false;

    UICraft craft_menu = UICraft(20,30);
    craft_menu.center_menu();
    craft_menu.free_scrool = true;

    bool mining_mode = true;
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
                ui_menu.render_ui();
                if (key == 10)
                {
                    switch (ui_menu.selected_option)
                    {
                        case 0:
                            game_state = 2;
                            ui_menu.hide();
                            RenderGame();
                            break;
                        case 1:
                            End();
                        default:
                            break;
                    }
                }
                break;
            }
            case 2:
            {
                if(inv_menu.is_focused)
                {
                    inv_menu.scroll_option_list(ny);
                    inv_menu.option_inventory(player_char.inv);
                    inv_menu.render_ui();
                    
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
                        inv_menu.render_ui();
                        goto game_focus;
                    }
                    if(key =='e'|| key==27)
                    {
                        inv_menu.unfocus();
                        inv_menu.hide();
                        goto game_focus;
                    }
                }
                else if(pause_menu.is_focused)
                {
                    
                    pause_menu.scroll_option_list(ny);
                    pause_menu.render_ui();
                    if (key == 10) //ENTER character
                    {
                        switch (pause_menu.selected_option)
                        {
                            case 0:
                                pause_menu.hide();
                                pause_menu.unfocus();
                                goto game_focus;
                                break;
                            case 1:
                                game_state = 0;
                                pause_menu.hide();
                                wclear(stdscr);
                                player_char = Player();
                                world_map = WorldMap();
                                ui_menu.render_ui();
                                break;
                            case 2:
                                End();
                            default:
                                break;
                        }
                        pause_menu.unfocus();
                        pause_menu.hide();
                    }
                }
                else if(craft_menu.is_focused)
                {
                    craft_menu.scroll_option_list(ny);
                    craft_menu.option_craft(&player_char);
                    craft_menu.render_ui();
                    
                    if(key == 10)
                    {
                        craft_menu.craft_select(&player_char);
                        craft_menu.option_craft(&player_char);
                        ener_bar.render_ui(player_char.ener,player_char.ener_max);
                        craft_menu.render_ui();
                        if (player_char.inv.getIndex(Item("Rocket")) != -1 )
                        {
                            std::string txt = "CONGRATULATIONS!";
                            print_centered_text(stdscr,LINES/2,txt.c_str(),COLOR_YELLOW,COLOR_BLACK);
                            txt = "YOU COMPLETED GAME";
                            print_centered_text(stdscr,LINES/2+1,txt.c_str(),COLOR_YELLOW,COLOR_BLACK);
                            txt = "THANKS FOR PLAYING";
                            print_centered_text(stdscr,LINES/2+2,txt.c_str(),COLOR_YELLOW,COLOR_BLACK);
                            getch();
                            ener_bar.hide();            
                            game_state = 0;
                            pause_menu.hide();
                            wclear(stdscr);
                            world_map = WorldMap();
                            player_char = Player();
                            ui_menu.render_ui();
                        }
                    }
                    if(key == 'c'|| key==27)
                    {
                        craft_menu.unfocus();
                        craft_menu.hide();
                        goto game_focus;
                    }
                }
                else if(key ==27) //ESC character
                {
                    pause_menu.focus();
                }
                else if(key == 'e')
                {
                    inv_menu.option_inventory(player_char.inv);
                    inv_menu.focus();
                }
                else if(key == 'c')
                {
                    craft_menu.option_craft(&player_char);
                    craft_menu.focus();
                }
                else 
                {
                    game_focus:
                    if(ny !=0 || nx !=0)
                    {
                        player_char.Move(ny,nx,&world_map,mining_mode);
                    }
                    
                      

                    RenderGame();
                    mvprintw(term_y/2,term_x/2,player_char.sign);
                    
                    std::string txt = std::to_string(player_char.x) + "," + std::to_string(player_char.y);
                    color_print(0,0,txt.c_str(),COLOR_RED,COLOR_WHITE);
                    color_print(1,0,std::to_string(player_char.tool_tier).c_str(),COLOR_CYAN,COLOR_WHITE);

                    move(term_y/2,term_x/2);
                    
                    ener_bar.render_ui(player_char.ener,player_char.ener_max);
                    if (player_char.inv.getIndex(Item("Rocket")) != -1 )
                    {
                        txt = "CONGRATULATIONS!";
                        print_centered_text(stdscr,LINES/2,txt.c_str(),COLOR_YELLOW,COLOR_BLACK);
                        txt = "YOU COMPLETED GAME";
                        print_centered_text(stdscr,LINES/2+1,txt.c_str(),COLOR_YELLOW,COLOR_BLACK);
                        txt = "THANKS FOR PLAYING";
                        print_centered_text(stdscr,LINES/2+2,txt.c_str(),COLOR_YELLOW,COLOR_BLACK);
                        getch();
                        ener_bar.hide();            
                        game_state = 0;
                        pause_menu.hide();
                        wclear(stdscr);
                        world_map = WorldMap();
                        player_char = Player();
                        ui_menu.render_ui();
                    }
                    if(player_char.ener <= 0)
                    {
                        txt = "BATTERY RUN OUT";
                        print_centered_text(stdscr,LINES/2,txt.c_str(),COLOR_RED,COLOR_BLACK);
                        refresh();
	                    doupdate();
                        sleep(1000);
                        getch();
                        ener_bar.hide();            
                        game_state = 0;
                        pause_menu.hide();
                        wclear(stdscr);
                        world_map = WorldMap();
                        player_char = Player();
                        ui_menu.render_ui();
                    }
                }
                
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
