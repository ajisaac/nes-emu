#include "PPU.h"

#include <utility>

#include "Console.h"

void PPU::Buffer::SetRGBA(const int x, const int y, const Color c) {
    const int i = (y * width + x) * 4;
    pix[i + 0] = c.r;
    pix[i + 1] = c.g;
    pix[i + 2] = c.b;
    pix[i + 3] = c.a;
}

PPU::PPU(Console *console)
    : ram{console->ram}, console{console}, front{std::make_unique<Buffer>()}, back{std::make_unique<Buffer>()} {
    reset();
}

uint8_t PPU::Read(const uint16_t address) {
    return ram->ReadPpuMemory(address);
}

void PPU::Write(const uint16_t address, const uint8_t value) {
    ram->WritePpuMemory(address, value);
}

void PPU::reset() {
    cycle = 340;
    scanline = 240;
    frame = 0;
    writeControl(0);
    writeMask(0);
    writeOAMAddress(0);
}

uint8_t PPU::readPalette(uint16_t address) const {
    if (address >= 16 && address % 4 == 0) {
        address -= 16;
    }
    return paletteData[address];
}

void PPU::writePalette(uint16_t address, const uint8_t value) {
    if (address >= 16 && address % 4 == 0) {
        address -= 16;
    }
    paletteData[address] = value;
}

uint8_t PPU::readRegister(const uint16_t address) {
    switch (address) {
        case 0x2002:
            return readStatus();
        case 0x2004:
            return readOAMData();
        case 0x2007:
            return readData();
    }
    return 0;
}

void PPU::writeRegister(const uint16_t address, const uint8_t value) {
    cpu_register = value;
    switch (address) {
        case 0x2000:
            writeControl(value);
            break;
        case 0x2001:
            writeMask(value);
            break;
        case 0x2003:
            writeOAMAddress(value);
            break;
        case 0x2004:
            writeOAMData(value);
            break;
        case 0x2005:
            writeScroll(value);
            break;
        case 0x2006:
            writeAddress(value);
            break;
        case 0x2007:
            writeData(value);
            break;
        case 0x4014:
            writeDMA(value);
            break;
    }
}

// $2000: PPUCTRL
void PPU::writeControl(const uint8_t value) {
    flagNameTable = (value >> 0) & 3;
    flagIncrement = (value >> 2) & 1;
    flagSpriteTable = (value >> 3) & 1;
    flagBackgroundTable = (value >> 4) & 1;
    flagSpriteSize = (value >> 5) & 1;
    flagMasterSlave = (value >> 6) & 1;
    nmiOutput = ((value >> 7) & 1) == 1;
    nmiChange();
    // t: ....BA.. ........ = d: ......BA
    t = (t & 0xF3FF) | ((static_cast<uint16_t>(value) & 0x03) << 10);
}

// $2001: PPUMASK
void PPU::writeMask(const uint8_t value) {
    flagGrayscale = (value >> 0) & 1;
    flagShowLeftBackground = (value >> 1) & 1;
    flagShowLeftSprites = (value >> 2) & 1;
    flagShowBackground = (value >> 3) & 1;
    flagShowSprites = (value >> 4) & 1;
    flagRedTint = (value >> 5) & 1;
    flagGreenTint = (value >> 6) & 1;
    flagBlueTint = (value >> 7) & 1;
}

// $2002: PPUSTATUS
uint8_t PPU::readStatus() {
    uint8_t result = cpu_register & 0x1F;
    result |= flagSpriteOverflow << 5;
    result |= flagSpriteZeroHit << 6;
    if (nmiOccurred) {
        result |= 1 << 7;
    }
    nmiOccurred = false;
    nmiChange();
    // w:                   = 0
    w = 0;
    return result;
}

// $2003: OAMADDR
void PPU::writeOAMAddress(const uint8_t value) {
    oamAddress = value;
}

// $2004: OAMDATA (read)
uint8_t PPU::readOAMData() const {
    uint8_t data = oamData[oamAddress];
    if ((oamAddress & 0x03) == 0x02) {
        data = data & 0xE3;
    }
    return data;
}

// $2004: OAMDATA (write)
void PPU::writeOAMData(const uint8_t value) {
    oamData[oamAddress] = value;
    oamAddress++;
}

