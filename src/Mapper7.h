#ifndef NES_EMU_MAPPER7_H
#define NES_EMU_MAPPER7_H
#include <cstdint>

#include "Mapper.h"

class Cartridge;

class Mapper7 : public Mapper {
public:
    Cartridge *cartridge;
    int prgBank{};

    explicit Mapper7(Cartridge *cartridge);

    uint8_t read(uint16_t address) override;

    void write(uint16_t address, uint8_t value) override;
};

#endif //NES_EMU_MAPPER7_H
