#ifndef CPPTEST_CONSOLE_H
#define CPPTEST_CONSOLE_H
#include <array>
#include <functional>
#include <string>

#include "APU.h"
#include "Cartridge.h"
#include "Controller.h"
#include "CPU.h"
#include "Mapper.h"
#include "Palette.h"
#include "PPU.h"
#include "Ram.h"


class Console {
public:
    CPU *cpu{};
    APU *apu{};
    PPU *ppu{};
    Cartridge *cartridge{};
    Controller *controller1{};
    Controller *controller2{};
    Mapper *mapper{};
    Ram *ram{};

    explicit Console(std::string const &path);

    // every component keeps a pointer back to its console, so it can't be copied
    Console(const Console &) = delete;

    Console &operator=(const Console &) = delete;

    ~Console();

    void reset();

    int step();

    int stepFrame();

    void stepSeconds(double seconds);

    PPU::Buffer *buffer();

    Color backgroundColor() const;

    void setButtons1(const std::array<bool, 8> &buttons);

    void setButtons2(const std::array<bool, 8> &buttons);

    void setAudioChannel(std::function<void(float)> channel);

    void setAudioSampleRate(double sampleRate);
};

#endif
