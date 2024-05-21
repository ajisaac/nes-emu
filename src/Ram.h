#ifndef CPPTEST_RAM_H
#define CPPTEST_RAM_H

#include <cstddef>

class Ram {

    /* this is all our memory mapping information */
    std::byte total_ram[0xFFFF]{std::byte{0}};

    /* first 2kb are internal ram */
    std::byte *zero_page = &total_ram[0];
    int zero_page_len = 0x100;
    std::byte *cpu_stack = &total_ram[0x100];
    int cpu_stack_len = 0x100;
    std::byte *cpu_ram = &total_ram[0x200];
    int cpu_ram_len = 0x600;

    /* then 3 x 2kb mirrors of first 2kb */
    int mirror_len = 0x800; // 2 kb each
    std::byte *mirror1 = &total_ram[0];
    std::byte *mirror2 = &total_ram[0];
    std::byte *mirror3 = &total_ram[0];

    /* 8 bytes for PPU registers */
    std::byte *ppu_reg = &total_ram[0x2000];

    /* 8kb - 8b for mirrors of PPU reg */
    std::byte *ppu_reg_mirror = &total_ram[0x2000]; // todo make 8k of mirrors, or don't

    /* 24b for APU, 8b disabled, $4000 to 401F, 32b total*/
    std::byte *apu = &total_ram[0x4000]; // this goes to 0x4020

    /* now we hit the cartridge, it goes from 0x4020 to 0x8000, 32kb - 32b*/
    std::byte *cartridge = &total_ram[0x4020];
};


#endif //CPPTEST_RAM_H
package co.aisaac;

import static co.aisaac.Utils.ub;

public class Ram {
    /*
    We don't need buses, everything deals with ram

    NES supports 16 bit addressing, up to 64kb of ram

    0x0000 to 0x00FF -> 256 bytes of zero-page ram
    */

    byte[] ram = new byte[2048];

    byte read(short address) {
        return ram[address];
    }

    void write(short address, int value) {
        if (address <= 0x00FF) {
            // 0000 -> 00FF
            // zero page

        } else if (address <= 0x01FF) {
            // 0100 -> 01FF
            // stack
            /*
            The stack is located at memory locations $0100-$01FF. The stack pointer is an 8-bit register
            which serves as an offset from $0100. The stack works top-down, so when a byte is pushed
            on to the stack, the stack pointer is decremented and when a byte is pulled from the stack,
            the stack pointer is incremented. There is no detection of stack overflow and the stack
            pointer will just wrap around from $00 to $FF.
             */

        } else if (address <= 0x07FF) {
            // 0200 -> 07FF
            // ram
            /*
            $0000-$07FF are mirrored three times at $0800-$1FFF. This means that, for example, any
            data written to $0000 will also be written to $0800, $1000 and $1800
             */

        } else if (address <= 0x1FFF) {
            // 0800 -> 1FFF
            // mirrors of range 0x0000 to 0x07FF

        } else if (address <= 0x4000) {
            // 2000 -> 2007
            // io registers
            /* The memory mapped
            I/O registers are located at $2000-$401F. Locations $2000-$2007 are mirrored every 8 bytes
            in the region $2008-$3FFF and the remaining registers follow this mirroring.
             */

            // 2008 -> 3FFF
            // mirrors of io registers

        } else if (address <= 0x401F) {
            // 4000 -> 401F
            // io registers and mirrors

        } else if (address <= 0x5FFF) {
            // 4020 -> 5FFF
            // expansion rom

        } else if (address <= 0x7FFF) {
            // 6000 -> 7FFF
            /*
            SRAM (WRAM) is the Save RAM, the addresses used to access RAM in the cartridges for storing save games.
             */

        } else if (address <= 0xBFFF) {
            // 8000 -> BFFF
            // PRG-ROM lower bank

        } else if (address <= 0xFFFF) {
            // C000 -> FFFF
            // PRG-ROM upper bank

        } else {
            return;
            // 0xFFFF
            // impossible

        }
        write(address, (byte) value);
    }

    void write(short address, byte value) {
        ram[address] = value;
    }

}

