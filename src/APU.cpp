#include "APU.h"

#include "Console.h"
#include "CPU.h"

// APU

APU::APU(Console *console) {
    this->console = console;
    this->noise.shiftRegister = 1;
    this->pulse1.channel = 1;
    this->pulse2.channel = 2;
    this->framePeriod = 4;
    this->dmc.cpu = console->cpu;
}

void APU::step() {
    const uint64_t cycle1 = this->cycle;
    this->cycle++;
    const uint64_t cycle2 = this->cycle;
    this->stepTimer();
    const auto f1 = static_cast<int64_t>(static_cast<double>(cycle1) / frameCounterRate);
    const auto f2 = static_cast<int64_t>(static_cast<double>(cycle2) / frameCounterRate);
    if (f1 != f2) {
        this->stepFrameCounter();
    }
    // Go divides by a zero sampleRate here; casting the infinite result to an
    // integer is undefined in C++, so skip samples until the rate is set
    if (this->sampleRate > 0) {
        const auto s1 = static_cast<int64_t>(static_cast<double>(cycle1) / this->sampleRate);
        const auto s2 = static_cast<int64_t>(static_cast<double>(cycle2) / this->sampleRate);
        if (s1 != s2) {
            this->sendSample();
        }
    }
}

void APU::sendSample() {
    const float sample = this->filterChain.Step(this->output());
    if (this->channel) {
        this->channel(sample);
    }
}

float APU::output() const {
    const auto p1 = this->pulse1.output();
    const auto p2 = this->pulse2.output();
    const auto t = this->triangle.output();
    const auto n = this->noise.output();
    const auto d = this->dmc.output();
    const auto pulseOut = pulseTable[p1 + p2];
    const auto tndOut = tndTable[3 * t + 2 * n + d];
    return pulseOut + tndOut;
}

// mode 0:    mode 1:       function
// ---------  -----------  -----------------------------
//  - - - f    - - - - -    IRQ (if bit 6 is clear)
//  - l - l    l - l - -    Length counter and sweep
//  e e e e    e e e e -    Envelope and linear counter
void APU::stepFrameCounter() {
    switch (this->framePeriod) {
        case 4:
            this->frameValue = (this->frameValue + 1) % 4;
            switch (this->frameValue) {
                case 0:
                case 2:
                    this->stepEnvelope();
                    break;
                case 1:
                    this->stepEnvelope();
                    this->stepSweep();
                    this->stepLength();
                    break;
                case 3:
                    this->stepEnvelope();
                    this->stepSweep();
                    this->stepLength();
                    this->fireIRQ();
                    break;
            }
            break;
        case 5:
            this->frameValue = (this->frameValue + 1) % 5;
            switch (this->frameValue) {
                case 0:
                case 2:
                    this->stepEnvelope();
                    break;
                case 1:
                case 3:
                    this->stepEnvelope();
                    this->stepSweep();
                    this->stepLength();
                    break;
            }
            break;
    }
}

void APU::stepTimer() {
    if (this->cycle % 2 == 0) {
        this->pulse1.stepTimer();
        this->pulse2.stepTimer();
        this->noise.stepTimer();
        this->dmc.stepTimer();
    }
    this->triangle.stepTimer();
}

void APU::stepEnvelope() {
    this->pulse1.stepEnvelope();
    this->pulse2.stepEnvelope();
    this->triangle.stepCounter();
    this->noise.stepEnvelope();
}

void APU::stepSweep() {
    this->pulse1.stepSweep();
    this->pulse2.stepSweep();
}

void APU::stepLength() {
    this->pulse1.stepLength();
    this->pulse2.stepLength();
    this->triangle.stepLength();
    this->noise.stepLength();
}

void APU::fireIRQ() const {
    if (this->frameIRQ) {
        this->console->cpu->triggerIRQ();
    }
}

uint8_t APU::readRegister(const uint16_t address) const {
    switch (address) {
        case 0x4015:
            return this->readStatus();
    }
    return 0;
}

