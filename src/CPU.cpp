#include "CPU.h"

#include <format>
#include <print>
#include <string>

#include "Ram.h"

const CPU::Instruction CPU::table[256] = {
    &CPU::brk, &CPU::ora, &CPU::kil, &CPU::slo, &CPU::nop, &CPU::ora, &CPU::asl, &CPU::slo,
    &CPU::php, &CPU::ora, &CPU::asl, &CPU::anc, &CPU::nop, &CPU::ora, &CPU::asl, &CPU::slo,
    &CPU::bpl, &CPU::ora, &CPU::kil, &CPU::slo, &CPU::nop, &CPU::ora, &CPU::asl, &CPU::slo,
    &CPU::clc, &CPU::ora, &CPU::nop, &CPU::slo, &CPU::nop, &CPU::ora, &CPU::asl, &CPU::slo,
    &CPU::jsr, &CPU::AND, &CPU::kil, &CPU::rla, &CPU::bit, &CPU::AND, &CPU::rol, &CPU::rla,
    &CPU::plp, &CPU::AND, &CPU::rol, &CPU::anc, &CPU::bit, &CPU::AND, &CPU::rol, &CPU::rla,
    &CPU::bmi, &CPU::AND, &CPU::kil, &CPU::rla, &CPU::nop, &CPU::AND, &CPU::rol, &CPU::rla,
    &CPU::sec, &CPU::AND, &CPU::nop, &CPU::rla, &CPU::nop, &CPU::AND, &CPU::rol, &CPU::rla,
    &CPU::rti, &CPU::eor, &CPU::kil, &CPU::sre, &CPU::nop, &CPU::eor, &CPU::lsr, &CPU::sre,
    &CPU::pha, &CPU::eor, &CPU::lsr, &CPU::alr, &CPU::jmp, &CPU::eor, &CPU::lsr, &CPU::sre,
    &CPU::bvc, &CPU::eor, &CPU::kil, &CPU::sre, &CPU::nop, &CPU::eor, &CPU::lsr, &CPU::sre,
    &CPU::cli, &CPU::eor, &CPU::nop, &CPU::sre, &CPU::nop, &CPU::eor, &CPU::lsr, &CPU::sre,
    &CPU::rts, &CPU::adc, &CPU::kil, &CPU::rra, &CPU::nop, &CPU::adc, &CPU::ror, &CPU::rra,
    &CPU::pla, &CPU::adc, &CPU::ror, &CPU::arr, &CPU::jmp, &CPU::adc, &CPU::ror, &CPU::rra,
    &CPU::bvs, &CPU::adc, &CPU::kil, &CPU::rra, &CPU::nop, &CPU::adc, &CPU::ror, &CPU::rra,
    &CPU::sei, &CPU::adc, &CPU::nop, &CPU::rra, &CPU::nop, &CPU::adc, &CPU::ror, &CPU::rra,
    &CPU::nop, &CPU::sta, &CPU::nop, &CPU::sax, &CPU::sty, &CPU::sta, &CPU::stx, &CPU::sax,
    &CPU::dey, &CPU::nop, &CPU::txa, &CPU::xaa, &CPU::sty, &CPU::sta, &CPU::stx, &CPU::sax,
    &CPU::bcc, &CPU::sta, &CPU::kil, &CPU::ahx, &CPU::sty, &CPU::sta, &CPU::stx, &CPU::sax,
    &CPU::tya, &CPU::sta, &CPU::txs, &CPU::tas, &CPU::shy, &CPU::sta, &CPU::shx, &CPU::ahx,
    &CPU::ldy, &CPU::lda, &CPU::ldx, &CPU::lax, &CPU::ldy, &CPU::lda, &CPU::ldx, &CPU::lax,
    &CPU::tay, &CPU::lda, &CPU::tax, &CPU::lax, &CPU::ldy, &CPU::lda, &CPU::ldx, &CPU::lax,
    &CPU::bcs, &CPU::lda, &CPU::kil, &CPU::lax, &CPU::ldy, &CPU::lda, &CPU::ldx, &CPU::lax,
    &CPU::clv, &CPU::lda, &CPU::tsx, &CPU::las, &CPU::ldy, &CPU::lda, &CPU::ldx, &CPU::lax,
    &CPU::cpy, &CPU::cmp, &CPU::nop, &CPU::dcp, &CPU::cpy, &CPU::cmp, &CPU::dec, &CPU::dcp,
    &CPU::iny, &CPU::cmp, &CPU::dex, &CPU::axs, &CPU::cpy, &CPU::cmp, &CPU::dec, &CPU::dcp,
    &CPU::bne, &CPU::cmp, &CPU::kil, &CPU::dcp, &CPU::nop, &CPU::cmp, &CPU::dec, &CPU::dcp,
    &CPU::cld, &CPU::cmp, &CPU::nop, &CPU::dcp, &CPU::nop, &CPU::cmp, &CPU::dec, &CPU::dcp,
    &CPU::cpx, &CPU::sbc, &CPU::nop, &CPU::isc, &CPU::cpx, &CPU::sbc, &CPU::inc, &CPU::isc,
    &CPU::inx, &CPU::sbc, &CPU::nop, &CPU::sbc, &CPU::cpx, &CPU::sbc, &CPU::inc, &CPU::isc,
    &CPU::beq, &CPU::sbc, &CPU::kil, &CPU::isc, &CPU::nop, &CPU::sbc, &CPU::inc, &CPU::isc,
    &CPU::sed, &CPU::sbc, &CPU::nop, &CPU::isc, &CPU::nop, &CPU::sbc, &CPU::inc, &CPU::isc,
};

