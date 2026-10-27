#ifndef NES_EMU_MAPPER3_H
#define NES_EMU_MAPPER3_H
#include <cstdint>

#include "Mapper.h"

class Cartridge;

class Mapper3 : public Mapper {
public:
    Cartridge *cartridge;
    int chrBank{};
    int prgBank1{};
    int prgBank2{};

    explicit Mapper3(Cartridge *cartridge);

    uint8_t read(uint16_t address) override;

    void write(uint16_t address, uint8_t value) override;
};

#endif //NES_EMU_MAPPER3_H
