#include "Cartridge.h"

#include <utility>

Cartridge::Cartridge(std::vector<uint8_t> prg, std::vector<uint8_t> chr, const uint8_t mapper, const uint8_t mirror,
                     const uint8_t battery)
    : prg{std::move(prg)}, chr{std::move(chr)}, sram(0x2000), mapper{mapper}, mirror{mirror}, battery{battery} {
}