CPU::CPU(Ram *ram) {
    this->ram = ram;
    Reset();
}

uint8_t CPU::Read(const uint16_t address) {
    return ram->read(address);
}

void CPU::Write(const uint16_t address, const uint8_t value) {
    ram->write(address, value);
}

// Reset resets the CPU to its initial powerup state
void CPU::Reset() {
    PC = Read16(0xFFFC);
    SP = 0xFD;
    SetFlags(0x24);
}

// PrintInstruction prints the current CPU state
void CPU::PrintInstruction() {
    const uint8_t opcode = Read(PC);
    const uint8_t bytes = instructionSizes[opcode];
    const char *name = instructionNames[opcode];
    const std::string w0 = std::format("{:02X}", Read(PC + 0));
    std::string w1 = std::format("{:02X}", Read(PC + 1));
    std::string w2 = std::format("{:02X}", Read(PC + 2));
    if (bytes < 2) {
        w1 = "  ";
    }
    if (bytes < 3) {
        w2 = "  ";
    }
    std::print(
        "{:4X}  {} {} {}  {} {:28}"
        "A:{:02X} X:{:02X} Y:{:02X} P:{:02X} SP:{:02X} CYC:{:3}\n",
        PC, w0, w1, w2, name, "",
        A, X, Y, Flags(), SP, (Cycles * 3) % 341);
}

// pagesDiffer returns true if the two addresses reference different pages
bool CPU::pagesDiffer(const uint16_t a, const uint16_t b) {
    return (a & 0xFF00) != (b & 0xFF00);
}

// addBranchCycles adds a cycle for taking a branch and adds another cycle
// if the branch jumps to a new page
void CPU::addBranchCycles(const stepInfo &info) {
    Cycles++;
    if (pagesDiffer(info.pc, info.address)) {
        Cycles++;
    }
}

void CPU::compare(const uint8_t a, const uint8_t b) {
    setZN(a - b);
    if (a >= b) {
        C = 1;
    } else {
        C = 0;
    }
}

// Read16 reads two bytes using Read to return a double-word value
uint16_t CPU::Read16(const uint16_t address) {
    const uint16_t lo = Read(address);
    const uint16_t hi = Read(address + 1);
    return hi << 8 | lo;
}

