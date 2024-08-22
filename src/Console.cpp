#include "Console.h"

Console::Console() = default;

void Console::run() {
    // load rom
    // start running
    while (true)
        cpu.step();
}

Console::~Console() = default;
//
// Created by Aaron Isaac on 5/20/24.
//

#ifndef NES_EMU_CARTRIDGE_H
#define NES_EMU_CARTRIDGE_H


class Cartridge {

};


#endif //NES_EMU_CARTRIDGE_H
package co.aisaac;

// Cartridge loaded from a .nes file
// http://wiki.nesdev.com/w/index.php/INES
// http://nesdev.com/NESDoc.pdf (page 28)
public class Cartridge {

    // PRG-ROM
    byte[] prg;

    // CHR-ROM
    byte[] chr;

    // mapper type for this game
    byte mapper;

    // mirroring mode?
    byte mirror;

    // does battery exist?
    byte battery;

public Cartridge(String rom) {
        // check if file exists
        // check if headers are proper
        // start loading the cartridge
        // throw ise if fails


    }
}
