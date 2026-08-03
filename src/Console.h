#ifndef CPPTEST_CONSOLE_H
#define CPPTEST_CONSOLE_H
#include <string>

#include "APU.h"
#include "Cartridge.h"
#include "Controller.h"
#include "CPU.h"
#include "Mapper.h"
#include "PPU.h"
#include "Ram.h"


class Console {
public:
    CPU *cpu;
    APU *apu;
    PPU *ppu;
    Controller *controller1;
    Controller *controller2;
    Cartridge *cartridge;
    Mapper *mapper;
    Ram *ram;

    explicit Console(std::string const &path);

    void reset();

    int step();

    int stepFrame();

    void stepSeconds(double seconds);

    PPU::Buffer *buffer();

    PPU::Buffer *backgroundColor();

    void setButtons1(bool buttons[8]);

    void setButtons2(bool buttons[8]);

    void setAudioChannel(); // todo

    void setAudioSampleRate(double sampleRate);

    ~Console() = default;
};

#endif
