#include "Mapper225.h"

#include <format>
#include <stdexcept>

#include "Cartridge.h"

Mapper225::Mapper225(Cartridge *cartridge) : cartridge{cartridge} {
    const int prgBanks = static_cast<int>(cartridge->prg.size() / 0x4000);
    chrBank = 0;
    prgBank1 = 0;
    prgBank2 = prgBanks - 1;
}

uint8_t Mapper225::read(const uint16_t address) {
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
    throw std::runtime_error{std::format("unhandled Mapper225 read at address: 0x{:04X}", address)};
}

void Mapper225::write(const uint16_t address, const uint8_t value) {
    if (address < 0x8000) {
        return;
    }

    const int A = address;
    const int bank = (A >> 14) & 1;
    chrBank = (A & 0x3f) | (bank << 6);
    const int prg = ((A >> 6) & 0x3f) | (bank << 6);
    const int mode = (A >> 12) & 1;
    if (mode == 1) {
        prgBank1 = prg;
        prgBank2 = prg;
    } else {
        prgBank1 = prg;
        prgBank2 = prg + 1;
    }
    const int mirr = (A >> 13) & 1;
    if (mirr == 1) {
        cartridge->mirror = MirrorHorizontal;
    } else {
        cartridge->mirror = MirrorVertical;
    }
}
