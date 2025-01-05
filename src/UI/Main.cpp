#include "Console.h"
#include "Cartridge.h"
#include "INes.h"
#include <bitset>
#include <cstddef>
#include <iostream>

std::ostream &operator<<(std::ostream &os, std::byte b) {
    return os << std::bitset<8>(std::to_integer<int>(b));
}

int main() {
//    Console console{};
//    console.run();

    INes ines{};
    Cartridge cartridge = ines.loadCartridge("/Users/aaron/Code/personal/nes-emu/data/Nintendo/Castlevania.nes");

    Console console(cartridge);
    console.run();

    // create ram, 2048 bytes
    // create controller 1
    // create controller 2
    // create console
    // create mapper, give it to the console
    // give the console a CPU, PPU, APU


    return 0;
}
