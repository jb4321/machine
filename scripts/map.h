#ifndef MAP_H
#define MAP_H


class CellObjectInfo
{
    private:

    public:
    char sign;
    int color;
    
};
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
#endif