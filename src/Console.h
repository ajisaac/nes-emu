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

    void reset() {
        cpu.reset();
    }

    int step() {
        const auto cpuCycles = cpu.step();
        const auto ppuCycles = cpuCycles * 3;
        for (int i = 0; i < ppuCycles; i++) {
            ppu.step();
            mapper.step();
        }
        for (int i = 0; i < cpuCycles; i++) {
            apu.step();
        }
        return cpuCycles;
    }

    int stepFrame() {
        auto cpuCycles = 0;
        unsigned long long frame = ppu.frame;
        while (frame == ppu.frame) {
            cpuCycles += step();
        }
        return cpuCycles;
    }

    void stepSeconds(unsigned long long seconds) {
        // cpu frequency
        auto cycles = 1789773 * seconds * 1000;
        while (cycles > 0) {
            cycles -= step();
        }
    }

    int* buffer() {
        // return ppu.front();
        return nullptr;
    }

    int backgroundColor() {
        return 0;
    }

    void setButtons1(char buttons) {}

    void setButtons2(char buttons) {}

    void setAudioChannel() {}

    void setAudioSampleRate(long long sampleRate) {}

    void saveState(std::string filename) {}

    void loadState(std::string filename) {}

    ~Console() = default;
};

#endif
