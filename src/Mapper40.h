#ifndef NES_EMU_MAPPER40_H
#define NES_EMU_MAPPER40_H
#include <cstdint>

#include "Mapper.h"

class Cartridge;
class Console;

class Mapper40 : public Mapper {
public:
    Cartridge *cartridge;
    Console *console;
    int bank{};
    int cycles{};

    Mapper40(Console *console, Cartridge *cartridge);

    uint8_t read(uint16_t address) override;

    void write(uint16_t address, uint8_t value) override;

    void step() override;
};

#endif //NES_EMU_MAPPER40_H
