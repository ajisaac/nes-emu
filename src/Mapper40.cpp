#include "Mapper40.h"

#include <format>
#include <print>
#include <stdexcept>

#include "Console.h"

Mapper40::Mapper40(Console *console, Cartridge *cartridge) : cartridge{cartridge}, console{console} {
}

void Mapper40::step() {
    if (cycles < 0) {
        return;
    }
    cycles++;
    if (cycles % (4096 * 3) == 0) {
        cycles = 0;
        console->cpu->triggerIRQ();
    }
}

uint8_t Mapper40::read(const uint16_t address) {
    if (address < 0x2000) {
        return cartridge->chr[address];
    }
    if (address >= 0x6000 && address < 0x8000) {
        return cartridge->prg[address - 0x6000 + 0x2000 * 6];
    }
    if (address >= 0x8000 && address < 0xA000) {
        return cartridge->prg[address - 0x8000 + 0x2000 * 4];
    }
    if (address >= 0xA000 && address < 0xC000) {
        return cartridge->prg[address - 0xA000 + 0x2000 * 5];
    }
    if (address >= 0xC000 && address < 0xE000) {
        // Go adds these as uint16, so a bank past 7 wraps back around
        return cartridge->prg[static_cast<uint16_t>(address - 0xC000 + 0x2000 * bank)];
    }
    if (address >= 0xE000) {
        return cartridge->prg[address - 0xE000 + 0x2000 * 7];
    }
    throw std::runtime_error{std::format("unhandled mapper40 read at address: 0x{:04X}", address)};
}

void Mapper40::write(const uint16_t address, const uint8_t value) {
    if (address < 0x2000) {
        cartridge->chr[address] = value;
    } else if (address >= 0x8000 && address < 0xA000) {
        cycles = -1;
    } else if (address >= 0xA000 && address < 0xC000) {
        cycles = 0;
    } else if (address >= 0xE000) {
        bank = value;
    } else {
        std::println("unhandled mapper40 write at address: 0x{:04X}", address);
    }
}
