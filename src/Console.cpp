
#include "Console.h"

Console::Console(std::string const& path)
    : cartridge{path},
      ram{&ppu, &apu, &controller1, &controller2, &mapper, this},
      cpu{&ram} {
    // cartridge should automatically load
}

void Console::reset() {
    cpu.Reset();
}

int Console::step() {
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

int Console::stepFrame() {
    auto cpuCycles = 0;
    unsigned long long frame = ppu.Frame;
    while (frame == ppu.Frame) {
        cpuCycles += step();
    }
    return cpuCycles;
}

void Console::stepSeconds(const unsigned long long seconds) {
    // cpu frequency
    auto cycles = 1789773 * seconds * 1000;
    while (cycles > 0) {
        cycles -= step();
    }
}

int* Console::buffer() {
    // return ppu.front();
    return nullptr;
}

int Console::backgroundColor() {
    return 0;
}

void Console::setButtons1(char buttons) {}

void Console::setButtons2(char buttons) {}

void Console::setAudioChannel() {}

void Console::setAudioSampleRate(long long sampleRate) {}

void Console::saveState(std::string filename) {}

void Console::loadState(std::string filename) {}