void APU::writeRegister(const uint16_t address, const uint8_t value) {
    switch (address) {
        case 0x4000:
            this->pulse1.writeControl(value);
            break;
        case 0x4001:
            this->pulse1.writeSweep(value);
            break;
        case 0x4002:
            this->pulse1.writeTimerLow(value);
            break;
        case 0x4003:
            this->pulse1.writeTimerHigh(value);
            break;
        case 0x4004:
            this->pulse2.writeControl(value);
            break;
        case 0x4005:
            this->pulse2.writeSweep(value);
            break;
        case 0x4006:
            this->pulse2.writeTimerLow(value);
            break;
        case 0x4007:
            this->pulse2.writeTimerHigh(value);
            break;
        case 0x4008:
            this->triangle.writeControl(value);
            break;
        case 0x4009:
            // unused
            break;
        case 0x400A:
            this->triangle.writeTimerLow(value);
            break;
        case 0x400B:
            this->triangle.writeTimerHigh(value);
            break;
        case 0x400C:
            this->noise.writeControl(value);
            break;
        case 0x400D:
            // unused
            break;
        case 0x400E:
            this->noise.writePeriod(value);
            break;
        case 0x400F:
            this->noise.writeLength(value);
            break;
        case 0x4010:
            this->dmc.writeControl(value);
            break;
        case 0x4011:
            this->dmc.writeValue(value);
            break;
        case 0x4012:
            this->dmc.writeAddress(value);
            break;
        case 0x4013:
            this->dmc.writeLength(value);
            break;
        case 0x4015:
            this->writeControl(value);
            break;
        case 0x4017:
            this->writeFrameCounter(value);
            break;
        default:
            break;
    }
}

uint8_t APU::readStatus() const {
    uint8_t result = 0;
    if (this->pulse1.lengthValue > 0) {
        result |= 1;
    }
    if (this->pulse2.lengthValue > 0) {
        result |= 2;
    }
    if (this->triangle.lengthValue > 0) {
        result |= 4;
    }
    if (this->noise.lengthValue > 0) {
        result |= 8;
    }
    if (this->dmc.currentLength > 0) {
        result |= 16;
    }
    return result;
}

void APU::writeControl(const uint8_t value) {
    this->pulse1.enabled = (value & 1) == 1;
    this->pulse2.enabled = (value & 2) == 2;
    this->triangle.enabled = (value & 4) == 4;
    this->noise.enabled = (value & 8) == 8;
    this->dmc.enabled = (value & 16) == 16;
    if (!this->pulse1.enabled) {
        this->pulse1.lengthValue = 0;
    }
    if (!this->pulse2.enabled) {
        this->pulse2.lengthValue = 0;
    }
    if (!this->triangle.enabled) {
        this->triangle.lengthValue = 0;
    }
    if (!this->noise.enabled) {
        this->noise.lengthValue = 0;
    }
    if (!this->dmc.enabled) {
        this->dmc.currentLength = 0;
    } else {
        if (this->dmc.currentLength == 0) {
            this->dmc.restart();
        }
    }
}

void APU::writeFrameCounter(const uint8_t value) {
    this->framePeriod = 4 + ((value >> 7) & 1);
    this->frameIRQ = ((value >> 6) & 1) == 0;
    // this->frameValue = 0;
    if (this->framePeriod == 5) {
        this->stepEnvelope();
        this->stepSweep();
        this->stepLength();
    }
}

// Pulse

void Pulse::writeControl(const uint8_t value) {
    this->dutyMode = (value >> 6) & 3;
    this->lengthEnabled = ((value >> 5) & 1) == 0;
    this->envelopeLoop = ((value >> 5) & 1) == 1;
    this->envelopeEnabled = ((value >> 4) & 1) == 0;
    this->envelopePeriod = value & 15;
    this->constantVolume = value & 15;
    this->envelopeStart = true;
}

void Pulse::writeSweep(const uint8_t value) {
    this->sweepEnabled = ((value >> 7) & 1) == 1;
    this->sweepPeriod = ((value >> 4) & 7) + 1;
    this->sweepNegate = ((value >> 3) & 1) == 1;
    this->sweepShift = value & 7;
    this->sweepReload = true;
}

void Pulse::writeTimerLow(const uint8_t value) {
    this->timerPeriod = (this->timerPeriod & 0xFF00) | static_cast<uint16_t>(value);
}