// $2005: PPUSCROLL
void PPU::writeScroll(const uint8_t value) {
    if (w == 0) {
        // t: ........ ...HGFED = d: HGFED...
        // x:               CBA = d: .....CBA
        // w:                   = 1
        t = (t & 0xFFE0) | (static_cast<uint16_t>(value) >> 3);
        x = value & 0x07;
        w = 1;
    } else {
        // t: .CBA..HG FED..... = d: HGFEDCBA
        // w:                   = 0
        t = (t & 0x8FFF) | ((static_cast<uint16_t>(value) & 0x07) << 12);
        t = (t & 0xFC1F) | ((static_cast<uint16_t>(value) & 0xF8) << 2);
        w = 0;
    }
}

// $2006: PPUADDR
void PPU::writeAddress(const uint8_t value) {
    if (w == 0) {
        // t: ..FEDCBA ........ = d: ..FEDCBA
        // t: .X...... ........ = 0
        // w:                   = 1
        t = (t & 0x80FF) | ((static_cast<uint16_t>(value) & 0x3F) << 8);
        w = 1;
    } else {
        // t: ........ HGFEDCBA = d: HGFEDCBA
        // v                    = t
        // w:                   = 0
        t = (t & 0xFF00) | static_cast<uint16_t>(value);
        v = t;
        w = 0;
    }
}

// $2007: PPUDATA (read)
uint8_t PPU::readData() {
    uint8_t value = Read(v);
    // emulate buffered reads
    if (v % 0x4000 < 0x3F00) {
        const uint8_t buffered = bufferedData;
        bufferedData = value;
        value = buffered;
    } else {
        bufferedData = Read(v - 0x1000);
    }
    // increment address
    if (flagIncrement == 0) {
        v += 1;
    } else {
        v += 32;
    }
    return value;
}

// $2007: PPUDATA (write)
void PPU::writeData(const uint8_t value) {
    Write(v, value);
    if (flagIncrement == 0) {
        v += 1;
    } else {
        v += 32;
    }
}

// $4014: OAMDMA
void PPU::writeDMA(const uint8_t value) {
    CPU *cpu = console->cpu;
    uint16_t address = static_cast<uint16_t>(value) << 8;
    for (int i = 0; i < 256; i++) {
        oamData[oamAddress] = cpu->Read(address);
        oamAddress++;
        address++;
    }
    cpu->stall += 513;
    if (cpu->Cycles % 2 == 1) {
        cpu->stall++;
    }
}

// NTSC Timing Helper Functions

void PPU::incrementX() {
    // increment hori(v)
    // if coarse X == 31
    if ((v & 0x001F) == 31) {
        // coarse X = 0
        v &= 0xFFE0;
        // switch horizontal nametable
        v ^= 0x0400;
    } else {
        // increment coarse X
        v++;
    }
}

void PPU::incrementY() {
    // increment vert(v)
    // if fine Y < 7
    if ((v & 0x7000) != 0x7000) {
        // increment fine Y
        v += 0x1000;
    } else {
        // fine Y = 0
        v &= 0x8FFF;
        // let y = coarse Y
        uint16_t y = (v & 0x03E0) >> 5;
        if (y == 29) {
            // coarse Y = 0
            y = 0;
            // switch vertical nametable
            v ^= 0x0800;
        } else if (y == 31) {
            // coarse Y = 0, nametable not switched
            y = 0;
        } else {
            // increment coarse Y
            y++;
        }
        // put coarse Y back into v
        v = (v & 0xFC1F) | (y << 5);
    }
}

void PPU::copyX() {
    // hori(v) = hori(t)
    // v: .....F.. ...EDCBA = t: .....F.. ...EDCBA
    v = (v & 0xFBE0) | (t & 0x041F);
}

void PPU::copyY() {
    // vert(v) = vert(t)
    // v: .IHGF.ED CBA..... = t: .IHGF.ED CBA.....
    v = (v & 0x841F) | (t & 0x7BE0);
}

void PPU::nmiChange() {
    const bool nmi = nmiOutput && nmiOccurred;
    if (nmi && !nmiPrevious) {
        // TODO: this fixes some games but the delay shouldn't have to be so
        // long, so the timings are off somewhere
        nmiDelay = 15;
    }
    nmiPrevious = nmi;
}

void PPU::setVerticalBlank() {
    std::swap(front, back);
    nmiOccurred = true;
    nmiChange();
}