// read16bug emulates a 6502 bug that caused the low byte to wrap without
// incrementing the high byte
uint16_t CPU::read16bug(const uint16_t address) {
    const uint16_t a = address;
    const uint16_t b = (a & 0xFF00) | static_cast<uint8_t>(a + 1);
    const uint16_t lo = Read(a);
    const uint16_t hi = Read(b);
    return hi << 8 | lo;
}

// push pushes a byte onto the stack
void CPU::push(const uint8_t value) {
    Write(0x100 | SP, value);
    SP--;
}

// pull pops a byte from the stack
uint8_t CPU::pull() {
    SP++;
    return Read(0x100 | SP);
}

// push16 pushes two bytes onto the stack
void CPU::push16(const uint16_t value) {
    const uint8_t hi = value >> 8;
    const uint8_t lo = value & 0xFF;
    push(hi);
    push(lo);
}

// pull16 pops two bytes from the stack
uint16_t CPU::pull16() {
    const uint16_t lo = pull();
    const uint16_t hi = pull();
    return hi << 8 | lo;
}

// Flags returns the processor status flags
uint8_t CPU::Flags() const {
    uint8_t flags = 0;
    flags |= C << 0;
    flags |= Z << 1;
    flags |= I << 2;
    flags |= D << 3;
    flags |= B << 4;
    flags |= U << 5;
    flags |= V << 6;
    flags |= N << 7;
    return flags;
}

// SetFlags sets the processor status flags
void CPU::SetFlags(const uint8_t flags) {
    C = (flags >> 0) & 1;
    Z = (flags >> 1) & 1;
    I = (flags >> 2) & 1;
    D = (flags >> 3) & 1;
    B = (flags >> 4) & 1;
    U = (flags >> 5) & 1;
    V = (flags >> 6) & 1;
    N = (flags >> 7) & 1;
}

// setZ sets the zero flag if the argument is zero
void CPU::setZ(const uint8_t value) {
    if (value == 0) {
        Z = 1;
    } else {
        Z = 0;
    }
}

// setN sets the negative flag if the argument is negative (high bit is set)
void CPU::setN(const uint8_t value) {
    if ((value & 0x80) != 0) {
        N = 1;
    } else {
        N = 0;
    }
}

// setZN sets the zero flag and the negative flag
void CPU::setZN(const uint8_t value) {
    setZ(value);
    setN(value);
}

// triggerNMI causes a non-maskable interrupt to occur on the next cycle
void CPU::triggerNMI() {
    interrupt = interruptNMI;
}

// triggerIRQ causes an IRQ interrupt to occur on the next cycle
void CPU::triggerIRQ() {
    if (I == 0) {
        interrupt = interruptIRQ;
    }
}

// step executes a single CPU instruction
int CPU::step() {
    if (stall > 0) {
        stall--;
        return 1;
    }

    const uint64_t cycles = Cycles;

    switch (interrupt) {
        case interruptNMI:
            nmi();
            break;
        case interruptIRQ:
            irq();
            break;
    }
    interrupt = interruptNone;

    const uint8_t opcode = Read(PC);
    const uint8_t mode = instructionModes[opcode];

    uint16_t address = 0;
    bool pageCrossed = false;
    switch (mode) {
        case modeAbsolute:
            address = Read16(PC + 1);
            break;
        case modeAbsoluteX:
            address = Read16(PC + 1) + X;
            pageCrossed = pagesDiffer(address - X, address);
            break;
        case modeAbsoluteY:
            address = Read16(PC + 1) + Y;
            pageCrossed = pagesDiffer(address - Y, address);
            break;
        case modeAccumulator:
            address = 0;
            break;
        case modeImmediate:
            address = PC + 1;
            break;
        case modeImplied:
            address = 0;
            break;
        case modeIndexedIndirect:
            // Go adds these as bytes, so the sum wraps within the zero page
            address = read16bug(static_cast<uint8_t>(Read(PC + 1) + X));
            break;
        case modeIndirect:
            address = read16bug(Read16(PC + 1));
            break;
        case modeIndirectIndexed:
            address = read16bug(Read(PC + 1)) + Y;
            pageCrossed = pagesDiffer(address - Y, address);
            break;
        case modeRelative: {
            const uint16_t offset = Read(PC + 1);
            if (offset < 0x80) {
                address = PC + 2 + offset;
            } else {
                address = PC + 2 + offset - 0x100;
            }
            break;
        }
        case modeZeroPage:
            address = Read(PC + 1);
            break;
        case modeZeroPageX:
            address = (Read(PC + 1) + X) & 0xFF;
            break;
        case modeZeroPageY:
            address = (Read(PC + 1) + Y) & 0xFF;
            break;
    }

    PC += instructionSizes[opcode];
    Cycles += instructionCycles[opcode];
    if (pageCrossed) {
        Cycles += instructionPageCycles[opcode];
    }
    const stepInfo info{address, PC, mode};
    (this->*table[opcode])(info);

    return static_cast<int>(Cycles - cycles);
}

