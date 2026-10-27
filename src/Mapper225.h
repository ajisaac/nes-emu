#ifndef NES_EMU_MAPPER225_H
#define NES_EMU_MAPPER225_H
#include <cstdint>

#include "Mapper.h"

class Cartridge;

// https://github.com/asfdfdfd/fceux/blob/master/src/boards/225.cpp
// https://wiki.nesdev.com/w/index.php/INES_Mapper_225

class Mapper225 : public Mapper {
public:
    Cartridge *cartridge;
    int chrBank{};
    int prgBank1{};
    int prgBank2{};

    explicit Mapper225(Cartridge *cartridge);

    uint8_t read(uint16_t address) override;

    void write(uint16_t address, uint8_t value) override;
};

#endif //NES_EMU_MAPPER225_H
