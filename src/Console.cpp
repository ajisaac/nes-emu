
#include "Console.h"


Console::Console(std::string const& path) {

    cartridge = LoadNESFile(path);
    // ram = make([]byte, 2048)
    ram = new Ram();
    controller1 = new Controller();
    controller2 = new Controller();
    mapper = new Mapper();
    cpu = new CPU();
    apu = new APU();
    ppu = new PPU();
}

void Console::reset() {
    cpu->Reset();
}

int Console::step() {
    const auto cpuCycles = cpu->step();
    const auto ppuCycles = cpuCycles * 3;
    for (int i = 0; i < ppuCycles; i++) {
        ppu->step();
        mapper->step();
    }
    for (int i = 0; i < cpuCycles; i++) {
        apu->step();
    }
    return cpuCycles;
}

int Console::stepFrame() {
    auto cpuCycles = 0;
    unsigned long long frame = ppu->frame;
    while (frame == ppu->frame) {
        cpuCycles += step();
    }
    return cpuCycles;
}

void Console::stepSeconds(double seconds) {
    // cpu frequency
    auto cycles = 1789773 * seconds * 1000;
    while (cycles > 0) {
        cycles -= step();
    }
}

PPU::Buffer *Console::buffer() {
    return &ppu->front;
}

PPU::Buffer *Console::backgroundColor() {
    return Palette[ppu->readPalette(0)%64];
}

void Console::setButtons1(char buttons) {}

void Console::setButtons2(char buttons) {}

void Console::setAudioChannel() {}

void Console::setAudioSampleRate(long long sampleRate) {}

void Console::saveState(std::string filename) {}

void Console::loadState(std::string filename) {}
