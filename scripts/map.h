#ifndef MAP_H
#define MAP_H

#include "color_handling.h"
#include "FastNoise.h"
#include <ctime>

class RenderInfo
{
    public:
        const char* sign;
        int fg;
        int bg;
    RenderInfo(const char* sign_, int fg_ = -1, int bg_ = -1)
    {
        sign = sign_;
        fg = fg_;
        bg = bg_;
    }
    RenderInfo(int bg_,bool is_pass_ = true)
    {
        sign = " ";
        fg = -1;
        bg = bg_;
    }
    RenderInfo()
    {
        sign = " ";
        fg = -1;
        bg = -1;
    }
};
class InfoGenericObject
{
    private:

    public:
        RenderInfo render;
        bool is_pass;
        int tool_tier;

    InfoGenericObject(RenderInfo render_, bool is_pass_ = true,int tool_tier_ = 0)
    {
        render = render_;
        is_pass = is_pass_;
        tool_tier = tool_tier_;
    }
    InfoGenericObject()
    {
        render = RenderInfo();
        is_pass = true;
    }
};

/*
!"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~⌂ ¡¢£¤¥¦§¨©ª«¬-®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ
    ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷øùúûüýþÿĀāĂăĄąĆćĈĉĊċČčĎďĐđĒēĔĕĖėĘęĚěĜĝĞğĠġĢģĤĥĦħĨĩĪīĬĭĮįİıĲĳĴĵĶķĸĹĺĻļĽľĿŀŁłŃńŅņŇňŉŊŋŌōŎŏŐőŒœŔŕŖŗŘřŚśŜŝŞşŠšŢţŤťŦŧŨũŪūŬ
    ŭU+016DŮůŰűŲųŴŵŶŷŸŹźŻżŽžſƒơƷǺǻǼǽǾǿȘșȚțɑɸˆˇˉ˘˙˚˛˜˝;΄΅Ά·ΈΉΊΌΎΏΐΑΒΓΔΕΖΗΘΙΚΛΜΝΞΟΠΡΣΤΥΦΧΨΩΪΫάέήίΰαβγδεζηθικλμνξοπρςστυφχψωϊϋόύώϐϴЀЁЂЃЄЅІЇЈЉЊЋЌЍЎЏАБВГДЕЖЗИЙ
    КЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдежзийклмнопрстуфхцчшщъыьэюяѐёђѓєѕіїјљњћќѝўџҐґ־אבגדהוזחטיךכלםמןנסעףפץצקרשתװױײ׳״ᴛᴦᴨẀẁẂẃẄẅẟỲỳ‐‒–—―‗‘’‚‛“”„‟†‡•…‧‰′″‵‹›‼‾‿⁀
    ⁄⁔⁴⁵⁶⁷⁸⁹⁺⁻ⁿ₁₂₃₄₅₆₇₈₉₊₋₣₤₧₪€℅ℓ№™Ω℮⅐⅑⅓⅔⅕⅖⅗⅘⅙⅚⅛⅜⅝⅞←↑→↓↔↕↨∂∅∆∈∏∑−∕∙√∞∟∩∫≈≠≡≤≥⊙⌀⌂⌐⌠⌡─│┌┐└┘├┤┬┴┼═║╒╓╔╕╖╗╘╙╚╛╜╝╞╟╠╡╢╣╤╥╦╧╨╩╪╫╬▀▁▄█▌▐░▒▓■□▪▫▬▲►▼◄◊○●◘◙◦☺☻☼♀
    ♂♠♣♥♦♪♫✓ﬁﬂ�
*/
std::map<std::string,InfoGenericObject> object_info_table = {
    {"none", InfoGenericObject()},
    {"cya", InfoGenericObject(RenderInfo("$",COLOR_BLACK,COLOR_CYAN))}, 
    {"default" , InfoGenericObject(RenderInfo("χ",COLOR_BLACK,COLOR_WHITE))},
    {"Grass" , InfoGenericObject(RenderInfo("▒",COLOR_YELLOW))},
    {"Tree" , InfoGenericObject(RenderInfo("T",COLOR_BLACK))},
    {"Acid Flower" , InfoGenericObject(RenderInfo("A",COLOR_CYAN))},

    {"Tin" , InfoGenericObject(RenderInfo("L",COLOR_MAGENTA,COLOR_WHITE),false,0)},
    {"Coal" , InfoGenericObject(RenderInfo("F",COLOR_BLACK,COLOR_WHITE),false,1)},
    {"Copper" , InfoGenericObject(RenderInfo("C",COLOR_RED,COLOR_WHITE),false,1)},
    {"Stone" , InfoGenericObject(RenderInfo("⌂",COLOR_BLACK,COLOR_WHITE),false,1)},
    {"Iron" , InfoGenericObject(RenderInfo("I",COLOR_BLACK,COLOR_WHITE),false,0)},
    {"Oil" , InfoGenericObject(RenderInfo("O",COLOR_RED,COLOR_BLACK),false,2)},
    {"Titan" , InfoGenericObject(RenderInfo("T",COLOR_CYAN,COLOR_WHITE),false,2)},

    //FLOORS
    {"sand" , InfoGenericObject(RenderInfo("░",COLOR_BLACK,COLOR_YELLOW))},
    {"grass_floor" , InfoGenericObject(RenderInfo(COLOR_GREEN))},
    {"stone_floor" , InfoGenericObject(RenderInfo(COLOR_WHITE))},
    {"titan_floor" , InfoGenericObject(RenderInfo("░",COLOR_WHITE,COLOR_BLACK))}
};


