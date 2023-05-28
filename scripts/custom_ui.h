#ifndef CUS_UI
#define CUS_UI

#include <vector>

#include "color_handling.h"
#include "pdcurses.h"
#include "panel.h"
#include "inventory.h"

void print_centered_text(WINDOW *window_p, int y, const char *text , int fg, int bg)
{
    color_print(y,(window_p->_maxx-strlen(text))/2,text,fg,bg,window_p);
}
class BaseUI
{
    public:
        WINDOW *window;
        PANEL *panel;
    BaseUI(int size_y, int size_x,int pos_y = 0 , int pos_x= 0)
    {
        window = newwin(size_y, size_x, pos_y, pos_x);
        panel = new_panel(window);
        hide();
    }
    void hide()
    {
        hide_panel(panel);
        wnoutrefresh(window);
    }
    void show()
    {
        show_panel(panel);
        wnoutrefresh(window);
    }
};
class UIMenu : public BaseUI
{
    public:
        int selected_option = 0;
        std::vector<std::string> options;
        bool free_scrool = false;
        bool is_focused = false;
        bool keep_centered = true;

    UIMenu(int size_y, int size_x,int pos_y = 0 , int pos_x= 0) : BaseUI(size_y,size_x,pos_y,pos_x)
    {
        
    }
    void center_menu()
    {  
        window->_begx = (COLS - window->_maxx)/2;
        window->_begy = (LINES - window->_maxy)/2;
    }
    void scroll_option_list(int direction)
    {
        selected_option += direction;
        if( selected_option < 0)
        {
            if(free_scrool) selected_option = options.size() -1;
            else selected_option = 0;
        }
        if (selected_option >= options.size())
        {
            if(free_scrool) selected_option = 0;
            else selected_option = options.size() -1;
        }
        
    }    
    
    virtual void render_ui()
    {
        wclear(window);
        show();
        box(window, 0, 0);
        for (int i = 0; i < options.size(); i++)
        {
            if (i==selected_option)
            {
                wattron(window,A_STANDOUT);
            }
            if(keep_centered)
            {
                print_centered_text(window,2+i, options[i].c_str(),COLOR_WHITE,COLOR_BLACK);
            }
            else
            {
                color_print(2+i,2,options[i].c_str(),COLOR_WHITE,COLOR_BLACK,window);
            }   
            
            if (i==selected_option)
            {
                wattroff(window,A_STANDOUT);
            }
        }
        update_panels();
        wnoutrefresh(window);
    }
    void focus()
    {
        is_focused = true;
        render_ui();
    }
    void unfocus()
    {
        is_focused = false;
    }
};
class InventoryMenu : public UIMenu
{
    public:
    InventoryMenu(int size_y, int size_x,int pos_y = 0 , int pos_x= 0) : UIMenu(size_y,size_x,pos_y,pos_x)
    {
        
    }
    void option_inventory(Inventory inv)
    {
        options = {};
        for (int i = 0; i < inv.inv.size(); i++)
        {
            options.push_back(inv.inv[i].name);
            if (inv.inv[i].count > 1)
            {
                options[i] += " " + std::to_string(inv.inv[i].count);  
            }
        }
    }
};

class UIBar : public BaseUI
{
    public:
        int bg;
        int fg;
    UIBar(int size_y, int size_x,int pos_y, int pos_x): BaseUI(size_y,size_x,pos_y,pos_x)
    {
        
    }
    void render_ui(int value, int max_value)
    {
        wclear(window);
        show();
        color_print(0,0,"▒",COLOR_WHITE,COLOR_BLACK,window);
        color_print(0,window->_maxx-1,"▒",COLOR_WHITE,COLOR_BLACK,window);
        for (int i = 0; i < window->_maxx; i++)
        {
            color_print(1,i,"▒",COLOR_WHITE,COLOR_BLACK,window);
        }
        std::string txt = std::to_string(value) + "/" + std::to_string(max_value);
        color_print(1,1,txt.c_str(),COLOR_CYAN,COLOR_BLACK,window);
        float amount = ((float)value/(float)max_value) * (window->_maxx-2);
        int flr_amount = std::floor(amount);
        for (int i = 0;i < flr_amount;i++)
        {
            color_print(0,1+i," ",COLOR_BLACK,COLOR_CYAN,window);
        }
        //░▒▓
        if (amount - flr_amount > 0.8)
        {
            color_print(0,flr_amount +1 ,"░",COLOR_BLACK,COLOR_CYAN,window);
        }
        else if(amount - flr_amount > 0.6)
        {
            color_print(0,flr_amount +1 ,"▒",COLOR_BLACK,COLOR_CYAN,window);
        }
        else if(amount - flr_amount > 0.4)
        {
            color_print(0,flr_amount +1 ,"▒",COLOR_CYAN,COLOR_BLACK,window);
        }
        else if(amount - flr_amount > 0.2)
        {
            color_print(0,flr_amount +1 ,"░",COLOR_CYAN,COLOR_BLACK,window);
        }
        update_panels();
        wnoutrefresh(window);
    }

};

#endif