// NMI - Non-Maskable Interrupt
void CPU::nmi() {
    push16(PC);
    php({});
    PC = Read16(0xFFFA);
    I = 1;
    Cycles += 7;
}

// IRQ - IRQ Interrupt
void CPU::irq() {
    push16(PC);
    php({});
    PC = Read16(0xFFFE);
    I = 1;
    Cycles += 7;
}

// ADC - Add with Carry
void CPU::adc(const stepInfo &info) {
    const uint8_t a = A;
    const uint8_t b = Read(info.address);
    const uint8_t c = C;
    A = a + b + c;
    setZN(A);
    if (int(a) + int(b) + int(c) > 0xFF) {
        C = 1;
    } else {
        C = 0;
    }
    if (((a ^ b) & 0x80) == 0 && ((a ^ A) & 0x80) != 0) {
        V = 1;
    } else {
        V = 0;
    }
}

// AND - Logical AND
void CPU::AND(const stepInfo &info) {
    A = A & Read(info.address);
    setZN(A);
}

// ASL - Arithmetic Shift Left
void CPU::asl(const stepInfo &info) {
    if (info.mode == modeAccumulator) {
        C = (A >> 7) & 1;
        A <<= 1;
        setZN(A);
    } else {
        uint8_t value = Read(info.address);
        C = (value >> 7) & 1;
        value <<= 1;
        Write(info.address, value);
        setZN(value);
    }
}

// BCC - Branch if Carry Clear
void CPU::bcc(const stepInfo &info) {
    if (C == 0) {
        PC = info.address;
        addBranchCycles(info);
    }
}

// BCS - Branch if Carry Set
void CPU::bcs(const stepInfo &info) {
    if (C != 0) {
        PC = info.address;
        addBranchCycles(info);
    }
}

// BEQ - Branch if Equal
void CPU::beq(const stepInfo &info) {
    if (Z != 0) {
        PC = info.address;
        addBranchCycles(info);
    }
}

// BIT - Bit Test
void CPU::bit(const stepInfo &info) {
    const uint8_t value = Read(info.address);
    V = (value >> 6) & 1;
    setZ(value & A);
    setN(value);
}

// BMI - Branch if Minus
void CPU::bmi(const stepInfo &info) {
    if (N != 0) {
        PC = info.address;
        addBranchCycles(info);
    }
}

// BNE - Branch if Not Equal
void CPU::bne(const stepInfo &info) {
    if (Z == 0) {
        PC = info.address;
        addBranchCycles(info);
    }
}

// BPL - Branch if Positive
void CPU::bpl(const stepInfo &info) {
    if (N == 0) {
        PC = info.address;
        addBranchCycles(info);
    }
}

// BRK - Force Interrupt
void CPU::brk(const stepInfo &info) {
    push16(PC);
    php(info);
    sei(info);
    PC = Read16(0xFFFE);
}

// BVC - Branch if Overflow Clear
void CPU::bvc(const stepInfo &info) {
    if (V == 0) {
        PC = info.address;
        addBranchCycles(info);
    }
}

