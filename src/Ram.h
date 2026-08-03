#ifndef RAM_H
#define RAM_H

#include <iostream>

class PPU;
class APU;
class Controller;
class Mapper;
class Console;

class Ram {
public:
    PPU *ppu;
    APU *apu;
    Controller *controller1;
    Controller *controller2;
    Mapper *mapper;
    Console *console;

    Ram(PPU *ppu, APU *apu, Controller *controller1, Controller *controller2, Mapper *mapper, Console *console) {
        this->ppu = ppu;
        this->apu = apu;
        this->controller1 = controller1;
        this->controller2 = controller2;
        this->mapper = mapper;
        this->console = console;
    }

    uint8_t read(const uint16_t address) const;

    void write(const uint16_t address, const uint8_t value);

    uint8_t ReadPpuMemory(uint16_t address);

    void WritePpuMemory(uint16_t address, const uint8_t value);

    uint16_t MirrorAddress(uint8_t mode, uint16_t address);
};

#endif //RAM_H
