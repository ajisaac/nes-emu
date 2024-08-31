//
// Created by Aaron Isaac on 5/20/24.
//

#ifndef NES_EMU_INES_H
#define NES_EMU_INES_H


#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include "Cartridge.h"

class INes {
/*
 Header (16 bytes)
 Trainer, if present (0 or 512 bytes)
 PRG ROM data (16384 * x bytes)
 CHR ROM data, if present (8192 * y bytes)
 PlayChoice INST-ROM, if present (0 or 8192 bytes)
 PlayChoice PROM, if present (16 bytes Data, 16 bytes CounterOut) (this is often missing; see PC10 ROM-Images for details)
 */

    /**
     * The beginning of every ines file format
     */
    const int MAGIC = 0x1a53454e;

    /**
     * The first 16 bytes
     */

public:
    Cartridge loadCartridge(const std::string &nesFile) const {

        // open file
        std::ifstream file;
        file.open(nesFile);
        if (!file.is_open()) {
            exit(1);
        }

        std::vector<char> bytes((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        // read file header
        // verify magic number, first 4 bytes
        int c = bytes[0] | bytes[1] << 8 | bytes[2] << 16 | bytes[3] << 24;
        if (c != MAGIC) {
            std::cout << "Invalid iNES file" << std::endl;
            exit(1);
        }

        // num of PRG-ROM in 16kb chunks
        char numPrg = bytes[4];

        // num of CHR-ROM banks in 8kb chunks, 0 means board uses CHR-RAM
        char numChr = bytes[5];

        char control1 = bytes[6];
        char control2 = bytes[7];

        char numRam = bytes[8];

        // unused
        char flags9 = bytes[9];
        char flags10 = bytes[10];

        // mapper type
        auto mapper1 = control1 >> 4;
        auto mapper2 = control2 >> 4;
        auto mapper = mapper1 | mapper2 << 4;

        // mirroring type
        auto mirror1 = control1 & 1;
        auto mirror2 = (control1 >> 3) & 1;
        auto mirror = mirror1 | mirror2 << 1;

        // battery-backed RAM
        auto battery = (control1 >> 1) & 1;

        // read trainer if present (unused)
        if ((control1 & 4) == 4) {
            std::vector<char> trainer{};
            for (int i = 0; i < 512; i++) {
                trainer[i] = char(bytes[i]);
            }
        }

        // read prg-rom bank(s)
        std::vector<char> prg(numPrg * 16384);
        for (int i = 0; i < numPrg * 16384; i++) {
            prg[i] = bytes[i];
        }

        // read chr-rom bank(s)
        std::vector<char> chr(numChr * 8192);
        for (int i = 0; i < numChr * 8192; i++) {
            chr[i] = bytes[i];
        }

        return {prg, chr, mapper, mirror, battery};
    }




    // FLAGS 6

    // 0 == vertical arrangement / "horizontal mirrored" (CIRAM a10 = PPU A11)
    // 1 == horizontal arrangement / "veritcal mirrored" (CIRAM a10 = PPU A10)
//    int nametableArrangement() {
//        return header[6] | 0b00000001;
//    }
//
//    int hasBatteryPrgRam() {
//        return header[6] | 0b00000010;
//    }
//
//    int hasTrainerData() {
//        return header[6] | 0b00000100;
//    }
//
//    int hasAlternativeNametableLayout() {
//        return header[6] | 0b00001000;
//    }
//
//    // first 4 bits of 6 and 7
//    // 6's is low nibble, 7's is high nibble
//    std::byte getMapper() {
//        return (byte)((header[6] & 0b11110000) | ((header[7] & 0b11110000) >> 4));
//    }
//
//    // FLAGS 7
//    int hasVsUnisystem() {
//        return header[7] | 0b00000001;
//    }
//
//    int hasPlayChoice10() {
//        return header[7] | 0b00000010;
//    }
//
//    // if equal to 2, then yes
//    int hasNes2format() {
//        return header[7] | 0b00001100;
//    }
//
//    // FLAGS 8
//
//    int prgRamSize() {
//        return header[8];
//    }
//
//    // 0:NTSC or 1:PAL
//    int tvSystem() {
//        return header[9] | 0b00000001;
//    }
//
//    // reserved, set to 0
//    int f9Reserved() {
//        int i = header[9] & 0b11111110;
//        if (i != 0) throw new IllegalStateException("f9 bits should be 0");
//        return i;
//    }
//
//    // FLAGS 10
//
//    // 0: NTSC, 2: PAL, 1/3: Dual Compatible
//    int f10tvSystem() {
//        return header[10] | 0b00000011;
//    }
//
//    // 0: present, 1: not present
//    int f10prgRam() {
//        return header[10] | 0b00010000;
//    }
//
//    // 0: no conflicts, 1: conflicts
//    int f10busConflicts() {
//        return header[10] | 0b00100000;
//    }

//}

};

#endif //NES_EMU_INES_H
