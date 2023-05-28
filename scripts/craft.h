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
        int tier;
    Crafting(std::vector<Item> ingrd_, int ener_use_, std::vector<Item> crafted_ = {}, int max_ener_add_ = 0 )
    {
        ingrd = ingrd_;
        ener_use = ener_use_;
        crafted = crafted_;
        max_ener_add = max_ener_add_;
        tier = -1;
        
    }
    Crafting(std::vector<Item> ingrd_, int ener_use_, int tier_)
    {
        ingrd = ingrd_;
        ener_use = ener_use_;
        tier = tier_;
        crafted = {};
        max_ener_add = 0;
        
    }
    bool CanCraft(Player *player)
    {
        if (player->ener - ener_use <= 0)
        {
            return false;
        }
        if (player->ener - ener_use > player->ener_max)
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
            if(tier != -1 && player->tool_tier!=tier)
            {
                return;
            }
            for (int i = 0; i < ingrd.size(); i++)
            {
                Item ing_item = ingrd[i];
                int w = player->inv.getIndex(ing_item);
                player->inv.removeItem(ing_item);
            }
            player->ChangeEner(ener_use);
            if(player->tool_tier == tier)
            {
                player->UpgradeTool();
            }
            for (int i = 0; i < crafted.size(); i++)
            {
                player->inv.AddItem(crafted[i]);
            }
            
        }
    }
};
std::map<std::string,Crafting> crafting_info = {
    {"! Grass Burn",Crafting({Item("Grass",7)},-10)},
    {"! Tree Burn",Crafting({Item("Tree",1)},-10)},
    {"! Coal Burn",Crafting({Item("Coal",1)},-15)},
    {"! Oil Burn",Crafting({Item("Oil",1)},-25)},

    {"Melt Tin",Crafting({Item("Tin",3)},5,{Item("Tin Ingot",1)})},
    {"Forge Steel",Crafting({Item("Iron",2),Item("Coal",1)},10,{Item("Steel Ingot",1)})},
    {"Copper Wires",Crafting({Item("Copper",2)},10,{Item("Copper Wires",1)})},
    {"Melt Titan",Crafting({Item("Titan",3)},10,{Item("Titan Ingot",1)})},

    {"Tin Drill",Crafting({Item("Tin Ingot",6)},20,0)},
    {"Steel Drill",Crafting({Item("Steel Ingot",5),Item("Copper Wires",5)},30,1)},

    {"Rocket",Crafting({Item("Titan Ingot",10),Item("Steel Ingot",10),Item("Copper Wires",20),Item("Oil",10),Item("Tin Ingot",10)},50,{Item("Rocket",1)})}


};

class UICraft : public UIMenu
{
    public:
    UICraft(int size_y=1, int size_x=1,int pos_y = 0 , int pos_x= 0) : UIMenu(size_y,size_x,pos_y,pos_x)
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
            pushed.append(" "+ std::to_string(x.second.ener_use*-1));
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