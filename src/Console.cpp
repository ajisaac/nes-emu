
#include "Console.h"


const iNESFileMagic = 0x1a53454e

type iNESFileHeader struct {
    Magic    uint32  // iNES magic number
    NumPRG   byte    // number of PRG-ROM banks (16KB each)
    NumCHR   byte    // number of CHR-ROM banks (8KB each)
    Control1 byte    // control bits
    Control2 byte    // control bits
    NumRAM   byte    // PRG-RAM size (x 8KB)
    _        [7]byte // unused padding
}

// LoadNESFile reads an iNES file (.nes) and returns a Cartridge on success.
// http://wiki.nesdev.com/w/index.php/INES
// http://nesdev.com/NESDoc.pdf (page 28)
func LoadNESFile(path string) (*Cartridge, error) {
    // open file
    file, err := os.Open(path)
    if err != nil {
        return nil, err
    }
    defer file.Close()

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

Console::Console(std::string const& path) {

    cartridge = LoadNESFile(path);
    // ram = make([]byte, 2048)
    ram = new Ram();
    controller1 = new Controller();
    controller2 = new Controller();
    mapper = new Mapper();
    cpu = new CPU();
    apu = new APU();
    ppu = new PPU();
}

void Console::reset() {
    cpu->Reset();
}

int Console::step() {
    const auto cpuCycles = cpu->step();
    const auto ppuCycles = cpuCycles * 3;
    for (int i = 0; i < ppuCycles; i++) {
        ppu->step();
        mapper->step();
    }
    for (int i = 0; i < cpuCycles; i++) {
        apu->step();
    }
    return cpuCycles;
}

int Console::stepFrame() {
    auto cpuCycles = 0;
    unsigned long long frame = ppu->frame;
    while (frame == ppu->frame) {
        cpuCycles += step();
    }
    return cpuCycles;
}

void Console::stepSeconds(double seconds) {
    // cpu frequency
    auto cycles = 1789773 * seconds * 1000;
    while (cycles > 0) {
        cycles -= step();
    }
}

PPU::Buffer *Console::buffer() {
    return &ppu->front;
}

PPU::Buffer *Console::backgroundColor() {
    return Palette[ppu->readPalette(0)%64];
}

void Console::setButtons1(char buttons) {}

void Console::setButtons2(char buttons) {}

void Console::setAudioChannel() {}

void Console::setAudioSampleRate(long long sampleRate) {}

void Console::saveState(std::string filename) {}

void Console::loadState(std::string filename) {}
