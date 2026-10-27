#ifndef RAM_H
#define RAM_H
#include <cstdint>

class Console;

// Ram is the console's 2KB of RAM plus the CPU and PPU memory maps: Go's
// Console.RAM, cpuMemory and ppuMemory in one. It's created before the parts it
// talks to, so it finds them through the console each time it needs them.
class Ram {
public:
    Console *console{};
    uint8_t RAM[2048]{};

    explicit Ram(Console *console);

    // CPU Memory Map

    uint8_t read(uint16_t address) const;

    void write(uint16_t address, uint8_t value);

    // PPU Memory Map

    uint8_t ReadPpuMemory(uint16_t address) const;

    void WritePpuMemory(uint16_t address, uint8_t value) const;

    static uint16_t MirrorAddress(uint8_t mode, uint16_t address);
};

#endif //RAM_H
