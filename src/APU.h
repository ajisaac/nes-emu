#ifndef NES_EMU_APU_H
#define NES_EMU_APU_H
#include <cstdint>

#include "Console.h"

constexpr int CPUFrequency = 1789773;
constexpr double frameCounterRate = CPUFrequency / 240.0;

constexpr uint8_t lengthTable[32]{
    10, 254, 20, 2, 40, 4, 80, 6, 160, 8, 60, 10, 14, 12, 26, 14,
    12, 16, 24, 18, 48, 20, 96, 22, 192, 24, 72, 26, 16, 28, 32, 30
};

constexpr uint8_t dutyTable[4][8]{
    {0, 1, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 0, 0, 0},
    {1, 0, 0, 1, 1, 1, 1, 1},
};

constexpr uint8_t triangleTable[32]{
    15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
};

constexpr uint16_t noiseTable[16]{
    4, 8, 16, 32, 64, 96, 128, 160, 202, 254, 380, 508, 762, 1016, 2034, 4068
};

constexpr uint8_t dmcTable[16]{
    214, 190, 170, 160, 143, 127, 113, 107, 95, 80, 71, 64, 53, 42, 36, 27
};

class CPU;

class DMC {
    CPU *cpu;
    bool enabled;
    uint8_t value;
    uint16_t sampleAddress;
    uint16_t sampleLength;
    uint16_t currentAddress;
    uint16_t currentLength;
    uint8_t shiftRegister;
    uint8_t bitCount;
    uint8_t tickPeriod;
    uint8_t tickValue;
    bool loop;
    bool irq;

    void writeControl(uint8_t value);

    void writeValue(uint8_t value);

    void writeAddress(uint8_t value);

    void writeLength(uint8_t value);

    void restart();

    void stepTimer();

    void stepReader();

    void stepShifter();

    uint8_t output();

    char readRegister(short i);

    void writeRegister(short i, char i1);

    void step();
};

class Noise {
    bool enabled;
    bool mode;
    uint16_t shiftRegister;
    bool lengthEnabled;
    uint8_t lengthValue;
    uint16_t timerPeriod;
    uint16_t timerValue;
    bool envelopeEnabled;
    bool envelopeLoop;
    bool envelopeStart;
    uint8_t envelopePeriod;
    uint8_t envelopeValue;
    uint8_t envelopeVolume;
    uint8_t constantVolume;

    void writeControl(uint8_t value);

    void writePeriod(uint8_t value);

    void writeLength(uint8_t value);

    void stepTimer();

    void stepEnvelope();

    void stepLength();

    uint8_t output();
};

class Triangle {
    bool enabled;
    bool lengthEnabled;
    uint8_t lengthValue;
    uint16_t timerPeriod;
    uint16_t timerValue;
    uint8_t dutyValue;
    uint8_t counterPeriod;
    uint8_t counterValue;
    bool counterReload;

    void writeControl(uint8_t value);

    void writeTimerLow(uint8_t value);

    void writeTimerHigh(uint8_t value);

    void stepTimer();

    void stepLength();

    void stepCounter();

    uint8_t output();
};

class Pulse {
    bool enabled;
    uint8_t channel;
    bool lengthEnabled;
    uint8_t lengthValue;
    uint16_t timerPeriod;
    uint16_t timerValue;
    uint8_t dutyMode;
    uint8_t dutyValue;
    bool sweepReload;
    bool sweepEnabled;
    bool sweepNegate;
    uint8_t sweepShift;
    uint8_t sweepPeriod;
    uint8_t sweepValue;
    bool envelopeEnabled;
    bool envelopeLoop;
    bool envelopeStart;
    uint8_t envelopePeriod;
    uint8_t envelopeValue;
    uint8_t envelopeVolume;
    uint8_t constantVolume;

    void writeControl(uint8_t value);

    void writeSweep(uint8_t value);

    void writeTimerLow(uint8_t value);

    void writeTimerHigh(uint8_t value);

    void stepTimer();

    void stepEnvelope();

    void stepSweep();

    void stepLength();

    void sweep();

    uint8_t output();
};

class APU {
public:
    static float pulseTable[31];
    static float tndTable[203];

    // APU
    Console *console;
    float channel; // chan float channel
    double sampleRate;
    Pulse pulse1;
    Pulse pulse2;
    Triangle triangle;
    Noise noise;
    DMC dmc;
    uint64_t cycle;
    uint8_t framePeriod;
    uint8_t frameValue;
    bool frameIRQ;
    FilterChain filterChain;

    APU();

    void init();

    void step();

    void sendSample();

    float output();

    // mode 0:    mode 1:       function
    // ---------  -----------  -----------------------------
    //  - - - f    - - - - -    IRQ (if bit 6 is clear)
    //  - l - l    l - l - -    Length counter and sweep
    //  e e e e    e e e e -    Envelope and linear counter
    void stepFrameCounter();

    void stepTimer();

    void stepEnvelope();

    void stepSweep();

    void stepLength();

    void fireIRQ();

    uint8_t readRegister(uint16_t address);

    void writeRegister(uint16_t address, uint8_t value);

    uint8_t readStatus();

    void writeControl(uint8_t value);

    void writeFrameCounter(uint8_t value);
};


#endif //NES_EMU_APU_H
