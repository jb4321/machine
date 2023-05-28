#ifndef INVE
#define INVE

class ItemInfo
{
    public:
        int max_count;
        RenderInfo render;
    ItemInfo(RenderInfo render_, int max_count_)
    {
        max_count = max_count;
        render = render_;
    }
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
    int getIndex(Item ad_item)
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
        return w;
    }
    bool canRemove(int w, int count)
    {
        Item ad_item = inv[w];
        ad_item.count = count;
        return canRemove(ad_item);
    }
    bool canRemove(Item ad_item)
    {
        int w = getIndex(ad_item);
        if(w >= inv.size() )
        {
            return false;
        }
        if(w <= -1)
        {
            return false;
        }
        else
        {
            if(inv[w].count < ad_item.count)
            {
                return false;
            }
        }
        return true;
    }
    int removeItem(int w,int count)
    {
        Item ad_item = inv[w];
        ad_item.count = count;
        return removeItem(ad_item);
    }
    int removeItem(Item ad_item)
    {
        int w = getIndex(ad_item);
        if (w <= -1)
        {
            return 0;
        }
        if(inv[w].count < ad_item.count)
        {
            return 0;
        }
        if(inv[w].count == ad_item.count)
        {
            inv.erase(inv.begin() + w);
            return 2;
        }
        if (inv[w].count > ad_item.count)
        {
            inv[w].count -= ad_item.count;
            return 1;
        }
        return 0;
    }
    void AddItem(Item ad_item)
    {
        int w = getIndex(ad_item);
        if(w ==-1)
        {
            inv.push_back(ad_item);
        }
        else
        {
            inv[w].count += ad_item.count;
        }
    }
};
#endif