class GenericObject
{
    public:
        std::string object_name;
    GenericObject(std::string object_name_ = "none")
    {
        object_name = object_name_;
    }
    InfoGenericObject GetInfo()
    {
        return object_info_table[object_name];
    }
};


class Cell
{
    private:
        /* data */
    public:
    GenericObject object_cell;
    GenericObject floor_object;
    Cell(std::string floor_ = "default",std::string name_ =  "none")
    {
        floor_object.object_name = floor_;
        object_cell.object_name = name_;
    }
    InfoGenericObject get_info()
    {
        InfoGenericObject object = object_cell.GetInfo();
        InfoGenericObject floor_i = floor_object.GetInfo();
        bool is_pass = object.is_pass && floor_i.is_pass;
        InfoGenericObject ret_render(object.render,is_pass);
        if (object.render.sign == " ")
        {
            ret_render.render.sign = floor_i.render.sign;
        }
        if (object.render.fg <= -1)
        {
            ret_render.render.fg = floor_i.render.fg;
        }
        if (object.render.bg <= -1)
        {
            ret_render.render.bg = floor_i.render.bg;
        }
        return ret_render;
    }
};
class GenericFeature
{
    public:
        float chance;
        GenericObject feature_object; 
    GenericFeature(float chance_,GenericObject feature_object_)
    {
        chance = chance_;
        feature_object = feature_object_;
    }
    GenericFeature()
    {
        chance = 0;
        feature_object = GenericObject();
    }
    bool Generate()
    {
        float r = (float)(rand()) / (float)(RAND_MAX);
        if (r <= chance)
        {
            return true;
        }
        return false;
    }
};
class Biome
{
    public:
        std::string biome_floor;
        float temp;
        std::vector<GenericFeature> gen_features;
    Biome(std::string biome_floor_, float temp_, std::vector<GenericFeature> gen_features_ = {})
    {
        biome_floor = biome_floor_;
        temp = temp_;
        gen_features = gen_features_;
    }
    Biome()
    {
        biome_floor = "cya";
        temp = 2;
    }
    GenericObject chooseRandomFeature()
    {
        
        for (int i =0; i < gen_features.size();i++)
        {
            if (gen_features[i].Generate())
            {
                return gen_features[i].feature_object;
            }
        }
        return GenericObject();

    }
    Cell GetCell()
    {
        Cell new_cell = Cell();
        GenericObject generated_feature = chooseRandomFeature();
        new_cell.object_cell = generated_feature;
        
        new_cell.floor_object.object_name = biome_floor;
        return new_cell;
    }
};

std::map<std::string,Biome> biome_info = {
    {"none", Biome()}, //"none" is debug biome, 2 cant be reached by noise functions
    
    {"tin_desert", Biome("stone_floor",-1,{
        GenericFeature(0.02,GenericObject("Coal")),
        GenericFeature(0.05,GenericObject("Tin"))
        })},
    {"plains", Biome("grass_floor",-0.8,{
        GenericFeature(0.5,GenericObject("Grass")),
        GenericFeature(0.02,GenericObject("Tree")),
        GenericFeature(0.004,GenericObject("Tin"))
        })},
    {"desert", Biome("sand",-0.2,{
        GenericFeature(0.01,GenericObject("Oil")),
        GenericFeature(0.02,GenericObject("Copper"))
    })},
    {"stone_desert", Biome("stone_floor",0.6,
    {
        GenericFeature(0.01,GenericObject("Tin")),
        GenericFeature(0.02,GenericObject("Iron")),
        GenericFeature(0.02,GenericObject("Coal")),
        GenericFeature(0.01,GenericObject("Copper"))
    })},
    {"titan_desert", Biome("stone_floor",0.8,
    {
        GenericFeature(0.04,GenericObject("Titan")),
        GenericFeature(0.02,GenericObject("Coal"))
    })}  
};


