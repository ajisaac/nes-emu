#ifndef NES_EMU_CARTRIDGE_H
#define NES_EMU_CARTRIDGE_H

#include <vector>

class Cartridge
{
public:
    // prg-rom banks
    // std::vector<char> prg;
    // chr-rom banks
    // std::vector<char> chr;
    // mapper type
    // int mapper;
    // mirroring mode
    // int mirror;
    // battery backed ram
    // int battery;


    explicit Cartridge(const std::string_view path)
    {
        std::cout << path << std::endl;
        // open the file at path
        // throw exception if not there
        //
    }
};

#endif //NES_EMU_CARTRIDGE_H
