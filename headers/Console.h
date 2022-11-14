#ifndef CPPTEST_CONSOLE_H
#define CPPTEST_CONSOLE_H

#include <bitset>
#include "Cpu.h"
#include "Ram.h"

class Console {
    // it contains busses, a clock, the Cpu, ppu, cartridge - it's our master program
private:
    std::bitset<16> address_bus{0};      // our 16 bit bus
    std::bitset<8> data_bus{0};          // 8-bit data bus
    std::bitset<8> control_bus{0};       // 8-bit control bus
    Ram ram{};
    Cpu cpu{&ram};

public:
    Console();

    ~Console();

    void run();
};

#endif
