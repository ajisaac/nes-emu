#include <bitset>
#include <cstddef>

#include "Emulator.h"

// todo move this elsewhere
std::ostream& operator<<(std::ostream& os, std::byte b) {
    return os << std::bitset<8>(std::to_integer<int>(b));
}

int main() {
    const Emulator emulator{"/Users/aaron/Code/personal/nes-emu/data/Nintendo/Castlevania.nes"};
    emulator.run();

    return 0;
}
