#include "Mapper3.h"

#include <format>
#include <stdexcept>

#include "Cartridge.h"

Mapper3::Mapper3(Cartridge *cartridge) : cartridge{cartridge} {
    const int prgBanks = static_cast<int>(cartridge->prg.size() / 0x4000);
    chrBank = 0;
    prgBank1 = 0;
    prgBank2 = prgBanks - 1;
}

uint8_t Mapper3::read(const uint16_t address) {
    if (address < 0x2000) {
        const int index = chrBank * 0x2000 + address;
        return cartridge->chr[index];
    }
    if (address >= 0xC000) {
        const int index = prgBank2 * 0x4000 + (address - 0xC000);
        return cartridge->prg[index];
    }
    if (address >= 0x8000) {
        const int index = prgBank1 * 0x4000 + (address - 0x8000);
        return cartridge->prg[index];
    }
    if (address >= 0x6000) {
        const int index = address - 0x6000;
        return cartridge->sram[index];
    }
    throw std::runtime_error{std::format("unhandled mapper3 read at address: 0x{:04X}", address)};
}

void Mapper3::write(const uint16_t address, const uint8_t value) {
    if (address < 0x2000) {
        const int index = chrBank * 0x2000 + address;
        cartridge->chr[index] = value;
    } else if (address >= 0x8000) {
        chrBank = value & 3;
    } else if (address >= 0x6000) {
        const int index = address - 0x6000;
        cartridge->sram[index] = value;
    } else {
        throw std::runtime_error{std::format("unhandled mapper3 write at address: 0x{:04X}", address)};
    }
}
