#ifndef NES_EMU_PPU_H
#define NES_EMU_PPU_H
#include <cstdint>

class Console;
class Ram;
// class CPU {
//
// };
// class Console {
// public:
//     CPU* CPU;
// };
// class Ram;
//
class PPU {
public:
    class Rect {
    public:
        Rect() = default;

        Rect(int i, int i1, int i2, int i3){};
    };

    class RGBA {
    public:
        RGBA() = default;

        explicit RGBA(Rect rect){};
    };

    Ram *ram;
    Console *console;

    int32_t Cycle{};      // 0-340
    int32_t ScanLine{};   // 0-261, 0-239=visible, 240=post, 241-260=vblank, 261=pre
    uint64_t Frame{};     // frame counter

    // storage variables
    uint8_t paletteData[32]{};
    uint8_t nameTableData[2048]{};
    uint8_t oamData[256]{};
    RGBA front{};
    RGBA back{};

    // PPU registers
    uint16_t v{};  // current vram address (15 bit)
    uint16_t t{};  // temporary vram address (15 bit)
    uint8_t x{};   // fine x scroll (3 bit)
    uint8_t w{};   // write toggle (1 bit)
    uint8_t f{};   // even/odd frame flag (1 bit)

    uint8_t cpu_register{};

    // NMI flags
    bool nmiOccurred{};
    bool nmiOutput{};
    bool nmiPrevious{};
    uint8_t nmiDelay{};

    // background temporary variables
    uint8_t nameTableByte{};
    uint8_t attributeTableByte{};
    uint8_t lowTileByte{};
    uint8_t highTileByte{};
    uint64_t tileData{};

    // sprite temporary variables
    int32_t spriteCount{};
    uint32_t spritePatterns[8]{};
    uint8_t spritePositions[8]{};
    uint8_t spritePriorities[8]{};
    uint8_t spriteIndexes[8]{};

    // $2000 PPUCTRL
    uint8_t flagNameTable{}; // 0: $2000; 1: $2400; 2: $2800; 3: $2C00
    uint8_t flagIncrement{}; // 0: add 1; 1: add 32
    uint8_t flagSpriteTable{}; // 0: $0000; 1: $1000; ignored in 8x16 mode
    uint8_t flagBackgroundTable{}; // 0: $0000; 1: $1000
    uint8_t flagSpriteSize{}; // 0: 8x8; 1: 8x16
    uint8_t flagMasterSlave{}; // 0: read EXT; 1: write EXT

    // $2001 PPUMASK
    uint8_t flagGrayscale{}; // 0: color; 1: grayscale
    uint8_t flagShowLeftBackground{}; // 0: hide; 1: show
    uint8_t flagShowLeftSprites{}; // 0: hide; 1: show
    uint8_t flagShowBackground{}; // 0: hide; 1: show
    uint8_t flagShowSprites{}; // 0: hide; 1: show
    uint8_t flagRedTint{}; // 0: normal; 1: emphasized
    uint8_t flagGreenTint{}; // 0: normal; 1: emphasized
    uint8_t flagBlueTint{}; // 0: normal; 1: emphasized

    // $2002 PPUSTATUS
    uint8_t flagSpriteZeroHit{};
    uint8_t flagSpriteOverflow{};

    // $2003 OAMADDR
    uint8_t oamAddress{};

    // $2007 PPUDATA
    uint8_t bufferedData{}; // for buffered reads


    explicit PPU(Console *console);

    uint8_t Read(uint16_t address);

    void Write(uint16_t address, uint8_t value);

    void reset();

    uint8_t readPalette(uint16_t address);

    void writePalette(uint16_t address, uint8_t value);

    uint8_t readRegister(uint16_t address);

    void writeRegister(uint16_t address, uint8_t value);

    // $2000: PPUCTRL
    void writeControl(uint8_t value);

    // $2001: PPUMASK
    void writeMask(uint8_t value);

    // $2002: PPUSTATUS
    uint8_t readStatus();

    // $2003: OAMADDR
    void writeOAMAddress(uint8_t value);

    // $2004: OAMDATA (read)
    uint8_t readOAMData();

    // $2004: OAMDATA (write)
    void writeOAMData(uint8_t value);

    // $2005: PPUSCROLL
    void writeScroll(uint8_t value);

    // $2006: PPUADDR
    void writeAddress(uint8_t value);

    // $2007: PPUDATA (read)
    uint8_t readData();

    // $2007: PPUDATA (write)
    void writeData(uint8_t value);

    // $4014: OAMDMA
    void writeDMA(uint8_t value);

    // NTSC Timing Helper Functions

    void incrementX();

    void incrementY();

    void copyX();

    void copyY();

    void nmiChange();

    void setVerticalBlank();

    void clearVerticalBlank();

    void fetchNameTableByte();

    void fetchAttributeTableByte();

    void fetchLowTileByte();

    void fetchHighTileByte();

    void storeTileData();

    uint32_t fetchTileData();

    uint8_t backgroundPixel();

    // todo fix
    // (uint8_t, uint8_t)  spritePixel() {
    void spritePixel();

    void renderPixel();

    uint32_t fetchSpritePattern(int32_t i, int32_t row);

    void evaluateSprites();

    // tick updates Cycle, ScanLine and Frame counters
    void tick();

    // Step executes a single PPU cycle
    void step();
};


#endif //NES_EMU_PPU_H
