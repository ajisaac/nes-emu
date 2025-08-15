#include "PPU.h"

#include "Console.h"

PPU::PPU(Console *console) {
    // ppu := PPU{Memory: NewPPUMemory(console), console: console};
    front = RGBA{Rect{0, 0, 256, 240}};
    back = RGBA{Rect{0, 0, 256, 240}};
    reset();
}

uint8_t PPU::Read(uint16_t address) {
}

void PPU::Write(uint16_t address, uint8_t value) {
}


void PPU::reset() {
    Cycle = 340;
    ScanLine = 240;
    Frame = 0;
    writeControl(0);
    writeMask(0);
    writeOAMAddress(0);
}

uint8_t PPU::readPalette(uint16_t address) {
    if (address >= 16 && address % 4 == 0) {
        address -= 16;
    }
    return paletteData[address];
}

void PPU::writePalette(uint16_t address, uint8_t value) {
    if (address >= 16 && address % 4 == 0) {
        address -= 16;
    }
    paletteData[address] = value;
}

uint8_t PPU::readRegister(uint16_t address) {
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

void PPU::writeRegister(uint16_t address, uint8_t value) {
    cpu_register = value;
    switch (address) {
        case 0x2000:
            writeControl(value);
        case 0x2001:
            writeMask(value);
        case 0x2003:
            writeOAMAddress(value);
        case 0x2004:
            writeOAMData(value);
        case 0x2005:
            writeScroll(value);
        case 0x2006:
            writeAddress(value);
        case 0x2007:
            writeData(value);
        case 0x4014:
            writeDMA(value);
    }
}

void PPU::writeControl(uint8_t value) {
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

void PPU::writeMask(uint8_t value) {
    flagGrayscale = (value >> 0) & 1;
    flagShowLeftBackground = (value >> 1) & 1;
    flagShowLeftSprites = (value >> 2) & 1;
    flagShowBackground = (value >> 3) & 1;
    flagShowSprites = (value >> 4) & 1;
    flagRedTint = (value >> 5) & 1;
    flagGreenTint = (value >> 6) & 1;
    flagBlueTint = (value >> 7) & 1;
}

uint8_t PPU::readStatus() {
    auto result = cpu_register & 0x1F;
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

void PPU::writeOAMAddress(uint8_t value) {
    oamAddress = value;
}

uint8_t PPU::readOAMData() {
    auto data = oamData[oamAddress];
    if ((oamAddress & 0x03) == 0x02) {
        data = data & 0xE3;
    }
    return data;
}

void PPU::writeOAMData(uint8_t value) {
    oamData[oamAddress] = value;
    oamAddress++;
}

void PPU::writeScroll(uint8_t value) {
    if (w == 0) {
        // t: ........ ...HGFED = d: HGFED...
        // x:               CBA = d: .....CBA
        // w:                   = 1
        t = (t & 0xFFE0) | (uint16_t(value) >> 3);
        x = value & 0x07;
        w = 1;
    } else {
        // t: .CBA..HG FED..... = d: HGFEDCBA
        // w:                   = 0
        t = (t & 0x8FFF) | ((uint16_t(value) & 0x07) << 12);
        t = (t & 0xFC1F) | ((uint16_t(value) & 0xF8) << 2);
        w = 0;
    }
}

void PPU::writeAddress(uint8_t value) {
    if (w == 0) {
        // t: ..FEDCBA ........ = d: ..FEDCBA
        // t: .X...... ........ = 0
        // w:                   = 1
        t = (t & 0x80FF) | ((uint16_t(value) & 0x3F) << 8);
        w = 1;
    } else {
        // t: ........ HGFEDCBA = d: HGFEDCBA
        // v                    = t
        // w:                   = 0
        t = (t & 0xFF00) | uint16_t(value);
        v = t;
        w = 0;
    }
}

uint8_t PPU::readData() {
    auto value = Read(v);
    // emulate buffered reads
    if (v % 0x4000 < 0x3F00) {
        auto buffered = bufferedData;
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

void PPU::writeData(uint8_t value) {
    Write(v, value);
    if (flagIncrement == 0) {
        v += 1;
    } else {
        v += 32;
    }
}

void PPU::writeDMA(uint8_t value) {
    auto cpu = console->cpu;
    auto address = uint16_t(value) << 8;
    for (auto i = 0; i < 256; i++) {
        oamData[oamAddress] = cpu.Read(address);
        oamAddress++;
        address++;
    }
    cpu.stall += 513;
    if (cpu.Cycles % 2 == 1) {
        cpu.stall++;
    }
}

void PPU::incrementX() {
    // increment hori(v)
    // if coarse X == 31
    if (v & 0x001F == 31) {
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
    if (v & 0x7000 != 0x7000) {
        // increment fine Y
        v += 0x1000;
    } else {
        // fine Y = 0
        v &= 0x8FFF;
        // let y = coarse Y
        auto y = (v & 0x03E0) >> 5;
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
    auto nmi = nmiOutput && nmiOccurred;
    if (nmi && !nmiPrevious) {
        // TODO: this fixes some games but the delay shouldn't have to be so
        // long, so the timings are off somewhere
        nmiDelay = 15;
    }
    nmiPrevious = nmi;
}

void PPU::setVerticalBlank() {
    front, back = back, front;
    nmiOccurred = true;
    nmiChange();
}

void PPU::clearVerticalBlank() {
    nmiOccurred = false;
    nmiChange();
}

void PPU::fetchNameTableByte() {
    // todo
    // auto v = v;
    auto address = 0x2000 | (v & 0x0FFF);
    nameTableByte = Read(address);
}

void PPU::fetchAttributeTableByte() {
    // todo
    // auto v = v;
    auto address = 0x23C0 | (v & 0x0C00) | ((v >> 4) & 0x38) | ((v >> 2) & 0x07);
    auto shift = ((v >> 4) & 4) | (v & 2);
    attributeTableByte = ((Read(address) >> shift) & 3) << 2;
}

void PPU::fetchLowTileByte() {
    auto fineY = (v >> 12) & 7;
    auto table = flagBackgroundTable;
    auto tile = nameTableByte;
    auto address = 0x1000 * uint16_t(table) + uint16_t(tile) * 16 + fineY;
    lowTileByte = Read(address);
}

void PPU::fetchHighTileByte() {
    auto fineY = (v >> 12) & 7;
    auto table = flagBackgroundTable;
    auto tile = nameTableByte;
    auto address = 0x1000 * uint16_t(table) + uint16_t(tile) * 16 + fineY;
    highTileByte = Read(address + 8);
}

void PPU::storeTileData() {
    uint32_t data;
    for (auto i = 0; i < 8; i++) {
        auto a = attributeTableByte;
        auto p1 = (lowTileByte & 0x80) >> 7;
        auto p2 = (highTileByte & 0x80) >> 6;
        lowTileByte <<= 1;
        highTileByte <<= 1;
        data <<= 4;
        data |= uint32_t(a | p1 | p2);
    }
    tileData |= uint64_t(data);
}

uint32_t PPU::fetchTileData() {
    return uint32_t(tileData >> 32);
}

uint8_t PPU::backgroundPixel() {
    if (flagShowBackground == 0) {
        return 0;
    }
    auto data = fetchTileData() >> ((7 - x) * 4);
    return uint8_t(data & 0x0F);
}

// (uint8_t, uint8_t)  spritePixel() {
void PPU::spritePixel() {
    if (flagShowSprites == 0) {
        // return 0, 0;
    }
    for (auto i = 0; i < spriteCount; i++) {
        auto offset = (Cycle - 1) - int(spritePositions[i]);
        if (offset < 0 || offset > 7) {
            continue;
        }
        offset = 7 - offset;
        auto color = uint8_t((spritePatterns[i] >> uint8_t(offset * 4)) & 0x0F);
        if (color % 4 == 0) {
            continue;
        }
        // return uint8_t(i), color;
    }
    // return 0, 0;
}

void PPU::renderPixel() {
    auto x = Cycle - 1;
    auto y = ScanLine;
    auto background = backgroundPixel();
    // auto i, auto sprite = spritePixel();
    // todo fix;
    auto i = 0;
    auto sprite = 0;
    if (x < 8 && flagShowLeftBackground == 0) {
        background = 0;
    }
    if (x < 8 && flagShowLeftSprites == 0) {
        sprite = 0;
    }
    auto b = background % 4 != 0;
    auto s = sprite % 4 != 0;
    std::uint8_t color;
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
    // todo
    // auto c = Palette[readPalette(uint16_t(color)) % 64];
    // back.SetRGBA(x, y, c);
}

uint32_t PPU::fetchSpritePattern(int32_t i, int32_t row) {
    auto tile = oamData[i * 4 + 1];
    auto attributes = oamData[i * 4 + 2];
    uint16_t address;
    if (flagSpriteSize == 0) {
        if (attributes & 0x80 == 0x80) {
            row = 7 - row;
        }
        auto table = flagSpriteTable;
        address = 0x1000 * uint16_t(table) + uint16_t(tile) * 16 + uint16_t(row);
    } else {
        if (attributes & 0x80 == 0x80) {
            row = 15 - row;
        }
        auto table = tile & 1;
        tile &= 0xFE;
        if (row > 7) {
            tile++;
            row -= 8;
        }
        address = 0x1000 * uint16_t(table) + uint16_t(tile) * 16 + uint16_t(row);
    }
    auto a = (attributes & 3) << 2;
    auto lowTileByte = Read(address);
    auto highTileByte = Read(address + 8);
    uint32_t data;
    for (auto i = 0; i < 8; i++) {
        std::uint8_t p1;
        std::uint8_t p2;
        if (attributes & 0x40 == 0x40) {
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
        data |= uint32_t(a | p1 | p2);
    }
    return data;
}

void PPU::evaluateSprites() {
    int32_t h;
    if (flagSpriteSize == 0) {
        h = 8;
    } else {
        h = 16;
    }
    auto count = 0;
    for (auto i = 0; i < 64; i++) {
        auto y = oamData[i * 4 + 0];
        auto a = oamData[i * 4 + 2];
        auto x = oamData[i * 4 + 3];
        auto row = ScanLine - int(y);
        if (row < 0 || row >= h) {
            continue;
        }
        if (count < 8) {
            spritePatterns[count] = fetchSpritePattern(i, row);
            spritePositions[count] = x;
            spritePriorities[count] = (a >> 5) & 1;
            spriteIndexes[count] = uint8_t(i);
        }
        count++;
    }
    if (count > 8) {
        count = 8;
        flagSpriteOverflow = 1;
    }
    spriteCount = count;
}

void PPU::tick() {
    if (nmiDelay > 0) {
        nmiDelay--;
        if (nmiDelay == 0 && nmiOutput && nmiOccurred) {
            console->cpu.triggerNMI();
        }
    }

    if (flagShowBackground != 0 || flagShowSprites != 0) {
        if (f == 1 && ScanLine == 261 && Cycle == 339) {
            Cycle = 0;
            ScanLine = 0;
            Frame++;
            f ^= 1;
            return;
        }
    }
    Cycle++;
    if (Cycle > 340) {
        Cycle = 0;
        ScanLine++;
        if (ScanLine > 261) {
            ScanLine = 0;
            Frame++;
            f ^= 1;
        }
    }
}

void PPU::step() {
    tick();

    auto renderingEnabled = flagShowBackground != 0 || flagShowSprites != 0;
    auto preLine = ScanLine == 261;
    auto visibleLine = ScanLine < 240;
    // postLine := ScanLine == 240
    auto renderLine = preLine || visibleLine;
    auto preFetchCycle = Cycle >= 321 && Cycle <= 336;
    auto visibleCycle = Cycle >= 1 && Cycle <= 256;
    auto fetchCycle = preFetchCycle || visibleCycle;

    // background logic
    if (renderingEnabled) {
        if (visibleLine && visibleCycle) {
            renderPixel();
        }
        if (renderLine && fetchCycle) {
            tileData <<= 4;
            switch (Cycle % 8) {
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
        if (preLine && Cycle >= 280 && Cycle <= 304) {
            copyY();
        }
        if (renderLine) {
            if (fetchCycle && Cycle % 8 == 0) {
                incrementX();
            }
            if (Cycle == 256) {
                incrementY();
            }
            if (Cycle == 257) {
                copyX();
            }
        }
    }

    // sprite logic
    if (renderingEnabled) {
        if (Cycle == 257) {
            if (visibleLine) {
                evaluateSprites();
            } else {
                spriteCount = 0;
            }
        }
    }

    // vblank logic
    if (ScanLine == 241 && Cycle == 1) {
        setVerticalBlank();
    }
    if (preLine && Cycle == 1) {
        clearVerticalBlank();
        flagSpriteZeroHit = 0;
        flagSpriteOverflow = 0;
    }
}


/*
    //    func (ppu *PPU) Save(encoder *gob.Encoder) error {
    //        encoder.Encode(Cycle)
    //        encoder.Encode(ScanLine)
    //        encoder.Encode(Frame)
    //        encoder.Encode(paletteData)
    //        encoder.Encode(nameTableData)
    //        encoder.Encode(oamData)
    //        encoder.Encode(v)
    //        encoder.Encode(t)
    //        encoder.Encode(x)
    //        encoder.Encode(w)
    //        encoder.Encode(f)
    //        encoder.Encode(cpu_register)
    //        encoder.Encode(nmiOccurred)
    //        encoder.Encode(nmiOutput)
    //        encoder.Encode(nmiPrevious)
    //        encoder.Encode(nmiDelay)
    //        encoder.Encode(nameTableByte)
    //        encoder.Encode(attributeTableByte)
    //        encoder.Encode(lowTileByte)
    //        encoder.Encode(highTileByte)
    //        encoder.Encode(tileData)
    //        encoder.Encode(spriteCount)
    //        encoder.Encode(spritePatterns)
    //        encoder.Encode(spritePositions)
    //        encoder.Encode(spritePriorities)
    //        encoder.Encode(spriteIndexes)
    //        encoder.Encode(flagNameTable)
    //        encoder.Encode(flagIncrement)
    //        encoder.Encode(flagSpriteTable)
    //        encoder.Encode(flagBackgroundTable)
    //        encoder.Encode(flagSpriteSize)
    //        encoder.Encode(flagMasterSlave)
    //        encoder.Encode(flagGrayscale)
    //        encoder.Encode(flagShowLeftBackground)
    //        encoder.Encode(flagShowLeftSprites)
    //        encoder.Encode(flagShowBackground)
    //        encoder.Encode(flagShowSprites)
    //        encoder.Encode(flagRedTint)
    //        encoder.Encode(flagGreenTint)
    //        encoder.Encode(flagBlueTint)
    //        encoder.Encode(flagSpriteZeroHit)
    //        encoder.Encode(flagSpriteOverflow)
    //        encoder.Encode(oamAddress)
    //        encoder.Encode(bufferedData)
    //        return nil
    //    }
    //
    //    func (ppu *PPU) Load(decoder *gob.Decoder) error {
    //        decoder.Decode(&Cycle)
    //        decoder.Decode(&ScanLine)
    //        decoder.Decode(&Frame)
    //        decoder.Decode(&paletteData)
    //        decoder.Decode(&nameTableData)
    //        decoder.Decode(&oamData)
    //        decoder.Decode(&v)
    //        decoder.Decode(&t)
    //        decoder.Decode(&x)
    //        decoder.Decode(&w)
    //        decoder.Decode(&f)
    //        decoder.Decode(&cpu_register)
    //        decoder.Decode(&nmiOccurred)
    //        decoder.Decode(&nmiOutput)
    //        decoder.Decode(&nmiPrevious)
    //        decoder.Decode(&nmiDelay)
    //        decoder.Decode(&nameTableByte)
    //        decoder.Decode(&attributeTableByte)
    //        decoder.Decode(&lowTileByte)
    //        decoder.Decode(&highTileByte)
    //        decoder.Decode(&tileData)
    //        decoder.Decode(&spriteCount)
    //        decoder.Decode(&spritePatterns)
    //        decoder.Decode(&spritePositions)
    //        decoder.Decode(&spritePriorities)
    //        decoder.Decode(&spriteIndexes)
    //        decoder.Decode(&flagNameTable)
    //        decoder.Decode(&flagIncrement)
    //        decoder.Decode(&flagSpriteTable)
    //        decoder.Decode(&flagBackgroundTable)
    //        decoder.Decode(&flagSpriteSize)
    //        decoder.Decode(&flagMasterSlave)
    //        decoder.Decode(&flagGrayscale)
    //        decoder.Decode(&flagShowLeftBackground)
    //        decoder.Decode(&flagShowLeftSprites)
    //        decoder.Decode(&flagShowBackground)
    //        decoder.Decode(&flagShowSprites)
    //        decoder.Decode(&flagRedTint)
    //        decoder.Decode(&flagGreenTint)
    //        decoder.Decode(&flagBlueTint)
    //        decoder.Decode(&flagSpriteZeroHit)
    //        decoder.Decode(&flagSpriteOverflow)
    //        decoder.Decode(&oamAddress)
    //        decoder.Decode(&bufferedData)
    //        return nil
    //    }
    //
    */
