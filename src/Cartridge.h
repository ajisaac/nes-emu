#ifndef NES_EMU_CARTRIDGE_H
#define NES_EMU_CARTRIDGE_H

#include <cstdint>
#include <vector>

// mirroring modes, the values Cartridge::mirror can hold
enum {
    MirrorHorizontal = 0,
    MirrorVertical = 1,
    MirrorSingle0 = 2,
    MirrorSingle1 = 3,
    MirrorFour = 4
};

class Cartridge {
public:
    std::vector<uint8_t> prg;  // PRG-ROM banks
    std::vector<uint8_t> chr;  // CHR-ROM banks
    std::vector<uint8_t> sram; // Save RAM
    uint8_t mapper;            // mapper type
    uint8_t mirror;            // mirroring mode
    uint8_t battery;           // battery present

    Cartridge(std::vector<uint8_t> prg, std::vector<uint8_t> chr, uint8_t mapper, uint8_t mirror, uint8_t battery);
};

#endif //NES_EMU_CARTRIDGE_H
