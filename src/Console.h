#ifndef CPPTEST_CONSOLE_H
#define CPPTEST_CONSOLE_H

#include <bitset>
#include <utility>

#include "Cpu.h"
#include "Ram.h"
#include "Cartridge.h"

class Console {
public:
    PPU ppu{};
    APU apu{};
    Controller controller1{};
    Controller controller2{};
    Cartridge cartridge;
    Mapper mapper{};
    Ram ram{&ppu, &apu, &controller1, &controller2, &mapper};
    Cpu cpu{&ram};

    long long start{};

    explicit Console(Cartridge cartridge) : cartridge(std::move(cartridge)) {
        start = std::chrono::system_clock::now().time_since_epoch().count();
    };

    ~Console() = default;

    void run() {
        // load rom
        // start running
        while (true) {
            long long now = std::chrono::system_clock::now().time_since_epoch().count();
            long long elapsed = now - start;
            start = now;
            std::cout << elapsed << std::endl;
            cpu.Step();
        }
    }
//
//    func (console *Console) Reset() {
//        console.CPU.Reset()
//    }
//
//    func (console *Console) Step() int {
//        cpuCycles := console.CPU.Step()
//        ppuCycles := cpuCycles * 3
//        for i := 0; i < ppuCycles; i++ {
//            console.PPU.Step()
//            console.Mapper.Step()
//        }
//        for i := 0; i < cpuCycles; i++ {
//            console.APU.Step()
//        }
//        return cpuCycles
//    }
//
//    func (console *Console) StepFrame() int {
//        cpuCycles := 0
//        frame := console.PPU.Frame
//        for frame == console.PPU.Frame {
//            cpuCycles += console.Step()
//        }
//        return cpuCycles
//    }
//
//    func (console *Console) StepSeconds(seconds float64) {
//        cycles := int(CPUFrequency * seconds)
//        for cycles > 0 {
//            cycles -= console.Step()
//        }
//    }
//
//    func (console *Console) Buffer() *image.RGBA {
//        return console.PPU.front
//    }
//
//    func (console *Console) BackgroundColor() color.RGBA {
//        return Palette[console.PPU.readPalette(0)%64]
//    }
//
//    func (console *Console) SetButtons1(buttons [8]bool) {
//        console.Controller1.SetButtons(buttons)
//    }
//
//    func (console *Console) SetButtons2(buttons [8]bool) {
//        console.Controller2.SetButtons(buttons)
//    }
//
//    func (console *Console) SetAudioChannel(channel chan float32) {
//        console.APU.channel = channel
//    }
//
//    func (console *Console) SetAudioSampleRate(sampleRate float64) {
//        if sampleRate != 0 {
//            // Convert samples per second to cpu steps per sample
//            console.APU.sampleRate = CPUFrequency / sampleRate
//            // Initialize filters
//            console.APU.filterChain = FilterChain{
//                    HighPassFilter(float32(sampleRate), 90),
//                    HighPassFilter(float32(sampleRate), 440),
//                    LowPassFilter(float32(sampleRate), 14000),
//            }
//        } else {
//            console.APU.filterChain = nil
//        }
//    }
//    func (console *Console) SaveState(filename string) error {
//        dir, _ := path.Split(filename)
//        if err := os.MkdirAll(dir, 0755); err != nil {
//                return err
//        }
//        file, err := os.Create(filename)
//        if err != nil {
//                    return err
//            }
//        defer file.Close()
//        encoder := gob.NewEncoder(file)
//        return console.Save(encoder)
//    }
//
//    func (console *Console) Save(encoder *gob.Encoder) error {
//        encoder.Encode(console.RAM)
//        console.CPU.Save(encoder)
//        console.APU.Save(encoder)
//        console.PPU.Save(encoder)
//        console.Cartridge.Save(encoder)
//        console.Mapper.Save(encoder)
//        return encoder.Encode(true)
//    }
//
//    func (console *Console) LoadState(filename string) error {
//        file, err := os.Open(filename)
//        if err != nil {
//                    return err
//            }
//        defer file.Close()
//        decoder := gob.NewDecoder(file)
//        return console.Load(decoder)
//    }
//
//    func (console *Console) Load(decoder *gob.Decoder) error {
//        decoder.Decode(&console.RAM)
//        console.CPU.Load(decoder)
//        console.APU.Load(decoder)
//        console.PPU.Load(decoder)
//        console.Cartridge.Load(decoder)
//        console.Mapper.Load(decoder)
//        var dummy bool
//        if err := decoder.Decode(&dummy); err != nil {
//                return err
//        }
//        return nil
//    }

};

#endif
