#ifndef CPPTEST_CPU_H
#define CPPTEST_CPU_H

#include <bitset>
#include "Ram.h"


class Cpu {

private:

    // special
    // The 2-byte program counter PC supports 65536 direct (unbanked) memory locations,  however not all values are sent
    // to the cartridge. It can be accessed either by  allowing CPU's internal fetch logic increment the address bus, an
    // interrupt (NMI, Reset, IRQ/BRQ), and using the RTS/JMP/JSR/Branch instructions.
    std::bitset<16> program_counter{0x0}; // $PC
    std::bitset<8> status_register{0x0};  // $SR
    std::bitset<8> stack_pointer{0x0};    // $SP

    // general
    // A is byte-wide and along with the arithmetic logic unit (ALU), supports using the status register for carrying,
    // overflow detection, and so on.
    std::bitset<8> accumulator{0x0};      // $A
    std::bitset<8> x_register{0x0};       // $X
    std::bitset<8> y_register{0x0};       // $Y

    Ram *ram;

    enum class instruction;

    static instruction decode();

    enum class addressing_mode;

public:
    explicit Cpu(Ram *ram);

    void step();
};


#endif //CPPTEST_CPU_H
