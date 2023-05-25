#ifndef INVE
#define INVE

class ItemInfo
{
    public:
        int max_count;
};
class Item
{
    public:
        std::string name;
        int count;
    Item(std::string name_, int count_ = 1)
    {
        name = name_;
        count = count_;
    }
};

class Inventory
{
    public:
        std::vector<Item> inv;
        int imax_size;
    Inventory()
    {
        
    }
    void removeItem(Item ad_item)
    {

    }
    void AddItem(Item ad_item)
    {
        int w = -1;
        for (int i = 0; i < inv.size(); i++)
        {
            if(inv[i].name == ad_item.name)
            {
                w = i;
                break;
            }
        }
        if(w ==-1)
        {
            inv.push_back(ad_item);
        }
    }
};
#endif