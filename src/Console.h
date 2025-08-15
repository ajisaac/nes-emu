#ifndef CPPTEST_CONSOLE_H
#define CPPTEST_CONSOLE_H
#include <string>

#include "APU.h"
#include "Cartridge.h"
#include "Controller.h"
#include "Cpu.h"
#include "Mapper.h"
#include "PPU.h"
#include "Ram.h"

class PPU;
class APU;
class Controller;
class Cartridge;
class Mapper;
class Ram;
class Cpu;


class Console {
public:
    PPU ppu{this};
    APU apu{};
    Controller controller1{};
    Controller controller2{};
    Cartridge cartridge;
    Mapper mapper{};
    Ram ram;
    Cpu cpu;

    explicit Console(std::string const &path);

    void reset();

    int step();

    int stepFrame();

    void stepSeconds(unsigned long long seconds);

    int *buffer();

    int backgroundColor();

    void setButtons1(char buttons);

    void setButtons2(char buttons);

    void setAudioChannel();

    void setAudioSampleRate(long long sampleRate);

    void saveState(std::string filename);

    void loadState(std::string filename);

    ~Console() = default;
};

#endif
