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
        const char *sign = "@";
        Inventory inv;
    Player(int y_ = 0, int x_ = 0)
    {
        y = y_;
        x = x_;
        inv.AddItem(Item("Metal"));
        inv.AddItem(Item("Copper Wires"));
    }
    void Move(int dy,int dx)
    {
        y+=dy;
        x+=dx;
    }
};

#endif