// Cartridge loaded from a .nes file
// http://wiki.nesdev.com/w/index.php/INES
// http://nesdev.com/NESDoc.pdf (page 28)

#ifndef NES_EMU_CARTRIDGE_H
#define NES_EMU_CARTRIDGE_H

#include <string>
#include "INes.h"

class Cartridge {
    // check if file exists
    // check if headers are proper
    // start loading the cartridge
    // throw ise if fails

    public:
    explicit Cartridge(std::string rom);

    // PRG-ROM
    char prg[1]{};

    // CHR-ROM
    char chr[1]{};

    // mapper type for this game
    char mapper{};

    // mirroring mode?
    char mirror{};

    // does battery exist?
    char battery{};

    void loadRom(INes nes);
};

#endif //NES_EMU_CARTRIDGE_H
