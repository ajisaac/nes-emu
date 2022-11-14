#include "../headers/Console.h"

Console::Console() = default;

void Console::run() {

    // load rom
    // start running
    cpu.step();
}

Console::~Console() = default;
