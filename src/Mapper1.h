#ifndef NES_EMU_MAPPER1_H
#define NES_EMU_MAPPER1_H
#include <cstdint>

#include "Mapper.h"

class Cartridge;

class Mapper1 : public Mapper {
public:
    Cartridge *cartridge;
    uint8_t shiftRegister{};
    uint8_t control{};
    uint8_t prgMode{};
    uint8_t chrMode{};
    uint8_t prgBank{};
    uint8_t chrBank0{};
    uint8_t chrBank1{};
    int prgOffsets[2]{};
    int chrOffsets[2]{};

    explicit Mapper1(Cartridge *cartridge);

    uint8_t read(uint16_t address) override;

    void write(uint16_t address, uint8_t value) override;

    void loadRegister(uint16_t address, uint8_t value);

    void writeRegister(uint16_t address, uint8_t value);

    // Control (internal, $8000-$9FFF)
    void writeControl(uint8_t value);

    // CHR bank 0 (internal, $A000-$BFFF)
    void writeCHRBank0(uint8_t value);

    // CHR bank 1 (internal, $C000-$DFFF)
    void writeCHRBank1(uint8_t value);

    // PRG bank (internal, $E000-$FFFF)
    void writePRGBank(uint8_t value);

    int prgBankOffset(int index) const;

    int chrBankOffset(int index) const;

    void updateOffsets();
};

#endif //NES_EMU_MAPPER1_H
