#ifndef CPPTEST_CONSOLE_H
#define CPPTEST_CONSOLE_H

#include <bitset>

#include "Cpu.h"
#include "Ram.h"
#include "PPU.h"
#include "Cartridge.h"
#include "Mapper.h"
#include "Controller.h"

class Console {
public:
    Ram ram;
    Cpu cpu{&ram};
    PPU ppu;
    Mapper mapper;

    constexpr static int CPUFrequency = 1789773;

    Console(){};

    explicit Console(Cartridge cartridge);

    void run();

    void step(long i);
};


//
//public long step() {
//        int cpuCycles = this.cpu.step();
//        int ppuCycles = cpuCycles * 3;
//        for (int i = 0; i < ppuCycles; i++) {
//            this.ppu.step();
//            this.mapper.step();
//        }
//        for (int i = 0; i < cpuCycles; i++) {
//            this.apu.step();
//        }
//        return cpuCycles;
//    }
//
//public void step(long diff) {
//        long cycles = CPUFrequency * diff;
//        while (cycles > 0) {
//            cycles -= this.step();
//        }
//    }
//
//public void reset() {
//        // reset the cpu
//        cpu.reset();
//    }
//
//}
#endif
