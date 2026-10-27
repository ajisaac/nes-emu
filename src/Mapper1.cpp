#include "Mapper1.h"

#include <format>
#include <stdexcept>

#include "Cartridge.h"

Mapper1::Mapper1(Cartridge *cartridge) : cartridge{cartridge} {
    shiftRegister = 0x10;
    prgOffsets[1] = prgBankOffset(-1);
}

uint8_t Mapper1::read(uint16_t address) {
    if (address < 0x2000) {
        const int bank = address / 0x1000;
        const int offset = address % 0x1000;
        return cartridge->chr[chrOffsets[bank] + offset];
    }
    if (address >= 0x8000) {
        address = address - 0x8000;
        const int bank = address / 0x4000;
        const int offset = address % 0x4000;
        return cartridge->prg[prgOffsets[bank] + offset];
    }
    if (address >= 0x6000) {
        return cartridge->sram[address - 0x6000];
    }
    throw std::runtime_error{std::format("unhandled mapper1 read at address: 0x{:04X}", address)};
}

void Mapper1::write(const uint16_t address, const uint8_t value) {
    if (address < 0x2000) {
        const int bank = address / 0x1000;
        const int offset = address % 0x1000;
        cartridge->chr[chrOffsets[bank] + offset] = value;
    } else if (address >= 0x8000) {
        loadRegister(address, value);
    } else if (address >= 0x6000) {
        cartridge->sram[address - 0x6000] = value;
    } else {
        throw std::runtime_error{std::format("unhandled mapper1 write at address: 0x{:04X}", address)};
    }
}

void Mapper1::loadRegister(const uint16_t address, const uint8_t value) {
    if ((value & 0x80) == 0x80) {
        shiftRegister = 0x10;
        writeControl(control | 0x0C);
    } else {
        const bool complete = (shiftRegister & 1) == 1;
        shiftRegister >>= 1;
        shiftRegister |= (value & 1) << 4;
        if (complete) {
            writeRegister(address, shiftRegister);
            shiftRegister = 0x10;
        }
    }
}

void Mapper1::writeRegister(const uint16_t address, const uint8_t value) {
    if (address <= 0x9FFF) {
        writeControl(value);
    } else if (address <= 0xBFFF) {
        writeCHRBank0(value);
    } else if (address <= 0xDFFF) {
        writeCHRBank1(value);
    } else {
        writePRGBank(value);
    }
}

// Control (internal, $8000-$9FFF)
void Mapper1::writeControl(const uint8_t value) {
    control = value;
    chrMode = (value >> 4) & 1;
    prgMode = (value >> 2) & 3;
    const uint8_t mirror = value & 3;
    switch (mirror) {
        case 0:
            cartridge->mirror = MirrorSingle0;
            break;
        case 1:
            cartridge->mirror = MirrorSingle1;
            break;
        case 2:
            cartridge->mirror = MirrorVertical;
            break;
        case 3:
            cartridge->mirror = MirrorHorizontal;
            break;
    }
    updateOffsets();
}

// CHR bank 0 (internal, $A000-$BFFF)
void Mapper1::writeCHRBank0(const uint8_t value) {
    chrBank0 = value;
    updateOffsets();
}

// CHR bank 1 (internal, $C000-$DFFF)
void Mapper1::writeCHRBank1(const uint8_t value) {
    chrBank1 = value;
    updateOffsets();
}

// PRG bank (internal, $E000-$FFFF)
void Mapper1::writePRGBank(const uint8_t value) {
    prgBank = value & 0x0F;
    updateOffsets();
}

// the sizes are cast to int because index can be negative, which would turn
// into a huge number if it were mixed with the vector's unsigned size
int Mapper1::prgBankOffset(int index) const {
    const int size = static_cast<int>(cartridge->prg.size());
    if (index >= 0x80) {
        index -= 0x100;
    }
    index %= size / 0x4000;
    int offset = index * 0x4000;
    if (offset < 0) {
        offset += size;
    }
    return offset;
}

int Mapper1::chrBankOffset(int index) const {
    const int size = static_cast<int>(cartridge->chr.size());
    if (index >= 0x80) {
        index -= 0x100;
    }
    index %= size / 0x1000;
    int offset = index * 0x1000;
    if (offset < 0) {
        offset += size;
    }
    return offset;
}

// PRG ROM bank mode (0, 1: switch 32 KB at $8000, ignoring low bit of bank number;
//                    2: fix first bank at $8000 and switch 16 KB bank at $C000;
//                    3: fix last bank at $C000 and switch 16 KB bank at $8000)
// CHR ROM bank mode (0: switch 8 KB at a time; 1: switch two separate 4 KB banks)
void Mapper1::updateOffsets() {
    switch (prgMode) {
        case 0:
        case 1:
            prgOffsets[0] = prgBankOffset(prgBank & 0xFE);
            prgOffsets[1] = prgBankOffset(prgBank | 0x01);
            break;
        case 2:
            prgOffsets[0] = 0;
            prgOffsets[1] = prgBankOffset(prgBank);
            break;
        case 3:
            prgOffsets[0] = prgBankOffset(prgBank);
            prgOffsets[1] = prgBankOffset(-1);
            break;
    }
    switch (chrMode) {
        case 0:
            chrOffsets[0] = chrBankOffset(chrBank0 & 0xFE);
            chrOffsets[1] = chrBankOffset(chrBank0 | 0x01);
            break;
        case 1:
            chrOffsets[0] = chrBankOffset(chrBank0);
            chrOffsets[1] = chrBankOffset(chrBank1);
            break;
    }
}
