#ifndef CPPTEST_CONSOLE_H
#define CPPTEST_CONSOLE_H

#include "Cpu.h"
#include "Ram.h"
#include "Cartridge.h"

class Console {
public:
    PPU ppu{};
    APU apu{};
    Controller controller1{};
    Controller controller2{};
    Cartridge cartridge;
    Mapper mapper{};
    Ram ram;
    Cpu cpu;

    explicit Console(std::string const& path)
        : cartridge{path},
          ram{&ppu, &apu, &controller1, &controller2, &mapper},
          cpu{&ram} {
        // cartridge should automatically load
    }

    ~Console() = default;
};

#endif
