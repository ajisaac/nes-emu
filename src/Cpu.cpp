#include <iostream>
#include "../headers/Cpu.h"
#include "../headers/Ram.h"

Cpu::Cpu(ram *ram) {
    this->_ram = ram;
}

void Cpu::print_ram() {
    std::cout << to_integer<int>(this->ram[0]) << " ";
    std::cout << to_integer<int>(this->ram[1]) << " ";
    std::cout << to_integer<int>(this->ram[2]) << " ";
    std::cout << to_integer<int>(this->ram[3]) << " ";
    std::cout << to_integer<int>(this->ram[4]) << " ";
    std::cout << to_integer<int>(this->ram[5]) << " ";
    std::cout << "Cpu ram" << std::endl;
}

void Cpu::change_ram() {
    this->ram[0] = std::byte{1};
    this->ram[2] = std::byte{2};
    this->ram[3] = std::byte{3};
    this->ram[4] = std::byte{4};
    this->ram[5] = std::byte{5};
}
