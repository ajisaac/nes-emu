#ifndef NES_EMU_CARTRIDGE_H
#define NES_EMU_CARTRIDGE_H

#include <vector>
#include <fstream>

/*
 Header (16 bytes)
 Trainer, if present (0 or 512 bytes)
 PRG ROM data (16384 * x bytes)
 CHR ROM data, if present (8192 * y bytes)
 PlayChoice INST-ROM, if present (0 or 8192 bytes)
 PlayChoice PROM, if present (16 bytes Data, 16 bytes CounterOut) (this is often missing; see PC10 ROM-Images for details)
 */

class Cartridge {
public:
    std::vector<uint8_t> prg;
    std::vector<uint8_t> chr;
    std::vector<uint8_t> sram;
    uint8_t mapper;
    uint8_t mirror;
    uint8_t battery;

    /**
     * The beginning of every ines file format
     */
    const int MAGIC = 0x1a53454e;

    /**
     * The first 16 bytes
     */

    explicit Cartridge(const std::string &path);
};

#endif //NES_EMU_CARTRIDGE_H
