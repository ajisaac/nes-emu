#ifndef RAM_H
#define RAM_H

#include <iostream>
#include "PPU.h"
#include "APU.h"
#include "Controller.h"
#include "Mapper.h"

class Ram {
public:
    PPU *ppu;
    APU *apu;
    Controller *controller1;
    Controller *controller2;
    Mapper *mapper;

    Ram(PPU *ppu, APU *apu, Controller *controller1, Controller *controller2, Mapper *mapper) {
        this->ppu = ppu;
        this->apu = apu;
        this->controller1 = controller1;
        this->controller2 = controller2;
        this->mapper = mapper;
    }

    uint8_t read(const uint16_t address) const;

    void write(const uint16_t address, const uint8_t value);

    uint8_t ReadPpuMemory(uint16_t address);

    void WritePpuMemory(uint16_t address, const uint8_t value);

    uint16_t MirrorAddress(uint8_t mode, uint16_t address);
};

#endif //RAM_H
