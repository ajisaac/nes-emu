#ifndef CPPTEST_CONSOLE_H
#define CPPTEST_CONSOLE_H

#include <bitset>

#include "Cpu.h"
#include "Ram.h"

class Console {
    // it contains busses, a clock, the Cpu, ppu, cartridge - it's our master
    // program
   private:
    std::bitset<16> address_bus{0};  // our 16 bit bus
    std::bitset<8> data_bus{0};      // 8-bit data bus
    std::bitset<8> control_bus{0};   // 8-bit control bus
    Ram ram{};
    Cpu cpu{&ram};

   public:
    Console();

    ~Console();

    void run();
};

#endif
package co.aisaac;

import java.io.IOException;

public class Console {
    Ram ram;
    Cpu cpu;
    Ppu ppu;
    Apu apu;

    Cartridge cartridge;
    Controller controller1;
    Controller controller2;
    Mapper mapper;

private int CPUFrequency = 1_789_773;

public Console(Cartridge cartridge) {

        this.cartridge = cartridge;

        this.controller1 = new Controller();
        this.controller2 = new Controller();

        // ram starts out blank
        this.ram = new Ram();
        this.mapper = new Mapper(ram);
        this.cpu = new Cpu(ram);
        this.ppu = new Ppu(ram);
        this.apu = new Apu(ram);
    }


public void run() {
        // start the console
        while (true) {
            cpu.step();
        }
    }

public long step() {
        int cpuCycles = this.cpu.step();
        int ppuCycles = cpuCycles * 3;
        for (int i = 0; i < ppuCycles; i++) {
            this.ppu.step();
            this.mapper.step();
        }
        for (int i = 0; i < cpuCycles; i++) {
            this.apu.step();
        }
        return cpuCycles;
    }

public void step(long diff) {
        long cycles = CPUFrequency * diff;
        while (cycles > 0) {
            cycles -= this.step();
        }
    }

public void reset() {
        // reset the cpu
        cpu.reset();
    }

}
