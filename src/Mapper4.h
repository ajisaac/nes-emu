#ifndef NES_EMU_MAPPER4_H
#define NES_EMU_MAPPER4_H
#include <cstdint>

#include "Mapper.h"

class Cartridge;
class Console;

class Mapper4 : public Mapper {
public:
    Cartridge *cartridge;
    Console *console;
    uint8_t registerIndex{}; // Go calls this register, which is a reserved word in C++
    uint8_t registers[8]{};
    uint8_t prgMode{};
    uint8_t chrMode{};
    int prgOffsets[4]{};
    int chrOffsets[8]{};
    uint8_t reload{};
    uint8_t counter{};
    bool irqEnable{};

    Mapper4(Console *console, Cartridge *cartridge);

    uint8_t read(uint16_t address) override;

    void write(uint16_t address, uint8_t value) override;

    void step() override;

    void HandleScanLine();

    void writeRegister(uint16_t address, uint8_t value);

    void writeBankSelect(uint8_t value);

    void writeBankData(uint8_t value);

    void writeMirror(uint8_t value);

    void writeProtect(uint8_t value);

    void writeIRQLatch(uint8_t value);

    void writeIRQReload(uint8_t value);

    void writeIRQDisable(uint8_t value);

    void writeIRQEnable(uint8_t value);

    int prgBankOffset(int index) const;

    int chrBankOffset(int index) const;

    void updateOffsets();
};

#endif //NES_EMU_MAPPER4_H
