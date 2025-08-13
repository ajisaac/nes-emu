#include "Cartridge.h"

#include <iostream>

/**
 * The beginning of every ines file format
 */
const int MAGIC = 0x1a53454e;

/**
 * The first 16 bytes
 */

Cartridge::Cartridge(const std::string &path) {
    std::cout << path << std::endl;

    // open file
    std::ifstream file;
    file.open(path);
    if (!file.is_open()) {
        exit(1);
    }

    std::vector<char> bytes((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());

    // read file header
    // Magic number First 4 bytes
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
    char numRam = bytes[8]; // x 8KB

    // unused
    char flags9 = bytes[9];
    char flags10 = bytes[10];

    // mapper type
    int mapper1 = control1 >> 4;
    int mapper2 = control2 >> 4;
    mapper = mapper1 | mapper2 << 4;

    // mirroring type
    int mirror1 = control1 & 1;
    int mirror2 = (control1 >> 3) & 1;
    mirror = mirror1 | mirror2 << 1;

    // battery-backed RAM
    battery = (control1 >> 1) & 1;

    // read trainer if present (unused)
    if ((control1 & 4) == 4) {
        std::vector<char> trainer{};
        for (int i = 0; i < 512; i++) {
            trainer[i] = bytes[i];
        }
    }

    // read prg-rom bank(s)
    prg.resize(numPrg * 16384);
    for (int i = 0; i < numPrg * 16384; i++) {
        prg[i] = bytes[i];
    }

    // read chr-rom bank(s)
    chr.resize(numChr * 8192);
    for (int i = 0; i < numChr * 8192; i++) {
        chr[i] = bytes[i];
    }

    std::cout << "get out of my house " << std::endl;
}
