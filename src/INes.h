//
// Created by Aaron Isaac on 5/20/24.
//

#ifndef NES_EMU_INES_H
#define NES_EMU_INES_H


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






};

#endif //NES_EMU_INES_H
