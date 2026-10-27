#ifndef NES_EMU_INES_H
#define NES_EMU_INES_H
#include <cstdint>
#include <string>

class Cartridge;

/*
 Header (16 bytes)
 Trainer, if present (0 or 512 bytes)
 PRG ROM data (16384 * x bytes)
 CHR ROM data, if present (8192 * y bytes)
 PlayChoice INST-ROM, if present (0 or 8192 bytes)
 PlayChoice PROM, if present (16 bytes Data, 16 bytes CounterOut) (this is often missing; see PC10 ROM-Images for details)
 */

constexpr uint32_t iNESFileMagic = 0x1a53454e;

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
// Throws std::runtime_error on failure.
// http://wiki.nesdev.com/w/index.php/INES
// http://nesdev.com/NESDoc.pdf (page 28)
Cartridge *LoadNESFile(const std::string &path);

#endif //NES_EMU_INES_H
