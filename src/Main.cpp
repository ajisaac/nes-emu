#include "Emulator.h"

int main() {
    std::string rom = "/Users/aaron/Code/nes-emu/data/Nintendo/1942.nes";
    Emulator emulator{rom};
    emulator.run();
    return 0;
}