void PPU::clearVerticalBlank() {
    nmiOccurred = false;
    nmiChange();
}

void PPU::fetchNameTableByte() {
    const uint16_t address = 0x2000 | (v & 0x0FFF);
    nameTableByte = Read(address);
}

void PPU::fetchAttributeTableByte() {
    const uint16_t address = 0x23C0 | (v & 0x0C00) | ((v >> 4) & 0x38) | ((v >> 2) & 0x07);
    const uint16_t shift = ((v >> 4) & 4) | (v & 2);
    attributeTableByte = ((Read(address) >> shift) & 3) << 2;
}

void PPU::fetchLowTileByte() {
    const uint16_t fineY = (v >> 12) & 7;
    const uint8_t table = flagBackgroundTable;
    const uint8_t tile = nameTableByte;
    const uint16_t address = 0x1000 * table + tile * 16 + fineY;
    lowTileByte = Read(address);
}

void PPU::fetchHighTileByte() {
    const uint16_t fineY = (v >> 12) & 7;
    const uint8_t table = flagBackgroundTable;
    const uint8_t tile = nameTableByte;
    const uint16_t address = 0x1000 * table + tile * 16 + fineY;
    highTileByte = Read(address + 8);
}

void PPU::storeTileData() {
    uint32_t data = 0;
    for (int i = 0; i < 8; i++) {
        const uint8_t a = attributeTableByte;
        const uint8_t p1 = (lowTileByte & 0x80) >> 7;
        const uint8_t p2 = (highTileByte & 0x80) >> 6;
        lowTileByte <<= 1;
        highTileByte <<= 1;
        data <<= 4;
        data |= a | p1 | p2;
    }
    tileData |= data;
}

uint32_t PPU::fetchTileData() const {
    return tileData >> 32;
}

uint8_t PPU::backgroundPixel() const {
    if (flagShowBackground == 0) {
        return 0;
    }
    const uint32_t data = fetchTileData() >> ((7 - x) * 4);
    return data & 0x0F;
}

PPU::SpritePixelReturn PPU::spritePixel() const {
    if (flagShowSprites == 0) {
        return {0, 0};
    }
    for (int i = 0; i < spriteCount; i++) {
        int offset = (cycle - 1) - static_cast<int>(spritePositions[i]);
        if (offset < 0 || offset > 7) {
            continue;
        }
        offset = 7 - offset;
        const uint8_t color = (spritePatterns[i] >> (offset * 4)) & 0x0F;
        if (color % 4 == 0) {
            continue;
        }
        return {static_cast<uint8_t>(i), color};
    }
    return {0, 0};
}

void PPU::renderPixel() {
    const int x = cycle - 1;
    const int y = scanline;
    uint8_t background = backgroundPixel();
    auto [i, sprite] = spritePixel();
    if (x < 8 && flagShowLeftBackground == 0) {
        background = 0;
    }
    if (x < 8 && flagShowLeftSprites == 0) {
        sprite = 0;
    }
    const bool b = background % 4 != 0;
    const bool s = sprite % 4 != 0;
    uint8_t color;
    if (!b && !s) {
        color = 0;
    } else if (!b && s) {
        color = sprite | 0x10;
    } else if (b && !s) {
        color = background;
    } else {
        if (spriteIndexes[i] == 0 && x < 255) {
            flagSpriteZeroHit = 1;
        }
        if (spritePriorities[i] == 0) {
            color = sprite | 0x10;
        } else {
            color = background;
        }
    }
    const Color c = Palette[readPalette(color) % 64];
    back->SetRGBA(x, y, c);
}

