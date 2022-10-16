#include "../headers/Console.h"

Console::Console() = default;

void Console::run() {
    cpu.step();
    cpu.step();
    cpu.step();
    cpu.step();
    cpu.step();
    cpu.step();
}

Console::~Console() = default;
