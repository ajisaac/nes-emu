#ifndef NES_EMU_CARTRIDGE_H
#define NES_EMU_CARTRIDGE_H

#include <vector>

// Cartridge loaded from a .nes file
// http://wiki.nesdev.com/w/index.php/INES
// http://nesdev.com/NESDoc.pdf (page 28)
class Cartridge {

public:
    // prg-rom banks
    std::vector<char> prg;
    // chr-rom banks
    std::vector<char> chr;
    // mapper type
    int mapper;
    // mirroring mode
    int mirror;
    // battery backed ram
    int battery;


    Cartridge(std::vector<char> prg, std::vector<char> chr, int mapper, int mirror, int battery)
            : prg(std::move(prg)), chr(std::move(chr)), mapper(mapper), mirror(mirror), battery(battery) {
    }
};

#endif //NES_EMU_CARTRIDGE_H
