#ifndef NES_EMU_INES_H
#define NES_EMU_INES_H
#include <cstdint>
#include <string>

#include "Cartridge.h"

constexpr int iNESFileMagic = 0x1a53454e;

struct iNESFileHeader {
    uint32_t Magic;    // iNES magic number
    uint8_t NumPRG;    // number of PRG-ROM banks (16KB each)
    uint8_t NumCHR;    // number of CHR-ROM banks (8KB each)
    uint8_t Control1;  // control bits
    uint8_t Control2;  // control bits
    uint8_t NumRAM;    // PRG-RAM size (x 8KB)
    uint8_t unused[7]; // unused padding
};

// LoadNESFile reads an iNES file (.nes) and returns a Cartridge on success.
// http://wiki.nesdev.com/w/index.php/INES
// http://nesdev.com/NESDoc.pdf (page 28)
Cartridge* LoadNESFile(const std::string& path) {
    // open file
    std::ifstream file{path};
    if (!file.is_open()) {
        throw std::runtime_error{"Unable to open file"};
    }


    // read file header
    header := iNESFileHeader{}
    if err := binary.Read(file, binary.LittleEndian, &header); err != nil {
        return nil, err
    }

    // verify header magic number
    if header.Magic != iNESFileMagic {
        return nil, errors.New("invalid .nes file")
    }

    // mapper type
    mapper1 := header.Control1 >> 4
    mapper2 := header.Control2 >> 4
    mapper := mapper1 | mapper2<<4

    // mirroring type
    mirror1 := header.Control1 & 1
    mirror2 := (header.Control1 >> 3) & 1
    mirror := mirror1 | mirror2<<1

    // battery-backed RAM
    battery := (header.Control1 >> 1) & 1

    // read trainer if present (unused)
    if header.Control1&4 == 4 {
        trainer := make([]byte, 512)
        if _, err := io.ReadFull(file, trainer); err != nil {
            return nil, err
        }
    }

    // read prg-rom bank(s)
    prg := make([]byte, int(header.NumPRG)*16384)
    if _, err := io.ReadFull(file, prg); err != nil {
        return nil, err
    }

    // read chr-rom bank(s)
    chr := make([]byte, int(header.NumCHR)*8192)
    if _, err := io.ReadFull(file, chr); err != nil {
        return nil, err
    }

    // provide chr-rom/ram if not in file
    if header.NumCHR == 0 {
        chr = make([]byte, 8192)
    }

    // success
    return NewCartridge(prg, chr, mapper, mirror, battery), nil
}


#endif //NES_EMU_INES_H