// BVS - Branch if Overflow Set
void CPU::bvs(const stepInfo &info) {
    if (V != 0) {
        PC = info.address;
        addBranchCycles(info);
    }
}

// CLC - Clear Carry Flag
void CPU::clc(const stepInfo &info) {
    C = 0;
}

// CLD - Clear Decimal Mode
void CPU::cld(const stepInfo &info) {
    D = 0;
}

// CLI - Clear Interrupt Disable
void CPU::cli(const stepInfo &info) {
    I = 0;
}

// CLV - Clear Overflow Flag
void CPU::clv(const stepInfo &info) {
    V = 0;
}

// CMP - Compare
void CPU::cmp(const stepInfo &info) {
    const uint8_t value = Read(info.address);
    compare(A, value);
}

// CPX - Compare X Register
void CPU::cpx(const stepInfo &info) {
    const uint8_t value = Read(info.address);
    compare(X, value);
}

// CPY - Compare Y Register
void CPU::cpy(const stepInfo &info) {
    const uint8_t value = Read(info.address);
    compare(Y, value);
}

// DEC - Decrement Memory
void CPU::dec(const stepInfo &info) {
    const uint8_t value = Read(info.address) - 1;
    Write(info.address, value);
    setZN(value);
}

// DEX - Decrement X Register
void CPU::dex(const stepInfo &info) {
    X--;
    setZN(X);
}

// DEY - Decrement Y Register
void CPU::dey(const stepInfo &info) {
    Y--;
    setZN(Y);
}

// EOR - Exclusive OR
void CPU::eor(const stepInfo &info) {
    A = A ^ Read(info.address);
    setZN(A);
}

// INC - Increment Memory
void CPU::inc(const stepInfo &info) {
    const uint8_t value = Read(info.address) + 1;
    Write(info.address, value);
    setZN(value);
}

// INX - Increment X Register
void CPU::inx(const stepInfo &info) {
    X++;
    setZN(X);
}

// INY - Increment Y Register
void CPU::iny(const stepInfo &info) {
    Y++;
    setZN(Y);
}

// JMP - Jump
void CPU::jmp(const stepInfo &info) {
    PC = info.address;
}

// JSR - Jump to Subroutine
void CPU::jsr(const stepInfo &info) {
    push16(PC - 1);
    PC = info.address;
}

// LDA - Load Accumulator
void CPU::lda(const stepInfo &info) {
    A = Read(info.address);
    setZN(A);
}

// LDX - Load X Register
void CPU::ldx(const stepInfo &info) {
    X = Read(info.address);
    setZN(X);
}

// LDY - Load Y Register
void CPU::ldy(const stepInfo &info) {
    Y = Read(info.address);
    setZN(Y);
}

// LSR - Logical Shift Right
void CPU::lsr(const stepInfo &info) {
    if (info.mode == modeAccumulator) {
        C = A & 1;
        A >>= 1;
        setZN(A);
    } else {
        uint8_t value = Read(info.address);
        C = value & 1;
        value >>= 1;
        Write(info.address, value);
        setZN(value);
    }
}

// NOP - No Operation
void CPU::nop(const stepInfo &info) {
}

// ORA - Logical Inclusive OR
void CPU::ora(const stepInfo &info) {
    A = A | Read(info.address);
    setZN(A);
}

// PHA - Push Accumulator
void CPU::pha(const stepInfo &info) {
    push(A);
}

// PHP - Push Processor Status
void CPU::php(const stepInfo &info) {
    push(Flags() | 0x10);
}

// PLA - Pull Accumulator
void CPU::pla(const stepInfo &info) {
    A = pull();
    setZN(A);
}

// PLP - Pull Processor Status
void CPU::plp(const stepInfo &info) {
    SetFlags((pull() & 0xEF) | 0x20);
}

