#include "Console.h"

#include <cstdint>
#include <utility>

#include "ines.h"

Console::Console(std::string const &path) {
    cartridge = LoadNESFile(path);
    ram = new Ram(this);
    controller1 = new Controller();
    controller2 = new Controller();
    mapper = NewMapper(this);
    cpu = new CPU(ram);
    apu = new APU(this);
    ppu = new PPU(this);
}

Console::~Console() {
    delete ppu;
    delete apu;
    delete cpu;
    delete mapper;
    delete controller2;
    delete controller1;
    delete ram;
    delete cartridge;
}

void Console::reset() {
    cpu->Reset();
}

int Console::step() {
    const int cpuCycles = cpu->step();
    const int ppuCycles = cpuCycles * 3;
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
    int cpuCycles = 0;
    const uint64_t frame = ppu->frame;
    while (frame == ppu->frame) {
        cpuCycles += step();
    }
    return cpuCycles;
}

void Console::stepSeconds(const double seconds) {
    auto cycles = static_cast<int64_t>(CPUFrequency * seconds);
    while (cycles > 0) {
        cycles -= step();
    }
}

PPU::Buffer *Console::buffer() {
    return ppu->front.get();
}

Color Console::backgroundColor() const {
    return Palette[ppu->readPalette(0) % 64];
}

void Console::setButtons1(const std::array<bool, 8> &buttons) {
    controller1->setButtons(buttons);
}

void Console::setButtons2(const std::array<bool, 8> &buttons) {
    controller2->setButtons(buttons);
}

void Console::setAudioChannel(std::function<void(float)> channel) {
    apu->channel = std::move(channel);
}

void Console::setAudioSampleRate(const double sampleRate) {
    if (sampleRate != 0) {
        // Convert samples per second to cpu steps per sample
        apu->sampleRate = CPUFrequency / sampleRate;
        // Initialize filters
        apu->filterChain = FilterChain{
            HighPassFilter(static_cast<float>(sampleRate), 90),
            HighPassFilter(static_cast<float>(sampleRate), 440),
            LowPassFilter(static_cast<float>(sampleRate), 14000),
        };
    } else {
        apu->filterChain = FilterChain{};
    }
}
