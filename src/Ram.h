#ifndef CPPTEST_RAM_H
#define CPPTEST_RAM_H

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

    char ram[2048] = {0};


    Ram(PPU *ppu, APU *apu, Controller *controller1, Controller *controller2, Mapper *mapper) {
        this->ppu = ppu;
        this->apu = apu;
        this->controller1 = controller1;
        this->controller2 = controller2;
        this->mapper = mapper;
    }

    char Read(short address) {
        if (address < 0x2000) {
            return ram[address % 0x0800];
        } else if (address < 0x4000) {
            return ppu->readRegister(0x2000 + address % 8);
        } else if (address == 0x4014) {

            return ppu->readRegister(address);
        } else if (address == 0x4015) {

            return apu->readRegister(address);
        } else if (address == 0x4016) {

            return controller1->Read();
        } else if (address == 0x4017) {

            return controller2->Read();
        } else if (address < 0x6000) {
            // TODO: I/O registers
            return 0;
        } else if (address >= 0x6000) {
            return mapper->Read(address);
        } else {
            std::cout << "unhandled cpu memory read at address: " << address << std::endl;
        }
        return 0;
    }

    void Write(short address, char value) {
        if (address < 0x2000) {
            ram[address % 0x0800] = value;
        } else if (address < 0x4000) {
            ppu->writeRegister(0x2000 + address % 8, value);
        } else if (address < 0x4014) {
            apu->writeRegister(address, value);
        } else if (address == 0x4014) {
            ppu->writeRegister(address, value);
        } else if (address == 0x4015) {
            apu->writeRegister(address, value);
        } else if (address == 0x4016) {
            controller1->Write(value);
            controller2->Write(value);
        } else if (address == 0x4017) {
            apu->writeRegister(address, value);
        } else if (address < 0x6000) {

        } else if (address >= 0x6000) {
            mapper->Write(address, value);
        } else {
            std::cout << "unhandled cpu memory write at address: " << address << std::endl;
        }
    }

// PPU Memory Map

    char ReadPpuMemory(short address) {
        auto address_div = address % 0x4000;
        if (address_div < 0x2000) {
            return mapper->Read(address_div);
        } else if (address < 0x3F00) {
            auto mode = ppu->cartridge->mirror;
            return ppu->nameTableData[MirrorAddress(mode, address_div) % 2048];
        } else if (address < 0x4000) {
            return ppu->readPalette(address_div % 32);
        } else {
            std::cout << "unhandled ppu memory read at address: " << address << std::endl;
        }
        return 0;
    }

    void WritePpuMemory(short address, char value) {
        auto address_div = address % 0x4000;

        if (address_div < 0x2000) {
            mapper->Write(address_div, value);
        } else if (address_div < 0x3F00) {
            auto mode = ppu->cartridge->mirror;
            ppu->nameTableData[MirrorAddress(mode, address_div) % 2048] = value;
        } else if (address_div < 0x4000) {
            ppu->writePalette(address_div % 32, value);
        } else {
            std::cout << "unhandled ppu memory write at address: " << address << std::endl;
        }
    }

// Mirroring Modes

    enum {
        MirrorHorizontal,
        MirrorVertical,
        MirrorSingle0,
        MirrorSingle1,
        MirrorFour
    };

    char MirrorLookup[5][4] = {
            {0, 0, 1, 1},
            {0, 1, 0, 1},
            {0, 0, 0, 0},
            {1, 1, 1, 1},
            {0, 1, 2, 3},
    };

    short MirrorAddress(char mode, short address) {
        auto address_div = (address - 0x2000) % 0x1000;
        auto table = address_div / 0x0400;
        auto offset = address_div % 0x0400;
        return 0x2000 + MirrorLookup[mode][table] * 0x0400 + offset;
    }

};


#endif //CPPTEST_RAM_H