// ROL - Rotate Left
void CPU::rol(const stepInfo &info) {
    if (info.mode == modeAccumulator) {
        const uint8_t c = C;
        C = (A >> 7) & 1;
        A = (A << 1) | c;
        setZN(A);
    } else {
        const uint8_t c = C;
        uint8_t value = Read(info.address);
        C = (value >> 7) & 1;
        value = (value << 1) | c;
        Write(info.address, value);
        setZN(value);
    }
}

// ROR - Rotate Right
void CPU::ror(const stepInfo &info) {
    if (info.mode == modeAccumulator) {
        const uint8_t c = C;
        C = A & 1;
        A = (A >> 1) | (c << 7);
        setZN(A);
    } else {
        const uint8_t c = C;
        uint8_t value = Read(info.address);
        C = value & 1;
        value = (value >> 1) | (c << 7);
        Write(info.address, value);
        setZN(value);
    }
}

// RTI - Return from Interrupt
void CPU::rti(const stepInfo &info) {
    SetFlags((pull() & 0xEF) | 0x20);
    PC = pull16();
}

// RTS - Return from Subroutine
void CPU::rts(const stepInfo &info) {
    PC = pull16() + 1;
}

// SBC - Subtract with Carry
void CPU::sbc(const stepInfo &info) {
    const uint8_t a = A;
    const uint8_t b = Read(info.address);
    const uint8_t c = C;
    A = a - b - (1 - c);
    setZN(A);
    if (int(a) - int(b) - int(1 - c) >= 0) {
        C = 1;
    } else {
        C = 0;
    }
    if (((a ^ b) & 0x80) != 0 && ((a ^ A) & 0x80) != 0) {
        V = 1;
    } else {
        V = 0;
    }
}

// SEC - Set Carry Flag
void CPU::sec(const stepInfo &info) {
    C = 1;
}

// SED - Set Decimal Flag
void CPU::sed(const stepInfo &info) {
    D = 1;
}

// SEI - Set Interrupt Disable
void CPU::sei(const stepInfo &info) {
    I = 1;
}

// STA - Store Accumulator
void CPU::sta(const stepInfo &info) {
    Write(info.address, A);
}

// STX - Store X Register
void CPU::stx(const stepInfo &info) {
    Write(info.address, X);
}

// STY - Store Y Register
void CPU::sty(const stepInfo &info) {
    Write(info.address, Y);
}

// TAX - Transfer Accumulator to X
void CPU::tax(const stepInfo &info) {
    X = A;
    setZN(X);
}

// TAY - Transfer Accumulator to Y
void CPU::tay(const stepInfo &info) {
    Y = A;
    setZN(Y);
}

// TSX - Transfer Stack Pointer to X
void CPU::tsx(const stepInfo &info) {
    X = SP;
    setZN(X);
}

// TXA - Transfer X to Accumulator
void CPU::txa(const stepInfo &info) {
    A = X;
    setZN(A);
}

// TXS - Transfer X to Stack Pointer
void CPU::txs(const stepInfo &info) {
    SP = X;
}

// TYA - Transfer Y to Accumulator
void CPU::tya(const stepInfo &info) {
    A = Y;
    setZN(A);
}

// illegal opcodes below

void CPU::ahx(const stepInfo &info) {
}

void CPU::alr(const stepInfo &info) {
}

void CPU::anc(const stepInfo &info) {
}

void CPU::arr(const stepInfo &info) {
}

void CPU::axs(const stepInfo &info) {
}

void CPU::dcp(const stepInfo &info) {
}

void CPU::isc(const stepInfo &info) {
}

void CPU::kil(const stepInfo &info) {
}

void CPU::las(const stepInfo &info) {
}

void CPU::lax(const stepInfo &info) {
}

void CPU::rla(const stepInfo &info) {
}

void CPU::rra(const stepInfo &info) {
}

void CPU::sax(const stepInfo &info) {
}

void CPU::shx(const stepInfo &info) {
}

void CPU::shy(const stepInfo &info) {
}

void CPU::slo(const stepInfo &info) {
}

void CPU::sre(const stepInfo &info) {
}

void CPU::tas(const stepInfo &info) {
}

void CPU::xaa(const stepInfo &info) {
}
