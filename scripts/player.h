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
        int ener_max = 200;
        int ener_drain = 1;
        const char *sign = "@";
        Inventory inv;
    Player(int y_ = 0, int x_ = 0)
    {
        ener = ener_max;

        y = y_;
        x = x_;
        inv.AddItem(Item("Metal"));
        inv.AddItem(Item("Copper Wires",3));
        inv.AddItem(Tool("Drill","drill",0));

    }
    void Move(int dy,int dx,WorldMap *mapa,bool mining_mode = false)
    {
        int ny = y+dy;
        int nx = x+dx;
        
        if(mining_mode)
        {
            mapa->SetCellObject(ny,nx,GenericObject());
        }

        Cell kom = mapa->GetCell(ny,nx);
        if (kom.get_info().is_pass)
        {
            y = ny;
            x = nx;
            ener -= ener_drain;
        }
    }
};

#endif