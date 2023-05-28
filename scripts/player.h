#ifndef PLAYR
#define PLAYR

#include <iostream>
#include <vector>
#include <cstring>
#include <map>

#include "color_handling.h"
#include "inventory.h"

class Player
{
    public:
        int y;
        int x;
        int ener;
        int ener_max = 150;
        int ener_drain = 1;
        const char *sign = "@";
        Inventory inv;
        int tool_tier;
    

    Player(int y_ = 0, int x_ = 0)
    {
        ener = ener_max;

        y = y_;
        x = x_;

    }
    void UpgradeTool(){
        tool_tier += 1;
    }
    void Mine()
    {

    }
    void ChangeEner(int ener_value)
    {
        int n_ener = ener - ener_value;
        ener = n_ener;
    }
    void Move(int dy,int dx,WorldMap *mapa,bool mining_mode = false)
    {
        int ny = y+dy;
        int nx = x+dx;
        
        Cell kom = mapa->GetCell(ny,nx);
        if(mining_mode && kom.object_cell.object_name != "none")
        {
            if(kom.object_cell.GetInfo().tool_tier <= tool_tier)
            {
                ener -= kom.get_info().tool_tier *2;
                mapa->SetCellObject(ny,nx,GenericObject());
                inv.AddItem(Item(kom.object_cell.object_name,1));
                
            }   
        }

        
        if (kom.get_info().is_pass)
        {
            y = ny;
            x = nx;
            ener -= ener_drain;
        }
    }
};

#endif