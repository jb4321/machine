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
class UIMenu
{
    public:
        WINDOW *window;
        PANEL *panel;
        int selected_option = 0;
        std::vector<std::string> options;
        bool free_scrool = false;
        bool is_focused = false;

    UIMenu(int size_y, int size_x,int pos_y = 0 , int pos_x= 0)
    {
        window = newwin(size_y, size_x, pos_y, pos_x);
        panel = new_panel(window);
        hide();
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
    void render_menu()
    {
        show();
        box(window, 0, 0);
        for (int i = 0; i < options.size(); i++)
        {
            if (i==selected_option)
            {
                wattron(window,A_STANDOUT);
            }
            print_centered_text(window,2+i, options[i].c_str(),COLOR_WHITE,COLOR_BLACK);
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
        render_menu();
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
        }
    }
};

#endif