#include "Ram.h"

#include <cstdint>
#include <print>

#include "APU.h"
#include "Console.h"
#include "Controller.h"
#include "Mapper.h"
#include "PPU.h"

uint8_t ram[2048] = {};

enum {
    MirrorHorizontal,
    MirrorVertical,
    MirrorSingle0,
    MirrorSingle1,
    MirrorFour
};

static constexpr uint16_t MirrorLookup[5][4] = {
    {0, 0, 1, 1},
    {0, 1, 0, 1},
    {0, 0, 0, 0},
    {1, 1, 1, 1},
    {0, 1, 2, 3},
};

uint8_t Ram::read(const uint16_t address) const {
    if (address < 0x2000) {
        return ram[address % 0x0800];
    }
    if (address < 0x4000) {
        return ppu->readRegister(0x2000 + address % 8);
    }
    if (address == 0x4014) {
        return ppu->readRegister(address);
    }
    if (address == 0x4015) {
        return apu->readRegister(address);
    }
    if (address == 0x4016) {
        return controller1->read();
    }
    if (address == 0x4017) {
        return controller2->read();
    }
    if (address < 0x6000) {
        // TODO: I/O registers
        return 0;
    }
    if (address >= 0x6000) {
        return mapper->read(address);
    }
    std::cout << "unhandled cpu memory read at address: " << address << std::endl;
    return 0;
}

void Ram::write(const uint16_t address, const uint8_t value) {
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
        controller1->write(value);
        controller2->write(value);
    } else if (address == 0x4017) {
        apu->writeRegister(address, value);
    } else if (address < 0x6000) {
    } else if (address >= 0x6000) {
        mapper->write(address, value);
    } else {
        std::cout << "unhandled cpu memory write at address: " << address << std::endl;
    }
}

uint8_t Ram::ReadPpuMemory(uint16_t address) {
    address = address % 0x4000;
    if (address < 0x2000) {
        return mapper->read(address);
    }
    if (address < 0x3F00) {
        int mode = console->cartridge.mirror;
        return ppu->nameTableData[MirrorAddress(mode, address) % 2048];
    }
    if (address < 0x4000) {
        return ppu->readPalette(address % 32);
    }
    std::println("unhandled ppu memory read at address: {}", address);
    return 0;
}

void Ram::WritePpuMemory(uint16_t address, const uint8_t value) {
    address = address % 0x4000;

    if (address < 0x2000) {
        mapper->write(address, value);
    } else if (address < 0x3F00) {
        const int mode = console->cartridge.mirror;
        ppu->nameTableData[MirrorAddress(mode, address) % 2048] = value;
    } else if (address < 0x4000) {
        ppu->writePalette(address % 32, value);
    } else {
        std::println("unhandled ppu memory write at address: {}", address);
    }
}

uint16_t Ram::MirrorAddress(uint8_t mode, uint16_t address) {
    const int address_div = (address - 0x2000) % 0x1000;
    const int table = address_div / 0x0400;
    const int offset = address_div % 0x0400;
    return 0x2000 + MirrorLookup[mode][table] * 0x0400 + offset;
}