//WORLD STRUCTURE

class Chunk
{
    public:
        static constexpr float SCALING_CH = 1;
        static const int CHUNK_SIZE = 16;
        Cell chunk_data[CHUNK_SIZE][CHUNK_SIZE];
    Chunk(int ch_y, int ch_x,FastNoise map_noise)
    {
        GenerateChunk(ch_y,ch_x,map_noise);
    }
    Chunk()
    {
        EmptyChunk();
    }
    int ToLocalPos(int n)
    {
        if (n < 0)
        {
            return ((n % Chunk::CHUNK_SIZE) + Chunk::CHUNK_SIZE) % Chunk::CHUNK_SIZE;
        }
        return n % Chunk::CHUNK_SIZE;
    }
    std::string BiomeChoose(float noise_value)
    {
        float max_val = -2.0; // -1 is smallest value so max's will be bigger 
        std::string biome_name = "none";
        for (auto const& [key, val] : biome_info)
        {   
            if (val.temp <= noise_value){
                if(val.temp > max_val)
                {
                    max_val = val.temp;
                    biome_name = key;
                }
            }
        }
        return biome_name;

    }
    Cell GenerateCell(int y_, int x_,FastNoise map_noise)
    {
        Cell new_cell;
        
        float noise_value = map_noise.GetNoise(float(x_)*SCALING_CH,float(y_)*SCALING_CH); 
        std::string biome_name = BiomeChoose(noise_value);
        Biome cell_biome = biome_info[biome_name];
        //std::cout << map_noise.GetNoise(float(x_)*SCALING_CH,float(y_)*SCALING_CH);
        new_cell = cell_biome.GetCell();
        return new_cell;
    }
    void GenerateChunk(int ch_y, int ch_x,FastNoise map_noise)
    {
        for (int i = 0; i < CHUNK_SIZE; i++)
        {
            for (int j = 0; j < CHUNK_SIZE; j++)
            {
                chunk_data[i][j] = GenerateCell(ch_y*CHUNK_SIZE+i,ch_x*CHUNK_SIZE+j,map_noise);
            }
        }
    }
    void EmptyChunk()
    {
        for (int i = 0; i < CHUNK_SIZE; i++)
        {
            for (int j = 0; j < CHUNK_SIZE; j++)
            {
                chunk_data[i][j] = Cell();
            }
        }
    }
    Cell GetCell(int y_, int x_)
    {
        return chunk_data[ToLocalPos(y_)][ToLocalPos(x_)];
    }
    void SetCell(int y_,int x_,GenericObject setted_object)
    {
        chunk_data[ToLocalPos(y_)][ToLocalPos(x_)].object_cell.object_name = setted_object.object_name;
    }
};
class WorldMap
{
    private:
        /* data */
    public:
        FastNoise map_noise;
        std::map<std::vector<int>,Chunk> world_chunk_data;
        bool generate_world = true;

    WorldMap()
    {
        int seed = time(NULL);
        srand(seed);
        map_noise.SetSeed(seed);
    }
    Chunk *GenerateChunk(int ch_y, int ch_x)
    {
        Chunk new_chunk = Chunk(ch_y,ch_x,map_noise);
        world_chunk_data[{ch_y,ch_x}] = new_chunk;
        return &world_chunk_data[{ch_y,ch_x}];
    }
    Chunk *GetChunk(int ch_y, int ch_x)
    {
        return &world_chunk_data[{ch_y,ch_x}];
    }
    bool FindChunk(int ch_y, int ch_x)
    {
        return world_chunk_data.find({ch_y,ch_x}) != world_chunk_data.end();
    }
    Cell GetCell(int y_, int x_)
    {
        int ch_y = GetChunkPos(y_);
        int ch_x = GetChunkPos(x_);
        if (FindChunk(ch_y,ch_x))
        {
            return GetChunk(ch_y,ch_x)->GetCell(y_,x_);
        }
        if(generate_world)
        {
            return GenerateChunk(ch_y,ch_x)->GetCell(y_,x_);
        }
        
        return Cell();
    }
    bool SetCellObject(int y_, int x_,GenericObject setted_object)
    {
        int ch_y = GetChunkPos(y_);
        int ch_x = GetChunkPos(x_);
        if(FindChunk(ch_y,ch_x))
        {
            GetChunk(ch_y,ch_x)->SetCell(y_,x_,setted_object);
            return true;
        }
        return false;
    }
    int GetChunkPos(int n)
    {
        return std::floor(float(n)/Chunk::CHUNK_SIZE);
    }
    
};

#endif