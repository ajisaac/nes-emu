#ifndef CPPTEST_CPU_H
#define CPPTEST_CPU_H

#include <bitset>
#include "Ram.h"

class Cpu {

private:
    // special
    std::bitset<8> program_counter;
    std::bitset<8> status_register;
    std::bitset<8> stack_pointer;

    // general
    std::bitset<8> accumulator;
    std::bitset<8> x_register;
    std::bitset<8> y_register;

    Ram *ram;

public:
    explicit Cpu(Ram *ram);
};


#endif //CPPTEST_CPU_H