void Pulse::writeTimerHigh(const uint8_t value) {
    this->lengthValue = lengthTable[value >> 3];
    this->timerPeriod = (this->timerPeriod & 0x00FF) | (static_cast<uint16_t>(value & 7) << 8);
    this->envelopeStart = true;
    this->dutyValue = 0;
}

void Pulse::stepTimer() {
    if (this->timerValue == 0) {
        this->timerValue = this->timerPeriod;
        this->dutyValue = (this->dutyValue + 1) % 8;
    } else {
        this->timerValue--;
    }
}

void Pulse::stepEnvelope() {
    if (this->envelopeStart) {
        this->envelopeVolume = 15;
        this->envelopeValue = this->envelopePeriod;
        this->envelopeStart = false;
    } else if (this->envelopeValue > 0) {
        this->envelopeValue--;
    } else {
        if (this->envelopeVolume > 0) {
            this->envelopeVolume--;
        } else if (this->envelopeLoop) {
            this->envelopeVolume = 15;
        }
        this->envelopeValue = this->envelopePeriod;
    }
}

void Pulse::stepSweep() {
    if (this->sweepReload) {
        if (this->sweepEnabled && this->sweepValue == 0) {
            this->sweep();
        }
        this->sweepValue = this->sweepPeriod;
        this->sweepReload = false;
    } else if (this->sweepValue > 0) {
        this->sweepValue--;
    } else {
        if (this->sweepEnabled) {
            this->sweep();
        }
        this->sweepValue = this->sweepPeriod;
    }
}

void Pulse::stepLength() {
    if (this->lengthEnabled && this->lengthValue > 0) {
        this->lengthValue--;
    }
}

void Pulse::sweep() {
    const uint16_t delta = this->timerPeriod >> this->sweepShift;
    if (this->sweepNegate) {
        this->timerPeriod -= delta;
        if (this->channel == 1) {
            this->timerPeriod--;
        }
    } else {
        this->timerPeriod += delta;
    }
}

uint8_t Pulse::output() const {
    if (!this->enabled) {
        return 0;
    }
    if (this->lengthValue == 0) {
        return 0;
    }
    if (dutyTable[this->dutyMode][this->dutyValue] == 0) {
        return 0;
    }
    if (this->timerPeriod < 8 || this->timerPeriod > 0x7FF) {
        return 0;
    }
    // if (!this->sweepNegate && this->timerPeriod + (this->timerPeriod >> this->sweepShift) > 0x7FF) {
    //     return 0;
    // }
    if (this->envelopeEnabled) {
        return this->envelopeVolume;
    }
    return this->constantVolume;
}

// Triangle

void Triangle::writeControl(const uint8_t value) {
    this->lengthEnabled = ((value >> 7) & 1) == 0;
    this->counterPeriod = value & 0x7F;
}

void Triangle::writeTimerLow(const uint8_t value) {
    this->timerPeriod = (this->timerPeriod & 0xFF00) | static_cast<uint16_t>(value);
}

void Triangle::writeTimerHigh(const uint8_t value) {
    this->lengthValue = lengthTable[value >> 3];
    this->timerPeriod = (this->timerPeriod & 0x00FF) | (static_cast<uint16_t>(value & 7) << 8);
    this->timerValue = this->timerPeriod;
    this->counterReload = true;
}

void Triangle::stepTimer() {
    if (this->timerValue == 0) {
        this->timerValue = this->timerPeriod;
        if (this->lengthValue > 0 && this->counterValue > 0) {
            this->dutyValue = (this->dutyValue + 1) % 32;
        }
    } else {
        this->timerValue--;
    }
}

void Triangle::stepLength() {
    if (this->lengthEnabled && this->lengthValue > 0) {
        this->lengthValue--;
    }
}

void Triangle::stepCounter() {
    if (this->counterReload) {
        this->counterValue = this->counterPeriod;
    } else if (this->counterValue > 0) {
        this->counterValue--;
    }
    if (this->lengthEnabled) {
        this->counterReload = false;
    }
}

uint8_t Triangle::output() const {
    if (!this->enabled) {
        return 0;
    }
    if (this->timerPeriod < 3) {
        return 0;
    }
    if (this->lengthValue == 0) {
        return 0;
    }
    if (this->counterValue == 0) {
        return 0;
    }
    return triangleTable[this->dutyValue];
}

