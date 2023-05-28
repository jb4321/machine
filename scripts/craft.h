#ifndef CRAFT
#define CRAFT

#include <iostream>
#include <vector>
#include <cstring>
#include <map>

#include "color_handling.h"
#include "inventory.h"
#include "custom_ui.h"
#include "player.h"

class Crafting
{
    public:
        std::vector<Item> ingrd;
        int max_ener_add;
        int ener_use;
        std::vector<Item> crafted;
    Crafting(std::vector<Item> ingrd_, int ener_use_, std::vector<Item> crafted_ = {}, int max_ener_add_ = 0 )
    {
        ingrd = ingrd_;
        ener_use = ener_use_;
        crafted = crafted_;
        max_ener_add = max_ener_add_;
        
    }
    bool CanCraft(Player *player)
    {
        if (player->ener - ener_use <= 0)
        {
            return false;
        }
        for (int i = 0; i < ingrd.size(); i++)
        {
            Item ing_item = ingrd[i];
            if(!player->inv.canRemove(ing_item))
            {
                return false;
            }
        }
        return true;
    }
    void CraftIt(Player *player)
    {
        if(CanCraft(player))
        {
            for (int i = 0; i < ingrd.size(); i++)
            {
                Item ing_item = ingrd[i];
                int w = player->inv.getIndex(ing_item);
                player->inv.removeItem(ing_item);
            }
            player->ChangeEner(ener_use);
            for (int i = 0; i < crafted.size(); i++)
            {
                player->inv.AddItem(crafted[i]);
            }
            
        }
    }
};
std::map<std::string,Crafting> crafting_info = {
    {"Grass Burn",Crafting({Item("grass",7)},-10)},
    {"Melt Iron",Crafting({Item("iron",3)},10,{Item("iron_bar",1)})}
};

class UICraft : public UIMenu
{
    public:
    UICraft(int size_y, int size_x,int pos_y = 0 , int pos_x= 0) : UIMenu(size_y,size_x,pos_y,pos_x)
    {
        
    }
    void craft_select(Player *player)
    {
        int i = 0;
        for (auto &x : crafting_info)
        {
            if(i == selected_option)
            {
                x.second.CraftIt(player);
                return;
            }
            i++;
        }
    }
    void option_craft(Player *player)
    {
        options = {};
        for (auto &x : crafting_info)
        {
            std::string pushed;
            if (x.second.CanCraft(player))
            {
                pushed.append("Y ");
            }
            else
            {
                pushed.append("N ");
            }
            pushed.append(x.first);
            options.push_back(pushed);
        }
    }
    void render_ui()
    {
        wclear(window);
        show();
        box(window, 0, 0);

        int fg_color = COLOR_WHITE;
        
        for (int i = 0; i < options.size(); i++)
        {
            std::string i_option = options[i]; 
            if(i_option[0] == 'Y')
            {
                fg_color = COLOR_GREEN;
            }
            else
            {
                fg_color = COLOR_RED;
            }
            
            if (i==selected_option)
            {
                wattron(window,A_STANDOUT);
            }
            if(keep_centered)
            {
                print_centered_text(window,2+i, i_option.c_str(),fg_color,COLOR_BLACK);
            }
            else
            {
                color_print(2+i,2,i_option.c_str(),fg_color,COLOR_BLACK,window);
            }   
            
            if (i==selected_option)
            {
                wattroff(window,A_STANDOUT);
            }
        }
        update_panels();
        wnoutrefresh(window);
    }
};

#endif