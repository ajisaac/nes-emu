#ifndef NES_EMU_INES_H
#define NES_EMU_INES_H

#include <iostream>
#include <string>

/*
 Header (16 bytes)
 Trainer, if present (0 or 512 bytes)
 PRG ROM data (16384 * x bytes)
 CHR ROM data, if present (8192 * y bytes)
 PlayChoice INST-ROM, if present (0 or 8192 bytes)
 PlayChoice PROM, if present (16 bytes Data, 16 bytes CounterOut) (this is often missing; see PC10 ROM-Images for details)
 */
class INes {

    void loadCartridge(std::string nesFile);

    // The beginning of every ines file format
    static constexpr int MAGIC = 0x1a53454e;

    // The first 16 bytes
    unsigned char header[16];

    // verify magic number, first 4 bytes
    void verifyHeader();

    // FLAGS 6

    // 0 == vertical arrangement / "horizontal mirrored" (CIRAM a10 = PPU A11)
    // 1 == horizontal arrangement / "veritcal mirrored" (CIRAM a10 = PPU A10)
    int nametableArrangement();

    int hasBatteryPrgRam();

    int hasTrainerData();

    int hasAlternativeNametableLayout();

    // first 4 bits of 6 and 7
    // 6's is low nibble, 7's is high nibble
    unsigned char getMapper();

    // FLAGS 7

    int hasVsUnisystem();

    int hasPlayChoice10();

    // if equal to 2, then yes
    int hasNes2format();

    // FLAGS 8

    int prgRamSize();

    // 0:NTSC or 1:PAL
    int tvSystem();

    // reserved, set to 0
    int f9Reserved();

    // FLAGS 10

    // 0: NTSC, 2: PAL, 1/3: Dual Compatible
    int f10tvSystem();

    // 0: present, 1: not present
    int f10prgRam();

    // 0: no conflicts, 1: conflicts
    int f10busConflicts();

};


#endif //NES_EMU_INES_H