#include "ines.h"

#include <bit>
#include <fstream>
#include <stdexcept>
#include <utility>
#include <vector>

#include "Cartridge.h"

// the header is read straight from the file, so the struct has to match the
// 16 byte little-endian layout on disk
static_assert(sizeof(iNESFileHeader) == 16);
static_assert(std::endian::native == std::endian::little);

namespace {
    // readFull reads exactly buffer.size() bytes, like Go's io.ReadFull
    void readFull(std::ifstream &file, std::vector<uint8_t> &buffer) {
        file.read(reinterpret_cast<char *>(buffer.data()), static_cast<std::streamsize>(buffer.size()));
        if (!file) {
            throw std::runtime_error{"unexpected end of .nes file"};
        }
    }
}

Cartridge *LoadNESFile(const std::string &path) {
    // open file
    std::ifstream file{path, std::ios::binary};
    if (!file.is_open()) {
        throw std::runtime_error{"unable to open file: " + path};
    }

    // read file header
    iNESFileHeader header{};
    file.read(reinterpret_cast<char *>(&header), sizeof(header));
    if (!file) {
        throw std::runtime_error{"unexpected end of .nes file"};
    }

    // verify header magic number
    if (header.Magic != iNESFileMagic) {
        throw std::runtime_error{"invalid .nes file"};
    }

    // mapper type
    const uint8_t mapper1 = header.Control1 >> 4;
    const uint8_t mapper2 = header.Control2 >> 4;
    const uint8_t mapper = mapper1 | (mapper2 << 4);

    // mirroring type
    const uint8_t mirror1 = header.Control1 & 1;
    const uint8_t mirror2 = (header.Control1 >> 3) & 1;
    const uint8_t mirror = mirror1 | (mirror2 << 1);

    // battery-backed RAM
    const uint8_t battery = (header.Control1 >> 1) & 1;

    // read trainer if present (unused)
    if ((header.Control1 & 4) == 4) {
        std::vector<uint8_t> trainer(512);
        readFull(file, trainer);
    }

    // read prg-rom bank(s)
    std::vector<uint8_t> prg(header.NumPRG * 16384);
    readFull(file, prg);

    // read chr-rom bank(s)
    std::vector<uint8_t> chr(header.NumCHR * 8192);
    readFull(file, chr);

    // provide chr-rom/ram if not in file
    if (header.NumCHR == 0) {
        chr.resize(8192);
    }

    // success
    return new Cartridge(std::move(prg), std::move(chr), mapper, mirror, battery);
}
