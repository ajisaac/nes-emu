//
// Created by Aaron Isaac on 5/20/24.
//

#include "INes.h"


void INes::loadCartridge(std::string nesFile) {

    // open file
//    Path path = Path.of(nesFile);
//    unsigned char [] bytes = Files.readAllBytes(path);

    // read file header
//    this.header = Arrays.copyOfRange(bytes, 0, 16);

    verifyHeader();

    // num of PRG-ROM in 16kb chunks
    unsigned char numPrg = header[4];

    // num of CHR-ROM in 8kb chunks, 0 means board uses CHR-RAM
    unsigned char numChr = header[5];

    unsigned char flags6 = header[6];
    unsigned char flags7 = header[7];
    unsigned char flags8 = header[8];
    unsigned char flags9 = header[9];
    unsigned char flags10 = header[10];


    // load trainer if present

    // load PRG ROM

    // LOAD CHR ROM

    // PlayChoice INST-ROM if present

    // PlayChoice PROM, if present


    // read in all the bytes from the file
}

// verify magic number, first 4 bytes
void INes::verifyHeader() {
    int i = header[0] | header[1] << 8 | header[2] << 16 | header[3] << 24;
//    if (i != MAGIC) throw new IllegalStateException("Magic number unexpected: 0x" + HexFormat.of().toHexDigits(i));
}

// FLAGS 6

// 0 == vertical arrangement / "horizontal mirrored" (CIRAM a10 = PPU A11)
// 1 == horizontal arrangement / "veritcal mirrored" (CIRAM a10 = PPU A10)
int INes::nametableArrangement() {
    return header[6] | 0b00000001;
}


int INes::hasBatteryPrgRam() {
    return header[6] | 0b00000010;
}

int INes::hasTrainerData() {
    return header[6] | 0b00000100;
}

int INes::hasAlternativeNametableLayout() {
    return header[6] | 0b00001000;
}

// first 4 bits of 6 and 7
// 6's is low nibble, 7's is high nibble
unsigned char INes::getMapper() {
    return ((header[6] & 0b11110000) | ((header[7] & 0b11110000) >> 4));
}

// FLAGS 7

int INes::hasVsUnisystem() {
    return header[7] | 0b00000001;
}

int INes::hasPlayChoice10() {
    return header[7] | 0b00000010;
}

// if equal to 2, then yes
int INes::hasNes2format() {
    return header[7] | 0b00001100;
}

// FLAGS 8

int INes::prgRamSize() {
    return header[8];
}

// 0:NTSC or 1:PAL
int INes::tvSystem() {
    return header[9] | 0b00000001;
}

// reserved, set to 0
int INes::f9Reserved() {
    int i = header[9] & 0b11111110;
//    if (i != 0) throw new IllegalStateException("f9 bits should be 0");
    return i;
}

// FLAGS 10

// 0: NTSC, 2: PAL, 1/3: Dual Compatible
int INes::f10tvSystem() {
    return header[10] | 0b00000011;
}

// 0: present, 1: not present
int INes::f10prgRam() {
    return header[10] | 0b00010000;
}

// 0: no conflicts, 1: conflicts
int INes::f10busConflicts() {
    return header[10] | 0b00100000;
}
