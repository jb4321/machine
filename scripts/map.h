#ifndef MAP_H
#define MAP_H

#include "color_handling.h"
#include "FastNoise.h"

class InfoGenericObject
{
    private:

    public:
        const char* sign;
        int fg;
        int bg;
        bool is_pass;
    InfoGenericObject(const char* sign_, int fg_ = -1, int bg_ = -1,bool is_pass_ = true)
    {
        sign = sign_;
        fg = fg_;
        bg = bg_;
    }
    InfoGenericObject(int bg_,bool is_pass_ = true)
    {
        sign = " ";
        fg = -1;
        bg = bg_;
        is_pass = is_pass_;
    }
    InfoGenericObject()
    {
        sign = " ";
        fg = -1;
        bg = -1;
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
    {"cya", InfoGenericObject("$",COLOR_BLACK,COLOR_CYAN)}, 
    {"default" , InfoGenericObject("χ",COLOR_BLACK,COLOR_WHITE)},
    {"grass" , InfoGenericObject("░",COLOR_CYAN)},
    {"a" , InfoGenericObject("a",COLOR_MAGENTA)},
    {"b" , InfoGenericObject("b",COLOR_CYAN)},
    {"c" , InfoGenericObject("c",COLOR_BLACK)},
    {"d" , InfoGenericObject("d",COLOR_MAGENTA)},
    //FLOORS
    {"sand" , InfoGenericObject(COLOR_YELLOW)},
    {"grass_floor" , InfoGenericObject(COLOR_GREEN)},
    {"stone_floor" , InfoGenericObject(COLOR_WHITE)}
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
        InfoGenericObject render(object.sign,object.fg,object.bg);
        if (object.sign == " ")
        {
            render.sign = floor_i.sign;
        }
        if (object.fg <= -1)
        {
            render.fg = floor_i.fg;
        }
        if (object.bg <= -1)
        {
            render.bg = floor_i.bg;
        }
        return render;
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
    {"desert", Biome("sand",0.5)},
    {"plains", Biome("grass_floor",-1,{GenericFeature(0.5,GenericObject("grass"))})},
    {"stone_desert", Biome("stone_floor",0.7)} 
};


//WORLD STRUCTURE


class Chunk
{
    public:
        static constexpr float SCALING_CH = 4;
        static const int CHUNK_SIZE = 4;
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
        srand(123);
        map_noise.SetSeed(123);
    }
    Chunk GenerateChunk(int ch_y, int ch_x)
    {
        Chunk new_chunk = Chunk(ch_y,ch_x,map_noise);
        world_chunk_data[{ch_y,ch_x}] = new_chunk;
        return new_chunk;
    }
    Chunk GetChunk(int ch_y, int ch_x)
    {
        return world_chunk_data[{ch_y,ch_x}];
    }
    Cell GetCell(int y_, int x_)
    {
        int ch_y = std::floor(float(y_)/Chunk::CHUNK_SIZE);
        int ch_x = std::floor(float(x_)/Chunk::CHUNK_SIZE);
        if (world_chunk_data.find({ch_y,ch_x}) != world_chunk_data.end())
        {
            return GetChunk(ch_y,ch_x).GetCell(y_,x_);
        }
        if(generate_world)
        {
            return GenerateChunk(ch_y,ch_x).GetCell(y_,x_);
        }
        
        return Cell();
    }
    
};

#endif