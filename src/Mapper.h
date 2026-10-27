#ifndef NES_EMU_MAPPER_H
#define NES_EMU_MAPPER_H
#include <cstdint>

class Console;

class Mapper {
public:
    virtual ~Mapper() = default;

    virtual uint8_t read(uint16_t address) = 0;

    virtual void write(uint16_t address, uint8_t value) = 0;

    // only the mappers that count cycles or scanlines do anything here
    virtual void step() {
    }
};

// NewMapper creates the mapper the console's cartridge uses. Throws
// std::runtime_error if that mapper isn't supported.
Mapper *NewMapper(Console *console);

#endif //NES_EMU_MAPPER_H
