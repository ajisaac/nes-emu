#include "Mapper7.h"

#include <format>
#include <stdexcept>

#include "Cartridge.h"

Mapper7::Mapper7(Cartridge *cartridge) : cartridge{cartridge} {
}

uint8_t Mapper7::read(const uint16_t address) {
    if (address < 0x2000) {
        return cartridge->chr[address];
    }
    if (address >= 0x8000) {
        const int index = prgBank * 0x8000 + (address - 0x8000);
        return cartridge->prg[index];
    }
    if (address >= 0x6000) {
        const int index = address - 0x6000;
        return cartridge->sram[index];
    }
    throw std::runtime_error{std::format("unhandled mapper7 read at address: 0x{:04X}", address)};
}

void Mapper7::write(const uint16_t address, const uint8_t value) {
    if (address < 0x2000) {
        cartridge->chr[address] = value;
    } else if (address >= 0x8000) {
        prgBank = value & 7;
        switch (value & 0x10) {
            case 0x00:
                cartridge->mirror = MirrorSingle0;
                break;
            case 0x10:
                cartridge->mirror = MirrorSingle1;
                break;
        }
    } else if (address >= 0x6000) {
        const int index = address - 0x6000;
        cartridge->sram[index] = value;
    } else {
        throw std::runtime_error{std::format("unhandled mapper7 write at address: 0x{:04X}", address)};
    }
}
