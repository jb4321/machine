#ifndef MAP_H
#define MAP_H

#include "color_handling.h"
class CellObjectInfo
{
    private:

    public:
    const char* sign;
    int fg;
    int bg;
    CellObjectInfo(const char* sign_, int fg_, int bg_)
    {
        sign = sign_;
        fg = fg_;
        bg = bg_;
    }
    CellObjectInfo()
    {
        sign = " ";
        fg = 0;
        bg = 0;
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
std::map<std::string,CellObjectInfo> cell_info_table = {
    {"default" , CellObjectInfo("χ",COLOR_BLACK,COLOR_WHITE)},
    {"grass" , CellObjectInfo("▓",COLOR_BLACK,COLOR_WHITE)},
    {"a" , CellObjectInfo("g",COLOR_BLACK,COLOR_CYAN)},
    {"b" , CellObjectInfo("g",COLOR_BLACK,COLOR_YELLOW)},
    {"c" , CellObjectInfo("g",COLOR_BLACK,COLOR_GREEN)},
    {"d" , CellObjectInfo("g",COLOR_BLACK,COLOR_BLUE)}
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
    CellObjectInfo get_info()
    {
        return cell_info_table[name];
    }
};
class Chunk
{   
    private:
        /* data */
    public:
        static const int CHUNK_SIZE = 16;
        Cell chunk_data[CHUNK_SIZE][CHUNK_SIZE];
        std::string color;
    Chunk(std::string color_)
    {
        color = color_;
        
        GenerateChunk();
    }
    Chunk()
    {
        color = "a";
    }
    void GenerateChunk()
    {
        std::cout << color;
        for (int i = 0; i < CHUNK_SIZE; i++)
        {
            for (int j = 0; j < CHUNK_SIZE; j++)
            {
                chunk_data[i][j]= Cell(color);
            }
        }
    }
    Cell GetCell(int y_, int x_)
    {
        return chunk_data[y_][x_];
    }
};
class WorldMap
{
    private:
        /* data */
    public:
        std::map<std::vector<int>,Chunk> world_chunk_data = {
            {{0,0} , Chunk("a")},
            {{1,0} , Chunk("b")},
            {{0,1} , Chunk("c")},
            {{1,1} , Chunk("d")}
        }; 

    Cell GetCell(int y_, int x_)
    {
        int ch_y = std::floor(float(y_)/Chunk::CHUNK_SIZE);
        int ch_x = std::floor(float(x_)/Chunk::CHUNK_SIZE);
        if (world_chunk_data.find({ch_y,ch_x}) ==world_chunk_data.end())
        {
            return Cell();
        }
        Chunk w_chunk = world_chunk_data[{ch_y,ch_x}];
        return w_chunk.GetCell(ToLocalPos(y_),ToLocalPos(x_));
    }
    int ToLocalPos(int n)
    {
        if (n < 0)
        {
            return ((n % Chunk::CHUNK_SIZE) + Chunk::CHUNK_SIZE) % Chunk::CHUNK_SIZE;
        }
        return n % Chunk::CHUNK_SIZE;
    }
};

#endif