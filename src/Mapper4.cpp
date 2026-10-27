#include "Mapper4.h"

#include <format>
#include <stdexcept>

#include "Console.h"

Mapper4::Mapper4(Console *console, Cartridge *cartridge) : cartridge{cartridge}, console{console} {
    prgOffsets[0] = prgBankOffset(0);
    prgOffsets[1] = prgBankOffset(1);
    prgOffsets[2] = prgBankOffset(-2);
    prgOffsets[3] = prgBankOffset(-1);
}

void Mapper4::step() {
    const PPU *ppu = console->ppu;
    if (ppu->cycle != 280) { // TODO: this *should* be 260
        return;
    }
    if (ppu->scanline > 239 && ppu->scanline < 261) {
        return;
    }
    if (ppu->flagShowBackground == 0 && ppu->flagShowSprites == 0) {
        return;
    }
    HandleScanLine();
}

void Mapper4::HandleScanLine() {
    if (counter == 0) {
        counter = reload;
    } else {
        counter--;
        if (counter == 0 && irqEnable) {
            console->cpu->triggerIRQ();
        }
    }
}

uint8_t Mapper4::read(uint16_t address) {
    if (address < 0x2000) {
        const int bank = address / 0x0400;
        const int offset = address % 0x0400;
        return cartridge->chr[chrOffsets[bank] + offset];
    }
    if (address >= 0x8000) {
        address = address - 0x8000;
        const int bank = address / 0x2000;
        const int offset = address % 0x2000;
        return cartridge->prg[prgOffsets[bank] + offset];
    }
    if (address >= 0x6000) {
        return cartridge->sram[address - 0x6000];
    }
    throw std::runtime_error{std::format("unhandled mapper4 read at address: 0x{:04X}", address)};
}

void Mapper4::write(const uint16_t address, const uint8_t value) {
    if (address < 0x2000) {
        const int bank = address / 0x0400;
        const int offset = address % 0x0400;
        cartridge->chr[chrOffsets[bank] + offset] = value;
    } else if (address >= 0x8000) {
        writeRegister(address, value);
    } else if (address >= 0x6000) {
        cartridge->sram[address - 0x6000] = value;
    } else {
        throw std::runtime_error{std::format("unhandled mapper4 write at address: 0x{:04X}", address)};
    }
}

void Mapper4::writeRegister(const uint16_t address, const uint8_t value) {
    if (address <= 0x9FFF && address % 2 == 0) {
        writeBankSelect(value);
    } else if (address <= 0x9FFF && address % 2 == 1) {
        writeBankData(value);
    } else if (address <= 0xBFFF && address % 2 == 0) {
        writeMirror(value);
    } else if (address <= 0xBFFF && address % 2 == 1) {
        writeProtect(value);
    } else if (address <= 0xDFFF && address % 2 == 0) {
        writeIRQLatch(value);
    } else if (address <= 0xDFFF && address % 2 == 1) {
        writeIRQReload(value);
    } else if (address % 2 == 0) {
        writeIRQDisable(value);
    } else {
        writeIRQEnable(value);
    }
}

void Mapper4::writeBankSelect(const uint8_t value) {
    prgMode = (value >> 6) & 1;
    chrMode = (value >> 7) & 1;
    registerIndex = value & 7;
    updateOffsets();
}

void Mapper4::writeBankData(const uint8_t value) {
    registers[registerIndex] = value;
    updateOffsets();
}

void Mapper4::writeMirror(const uint8_t value) {
    switch (value & 1) {
        case 0:
            cartridge->mirror = MirrorVertical;
            break;
        case 1:
            cartridge->mirror = MirrorHorizontal;
            break;
    }
}

void Mapper4::writeProtect(const uint8_t value) {
}

void Mapper4::writeIRQLatch(const uint8_t value) {
    reload = value;
}

void Mapper4::writeIRQReload(const uint8_t value) {
    counter = 0;
}

void Mapper4::writeIRQDisable(const uint8_t value) {
    irqEnable = false;
}

void Mapper4::writeIRQEnable(const uint8_t value) {
    irqEnable = true;
}

// the sizes are cast to int because index can be negative, which would turn
// into a huge number if it were mixed with the vector's unsigned size
int Mapper4::prgBankOffset(int index) const {
    const int size = static_cast<int>(cartridge->prg.size());
    if (index >= 0x80) {
        index -= 0x100;
    }
    index %= size / 0x2000;
    int offset = index * 0x2000;
    if (offset < 0) {
        offset += size;
    }
    return offset;
}

int Mapper4::chrBankOffset(int index) const {
    const int size = static_cast<int>(cartridge->chr.size());
    if (index >= 0x80) {
        index -= 0x100;
    }
    index %= size / 0x0400;
    int offset = index * 0x0400;
    if (offset < 0) {
        offset += size;
    }
    return offset;
}

void Mapper4::updateOffsets() {
    switch (prgMode) {
        case 0:
            prgOffsets[0] = prgBankOffset(registers[6]);
            prgOffsets[1] = prgBankOffset(registers[7]);
            prgOffsets[2] = prgBankOffset(-2);
            prgOffsets[3] = prgBankOffset(-1);
            break;
        case 1:
            prgOffsets[0] = prgBankOffset(-2);
            prgOffsets[1] = prgBankOffset(registers[7]);
            prgOffsets[2] = prgBankOffset(registers[6]);
            prgOffsets[3] = prgBankOffset(-1);
            break;
    }
    switch (chrMode) {
        case 0:
            chrOffsets[0] = chrBankOffset(registers[0] & 0xFE);
            chrOffsets[1] = chrBankOffset(registers[0] | 0x01);
            chrOffsets[2] = chrBankOffset(registers[1] & 0xFE);
            chrOffsets[3] = chrBankOffset(registers[1] | 0x01);
            chrOffsets[4] = chrBankOffset(registers[2]);
            chrOffsets[5] = chrBankOffset(registers[3]);
            chrOffsets[6] = chrBankOffset(registers[4]);
            chrOffsets[7] = chrBankOffset(registers[5]);
            break;
        case 1:
            chrOffsets[0] = chrBankOffset(registers[2]);
            chrOffsets[1] = chrBankOffset(registers[3]);
            chrOffsets[2] = chrBankOffset(registers[4]);
            chrOffsets[3] = chrBankOffset(registers[5]);
            chrOffsets[4] = chrBankOffset(registers[0] & 0xFE);
            chrOffsets[5] = chrBankOffset(registers[0] | 0x01);
            chrOffsets[6] = chrBankOffset(registers[1] & 0xFE);
            chrOffsets[7] = chrBankOffset(registers[1] | 0x01);
            break;
    }
}
