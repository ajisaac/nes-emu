#include <iostream>
#include "../headers/Cpu.h"
#include "../headers/Ram.h"

Cpu::Cpu(Ram *ram) {
    this->ram = ram;
}