// Noise

void Noise::writeControl(const uint8_t value) {
    this->lengthEnabled = ((value >> 5) & 1) == 0;
    this->envelopeLoop = ((value >> 5) & 1) == 1;
    this->envelopeEnabled = ((value >> 4) & 1) == 0;
    this->envelopePeriod = value & 15;
    this->constantVolume = value & 15;
    this->envelopeStart = true;
}

void Noise::writePeriod(const uint8_t value) {
    this->mode = (value & 0x80) == 0x80;
    this->timerPeriod = noiseTable[value & 0x0F];
}

void Noise::writeLength(const uint8_t value) {
    this->lengthValue = lengthTable[value >> 3];
    this->envelopeStart = true;
}

void Noise::stepTimer() {
    if (this->timerValue == 0) {
        this->timerValue = this->timerPeriod;
        const uint8_t shift = this->mode ? 6 : 1;
        const auto b1 = this->shiftRegister & 1;
        const auto b2 = (this->shiftRegister >> shift) & 1;
        this->shiftRegister >>= 1;
        this->shiftRegister |= (b1 ^ b2) << 14;
    } else {
        this->timerValue--;
    }
}

void Noise::stepEnvelope() {
    if (this->envelopeStart) {
        this->envelopeVolume = 15;
        this->envelopeValue = this->envelopePeriod;
        this->envelopeStart = false;
    } else if (this->envelopeValue > 0) {
        this->envelopeValue--;
    } else {
        if (this->envelopeVolume > 0) {
            this->envelopeVolume--;
        } else if (this->envelopeLoop) {
            this->envelopeVolume = 15;
        }
        this->envelopeValue = this->envelopePeriod;
    }
}

void Noise::stepLength() {
    if (this->lengthEnabled && this->lengthValue > 0) {
        this->lengthValue--;
    }
}

uint8_t Noise::output() const {
    if (!this->enabled) {
        return 0;
    }
    if (this->lengthValue == 0) {
        return 0;
    }
    if ((this->shiftRegister & 1) == 1) {
        return 0;
    }
    if (this->envelopeEnabled) {
        return this->envelopeVolume;
    }
    return this->constantVolume;
}

// DMC

void DMC::writeControl(const uint8_t value) {
    this->irq = (value & 0x80) == 0x80;
    this->loop = (value & 0x40) == 0x40;
    this->tickPeriod = dmcTable[value & 0x0F];
}

void DMC::writeValue(const uint8_t value) {
    this->value = value & 0x7F;
}

void DMC::writeAddress(const uint8_t value) {
    // Sample address = %11AAAAAA.AA000000
    this->sampleAddress = 0xC000 | (static_cast<uint16_t>(value) << 6);
}

void DMC::writeLength(const uint8_t value) {
    // Sample length = %0000LLLL.LLLL0001
    this->sampleLength = (static_cast<uint16_t>(value) << 4) | 1;
}

void DMC::restart() {
    this->currentAddress = this->sampleAddress;
    this->currentLength = this->sampleLength;
}

void DMC::stepTimer() {
    if (!this->enabled) {
        return;
    }
    this->stepReader();
    if (this->tickValue == 0) {
        this->tickValue = this->tickPeriod;
        this->stepShifter();
    } else {
        this->tickValue--;
    }
}

void DMC::stepReader() {
    if (this->currentLength > 0 && this->bitCount == 0) {
        this->cpu->stall += 4;
        this->shiftRegister = this->cpu->Read(this->currentAddress);
        this->bitCount = 8;
        this->currentAddress++;
        if (this->currentAddress == 0) {
            this->currentAddress = 0x8000;
        }
        this->currentLength--;
        if (this->currentLength == 0 && this->loop) {
            this->restart();
        }
    }
}

void DMC::stepShifter() {
    if (this->bitCount == 0) {
        return;
    }
    if ((this->shiftRegister & 1) == 1) {
        if (this->value <= 125) {
            this->value += 2;
        }
    } else {
        if (this->value >= 2) {
            this->value -= 2;
        }
    }
    this->shiftRegister >>= 1;
    this->bitCount--;
}

uint8_t DMC::output() const {
    return this->value;
}
