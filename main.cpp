#include <iostream>
#include <vector>
#include <cstring>

#include <pdcurses.h>
//zmienne

class Cell
{
    private:
        /* data */
    public:
    std::string name;
    Cell(std::string name_ =  "default")
    {
        name = name_;
    }
};
class Chunk
{   
    private:
        /* data */
    public:
        const int CHUNK_SIZE = 16;
        Cell chunk_data[16][16];
        
    Chunk(int x_,int y_)
    {
        GenerateChunk();
        CommandLineChunk();
    }
    void GenerateChunk()
    {
        for (int i = 0; i < CHUNK_SIZE; i++)
        {
            for (int j = 0; j < CHUNK_SIZE; j++)
            {
                std::string name_it;
                if(i%2==0){name_it = "a";}
                else{name_it = "b";}
                chunk_data[i][j]= Cell(name_it);
            }
            
        }
        
    }
    void CommandLineChunk()
    {
        for (int i = 0; i < CHUNK_SIZE; i++)
        {
            for (int j = 0; j < CHUNK_SIZE; j++)
            {
                std::cout << chunk_data[i][j].name<<" ";
            }
            std::cout << std::endl;
            
        }
    }
    void RenderChunk(int cam_y, int cam_x)
    {
        //i -> y and j -> x;
        for (int i = 0; i < CHUNK_SIZE; i++)
        {
            
            for (int j = 0; j < CHUNK_SIZE; j++)
            {

                mvprintw(i-cam_y,j-cam_x,chunk_data[i][j].name.c_str());
            }
            
        }
    }
};
class Map
{
    private:
        /* data */
    public:

};



int x = 0;
int y = 0;
int term_x = 60;
int term_y = 30;
Chunk chunk1(1,2);
void RenderGame()
{
    chunk1.RenderChunk(y,x);
}
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
void myclear()
{

}
void GameLoop()
{
    do
    {
        int ny=y;
        int nx=x; 
        int key = getch();
        
        switch (key)
        {
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
        
        if (key == KEY_F(1))
        {
            break;
        }
        wclear(stdscr);
        
        x = nx;
        y = ny;
        /*if(IsNotOut(ny,nx))
        {
            x = nx;
            y = ny;
        }*/
        RenderGame();
        mvprintw(term_y/2,term_x/2,"@");
        
        std::string txt = std::to_string(x) + "," + std::to_string(y) + " ";
        attron(A_BOLD);
        attron(COLOR_PAIR(1));

        mvprintw(0,0,txt.c_str());
        attroff(A_BOLD);
        attroff(COLOR_PAIR(1));
        
        move(term_y/2,term_x/2);
        refresh();
    } while (true);
}
void Start()
{
    #ifdef XCURSES
        Xinitscr(argc, argv);
    #else
        initscr();
    #endif
    def_prog_mode();
    raw();
    resize_term(term_y, term_x);
    refresh();
    if(has_colors() == FALSE)
	{	endwin();
		printf("Your terminal does not support color\n");
		exit(1);
	}
    
    start_color();
    init_pair(1,COLOR_RED,COLOR_BLACK);

    printw( "Press Button..\n" );
    keypad(stdscr, TRUE);	
    noecho();
    move(y,x);
}
void End()
{
    printw( "End" );
    getch();
    endwin();
}
int main(int argc, char **argv)
{
    
    Start();
    GameLoop();
    End();
    return 0;
}