uint32_t PPU::fetchSpritePattern(const int i, int row) {
    uint8_t tile = oamData[i * 4 + 1];
    const uint8_t attributes = oamData[i * 4 + 2];
    uint16_t address;
    if (flagSpriteSize == 0) {
        if ((attributes & 0x80) == 0x80) {
            row = 7 - row;
        }
        const uint8_t table = flagSpriteTable;
        address = 0x1000 * table + tile * 16 + row;
    } else {
        if ((attributes & 0x80) == 0x80) {
            row = 15 - row;
        }
        const uint8_t table = tile & 1;
        tile &= 0xFE;
        if (row > 7) {
            tile++;
            row -= 8;
        }
        address = 0x1000 * table + tile * 16 + row;
    }
    const uint8_t a = (attributes & 3) << 2;
    uint8_t lowTileByte = Read(address);
    uint8_t highTileByte = Read(address + 8);
    uint32_t data = 0;
    for (int j = 0; j < 8; j++) {
        uint8_t p1;
        uint8_t p2;
        if ((attributes & 0x40) == 0x40) {
            p1 = (lowTileByte & 1) << 0;
            p2 = (highTileByte & 1) << 1;
            lowTileByte >>= 1;
            highTileByte >>= 1;
        } else {
            p1 = (lowTileByte & 0x80) >> 7;
            p2 = (highTileByte & 0x80) >> 6;
            lowTileByte <<= 1;
            highTileByte <<= 1;
        }
        data <<= 4;
        data |= a | p1 | p2;
    }
    return data;
}

void PPU::evaluateSprites() {
    int h;
    if (flagSpriteSize == 0) {
        h = 8;
    } else {
        h = 16;
    }
    int count = 0;
    for (int i = 0; i < 64; i++) {
        const uint8_t y = oamData[i * 4 + 0];
        const uint8_t a = oamData[i * 4 + 2];
        const uint8_t x = oamData[i * 4 + 3];
        const int row = scanline - static_cast<int>(y);
        if (row < 0 || row >= h) {
            continue;
        }
        if (count < 8) {
            spritePatterns[count] = fetchSpritePattern(i, row);
            spritePositions[count] = x;
            spritePriorities[count] = (a >> 5) & 1;
            spriteIndexes[count] = static_cast<uint8_t>(i);
        }
        count++;
    }
    if (count > 8) {
        count = 8;
        flagSpriteOverflow = 1;
    }
    spriteCount = count;
}

// tick updates Cycle, ScanLine and Frame counters
void PPU::tick() {
    if (nmiDelay > 0) {
        nmiDelay--;
        if (nmiDelay == 0 && nmiOutput && nmiOccurred) {
            console->cpu->triggerNMI();
        }
    }

    if (flagShowBackground != 0 || flagShowSprites != 0) {
        if (f == 1 && scanline == 261 && cycle == 339) {
            cycle = 0;
            scanline = 0;
            frame++;
            f ^= 1;
            return;
        }
    }
    cycle++;
    if (cycle > 340) {
        cycle = 0;
        scanline++;
        if (scanline > 261) {
            scanline = 0;
            frame++;
            f ^= 1;
        }
    }
}

// step executes a single PPU cycle
void PPU::step() {
    tick();

    const bool renderingEnabled = flagShowBackground != 0 || flagShowSprites != 0;
    const bool preLine = scanline == 261;
    const bool visibleLine = scanline < 240;
    // const bool postLine = scanline == 240;
    const bool renderLine = preLine || visibleLine;
    const bool preFetchCycle = cycle >= 321 && cycle <= 336;
    const bool visibleCycle = cycle >= 1 && cycle <= 256;
    const bool fetchCycle = preFetchCycle || visibleCycle;

    // background logic
    if (renderingEnabled) {
        if (visibleLine && visibleCycle) {
            renderPixel();
        }
        if (renderLine && fetchCycle) {
            tileData <<= 4;
            switch (cycle % 8) {
                case 1:
                    fetchNameTableByte();
                    break;
                case 3:
                    fetchAttributeTableByte();
                    break;
                case 5:
                    fetchLowTileByte();
                    break;
                case 7:
                    fetchHighTileByte();
                    break;
                case 0:
                    storeTileData();
                    break;
            }
        }
        if (preLine && cycle >= 280 && cycle <= 304) {
            copyY();
        }
        if (renderLine) {
            if (fetchCycle && cycle % 8 == 0) {
                incrementX();
            }
            if (cycle == 256) {
                incrementY();
            }
            if (cycle == 257) {
                copyX();
            }
        }
    }

    // sprite logic
    if (renderingEnabled) {
        if (cycle == 257) {
            if (visibleLine) {
                evaluateSprites();
            } else {
                spriteCount = 0;
            }
        }
    }

    // vblank logic
    if (scanline == 241 && cycle == 1) {
        setVerticalBlank();
    }
    if (preLine && cycle == 1) {
        clearVerticalBlank();
        flagSpriteZeroHit = 0;
        flagSpriteOverflow = 0;
    }
}
