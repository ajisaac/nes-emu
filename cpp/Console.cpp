#include "Console.h"

Console::Console() = default;

void Console::run() {
    // load rom
    // start running
    while (true)
        cpu.step();
}

Console::~Console() = default;
