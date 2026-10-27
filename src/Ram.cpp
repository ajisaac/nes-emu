#include "Ram.h"

#include "Console.h"

// indexed by Cartridge::mirror, then by which of the four nametables is addressed
static constexpr uint16_t MirrorLookup[5][4] = {
    {0, 0, 1, 1},
    {0, 1, 0, 1},
    {0, 0, 0, 0},
    {1, 1, 1, 1},
    {0, 1, 2, 3},
};

Ram::Ram(Console *console) : console{console} {
}

uint8_t Ram::read(const uint16_t address) const {
    if (address < 0x2000) {
        return RAM[address % 0x0800];
    }
    if (address < 0x4000) {
        return console->ppu->readRegister(0x2000 + address % 8);
    }
    if (address == 0x4014) {
        return console->ppu->readRegister(address);
    }
    if (address == 0x4015) {
        return console->apu->readRegister(address);
    }
    if (address == 0x4016) {
        return console->controller1->read();
    }
    if (address == 0x4017) {
        return console->controller2->read();
    }
    if (address < 0x6000) {
        // TODO: I/O registers
        return 0;
    }
    return console->mapper->read(address);
}

void Ram::write(const uint16_t address, const uint8_t value) {
    if (address < 0x2000) {
        RAM[address % 0x0800] = value;
    } else if (address < 0x4000) {
        console->ppu->writeRegister(0x2000 + address % 8, value);
    } else if (address < 0x4014) {
        console->apu->writeRegister(address, value);
    } else if (address == 0x4014) {
        console->ppu->writeRegister(address, value);
    } else if (address == 0x4015) {
        console->apu->writeRegister(address, value);
    } else if (address == 0x4016) {
        console->controller1->write(value);
        console->controller2->write(value);
    } else if (address == 0x4017) {
        console->apu->writeRegister(address, value);
    } else if (address < 0x6000) {
        // TODO: I/O registers
    } else {
        console->mapper->write(address, value);
    }
}

uint8_t Ram::ReadPpuMemory(uint16_t address) const {
    address = address % 0x4000;
    if (address < 0x2000) {
        return console->mapper->read(address);
    }
    if (address < 0x3F00) {
        const uint8_t mode = console->cartridge->mirror;
        return console->ppu->nameTableData[MirrorAddress(mode, address) % 2048];
    }
    return console->ppu->readPalette(address % 32);
}

void Ram::WritePpuMemory(uint16_t address, const uint8_t value) const {
    address = address % 0x4000;
    if (address < 0x2000) {
        console->mapper->write(address, value);
    } else if (address < 0x3F00) {
        const uint8_t mode = console->cartridge->mirror;
        console->ppu->nameTableData[MirrorAddress(mode, address) % 2048] = value;
    } else {
        console->ppu->writePalette(address % 32, value);
    }
}

uint16_t Ram::MirrorAddress(const uint8_t mode, uint16_t address) {
    // Go subtracts in uint16, so this wraps instead of going negative
    address = static_cast<uint16_t>(address - 0x2000) % 0x1000;
    const uint16_t table = address / 0x0400;
    const uint16_t offset = address % 0x0400;
    return 0x2000 + MirrorLookup[mode][table] * 0x0400 + offset;
}
