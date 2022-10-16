#include <iostream>
#include "../headers/Cpu.h"
#include "../headers/Ram.h"

Cpu::Cpu(Ram *ram) {
    this->ram = ram;
}

enum class Cpu::instruction {

};

enum class Cpu::addressing_mode {
    INDEXED_D_X,      // val = PEEK((arg + X) % 256)
    INDEXED_D_Y,      // val = PEEK((arg + Y) % 256)
    INDEXED_A_X,      // val = PEEK(arg + X)
    INDEXED_A_Y,      // val = PEEK(arg + Y)
    INDEXED_INDIRECT, // val = PEEK(PEEK((arg + X) % 256) + PEEK((arg + X + 1) % 256) * 256)
    INDIRECT_INDEXED, // val = PEEK(PEEK(arg) + PEEK((arg + 1) % 256) * 256 + Y)
};

// will overflow
template<size_t N>
static void increment(std::bitset<N> &bitset) {
    for (size_t s = 0; s < N; s++) {
        if (bitset[s] == 0) {
            bitset[s] = 1;
            break;
        }
        bitset[s] = 0;
    }
    std::cout << bitset << std::endl;

}

void Cpu::step() {
    // step 1
    // set $pc to memory location of instruction
    // step 2
    // check if we reach special end condition, if so then end
    // step 3
    // decode CPU instruction at $pc
    auto inst = decode();
    // step 4
    // set $pc to next instruction
    increment(program_counter);
    // step 5
    // fetch data per memory access mode
    // step 6
    // execute instruction with memory fetched
    // step 7
    // goto step 2

}

Cpu::instruction Cpu::decode() {
    Cpu::instruction result;
    return result;
}
