#ifndef CPPTEST_CPU_H
#define CPPTEST_CPU_H
#include <cstdint>

class Ram;

constexpr int CPUFrequency = 1789773;

class CPU {
public:
    // interrupt types
    enum : uint8_t {
        interruptNone = 1,
        interruptNMI,
        interruptIRQ
    };

    // addressing modes, numbered from 1 like Go's iota so they match instructionModes
    enum : uint8_t {
        modeAbsolute = 1,
        modeAbsoluteX,
        modeAbsoluteY,
        modeAccumulator,
        modeImmediate,
        modeImplied,
        modeIndexedIndirect,
        modeIndirect,
        modeIndirectIndexed,
        modeRelative,
        modeZeroPage,
        modeZeroPageX,
        modeZeroPageY
    };

    // instructionModes indicates the addressing mode for each instruction
    static constexpr uint8_t instructionModes[256] = {
        6, 7, 6, 7, 11, 11, 11, 11, 6, 5, 4, 5, 1, 1, 1, 1,
        10, 9, 6, 9, 12, 12, 12, 12, 6, 3, 6, 3, 2, 2, 2, 2,
        1, 7, 6, 7, 11, 11, 11, 11, 6, 5, 4, 5, 1, 1, 1, 1,
        10, 9, 6, 9, 12, 12, 12, 12, 6, 3, 6, 3, 2, 2, 2, 2,
        6, 7, 6, 7, 11, 11, 11, 11, 6, 5, 4, 5, 1, 1, 1, 1,
        10, 9, 6, 9, 12, 12, 12, 12, 6, 3, 6, 3, 2, 2, 2, 2,
        6, 7, 6, 7, 11, 11, 11, 11, 6, 5, 4, 5, 8, 1, 1, 1,
        10, 9, 6, 9, 12, 12, 12, 12, 6, 3, 6, 3, 2, 2, 2, 2,
        5, 7, 5, 7, 11, 11, 11, 11, 6, 5, 6, 5, 1, 1, 1, 1,
        10, 9, 6, 9, 12, 12, 13, 13, 6, 3, 6, 3, 2, 2, 3, 3,
        5, 7, 5, 7, 11, 11, 11, 11, 6, 5, 6, 5, 1, 1, 1, 1,
        10, 9, 6, 9, 12, 12, 13, 13, 6, 3, 6, 3, 2, 2, 3, 3,
        5, 7, 5, 7, 11, 11, 11, 11, 6, 5, 6, 5, 1, 1, 1, 1,
        10, 9, 6, 9, 12, 12, 12, 12, 6, 3, 6, 3, 2, 2, 2, 2,
        5, 7, 5, 7, 11, 11, 11, 11, 6, 5, 6, 5, 1, 1, 1, 1,
        10, 9, 6, 9, 12, 12, 12, 12, 6, 3, 6, 3, 2, 2, 2, 2,
    };

    // instructionSizes indicates the size of each instruction in bytes
    static constexpr uint8_t instructionSizes[256] = {
        2, 2, 0, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
        2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 3, 3, 3, 0,
        3, 2, 0, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
        2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 3, 3, 3, 0,
        1, 2, 0, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
        2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 3, 3, 3, 0,
        1, 2, 0, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
        2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 3, 3, 3, 0,
        2, 2, 0, 0, 2, 2, 2, 0, 1, 0, 1, 0, 3, 3, 3, 0,
        2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 0, 3, 0, 0,
        2, 2, 2, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
        2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 3, 3, 3, 0,
        2, 2, 0, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
        2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 3, 3, 3, 0,
        2, 2, 0, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
        2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 3, 3, 3, 0,
    };

    // instructionCycles indicates the number of cycles used by each instruction,
    // not including conditional cycles
    static constexpr uint8_t instructionCycles[256] = {
        7, 6, 2, 8, 3, 3, 5, 5, 3, 2, 2, 2, 4, 4, 6, 6,
        2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
        6, 6, 2, 8, 3, 3, 5, 5, 4, 2, 2, 2, 4, 4, 6, 6,
        2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
        6, 6, 2, 8, 3, 3, 5, 5, 3, 2, 2, 2, 3, 4, 6, 6,
        2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
        6, 6, 2, 8, 3, 3, 5, 5, 4, 2, 2, 2, 5, 4, 6, 6,
        2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
        2, 6, 2, 6, 3, 3, 3, 3, 2, 2, 2, 2, 4, 4, 4, 4,
        2, 6, 2, 6, 4, 4, 4, 4, 2, 5, 2, 5, 5, 5, 5, 5,
        2, 6, 2, 6, 3, 3, 3, 3, 2, 2, 2, 2, 4, 4, 4, 4,
        2, 5, 2, 5, 4, 4, 4, 4, 2, 4, 2, 4, 4, 4, 4, 4,
        2, 6, 2, 8, 3, 3, 5, 5, 2, 2, 2, 2, 4, 4, 6, 6,
        2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
        2, 6, 2, 8, 3, 3, 5, 5, 2, 2, 2, 2, 4, 4, 6, 6,
        2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
    };

    // instructionPageCycles indicates the number of cycles used by each
    // instruction when a page is crossed
    static constexpr uint8_t instructionPageCycles[256] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0,
    };

    // instructionNames indicates the name of each instruction
    static constexpr const char *instructionNames[256] = {
        "BRK", "ORA", "KIL", "SLO", "NOP", "ORA", "ASL", "SLO",
        "PHP", "ORA", "ASL", "ANC", "NOP", "ORA", "ASL", "SLO",
        "BPL", "ORA", "KIL", "SLO", "NOP", "ORA", "ASL", "SLO",
        "CLC", "ORA", "NOP", "SLO", "NOP", "ORA", "ASL", "SLO",
        "JSR", "AND", "KIL", "RLA", "BIT", "AND", "ROL", "RLA",
        "PLP", "AND", "ROL", "ANC", "BIT", "AND", "ROL", "RLA",
        "BMI", "AND", "KIL", "RLA", "NOP", "AND", "ROL", "RLA",
        "SEC", "AND", "NOP", "RLA", "NOP", "AND", "ROL", "RLA",
        "RTI", "EOR", "KIL", "SRE", "NOP", "EOR", "LSR", "SRE",
        "PHA", "EOR", "LSR", "ALR", "JMP", "EOR", "LSR", "SRE",
        "BVC", "EOR", "KIL", "SRE", "NOP", "EOR", "LSR", "SRE",
        "CLI", "EOR", "NOP", "SRE", "NOP", "EOR", "LSR", "SRE",
        "RTS", "ADC", "KIL", "RRA", "NOP", "ADC", "ROR", "RRA",
        "PLA", "ADC", "ROR", "ARR", "JMP", "ADC", "ROR", "RRA",
        "BVS", "ADC", "KIL", "RRA", "NOP", "ADC", "ROR", "RRA",
        "SEI", "ADC", "NOP", "RRA", "NOP", "ADC", "ROR", "RRA",
        "NOP", "STA", "NOP", "SAX", "STY", "STA", "STX", "SAX",
        "DEY", "NOP", "TXA", "XAA", "STY", "STA", "STX", "SAX",
        "BCC", "STA", "KIL", "AHX", "STY", "STA", "STX", "SAX",
        "TYA", "STA", "TXS", "TAS", "SHY", "STA", "SHX", "AHX",
        "LDY", "LDA", "LDX", "LAX", "LDY", "LDA", "LDX", "LAX",
        "TAY", "LDA", "TAX", "LAX", "LDY", "LDA", "LDX", "LAX",
        "BCS", "LDA", "KIL", "LAX", "LDY", "LDA", "LDX", "LAX",
        "CLV", "LDA", "TSX", "LAS", "LDY", "LDA", "LDX", "LAX",
        "CPY", "CMP", "NOP", "DCP", "CPY", "CMP", "DEC", "DCP",
        "INY", "CMP", "DEX", "AXS", "CPY", "CMP", "DEC", "DCP",
        "BNE", "CMP", "KIL", "DCP", "NOP", "CMP", "DEC", "DCP",
        "CLD", "CMP", "NOP", "DCP", "NOP", "CMP", "DEC", "DCP",
        "CPX", "SBC", "NOP", "ISC", "CPX", "SBC", "INC", "ISC",
        "INX", "SBC", "NOP", "SBC", "CPX", "SBC", "INC", "ISC",
        "BEQ", "SBC", "KIL", "ISC", "NOP", "SBC", "INC", "ISC",
        "SED", "SBC", "NOP", "ISC", "NOP", "SBC", "INC", "ISC",
    };

    Ram *ram{};                       // memory interface
    uint64_t Cycles{};                // number of cycles
    uint16_t PC{};                    // program counter
    uint8_t SP{};                     // stack pointer
    uint8_t A{};                      // accumulator
    uint8_t X{};                      // x register
    uint8_t Y{};                      // y register
    uint8_t C{};                      // carry flag
    uint8_t Z{};                      // zero flag
    uint8_t I{};                      // interrupt disable flag
    uint8_t D{};                      // decimal mode flag
    uint8_t B{};                      // break command flag
    uint8_t U{};                      // unused flag
    uint8_t V{};                      // overflow flag
    uint8_t N{};                      // negative flag
    uint8_t interrupt{interruptNone}; // interrupt type to perform
    int stall{};                      // number of cycles to stall

    // stepInfo contains information that the instruction functions use
    struct stepInfo {
        uint16_t address;
        uint16_t pc;
        uint8_t mode;
    };

    using Instruction = void (CPU::*)(const stepInfo &info);

    // table holds the function for each instruction
    static const Instruction table[256];

    explicit CPU(Ram *ram);

    uint8_t Read(uint16_t address);

    void Write(uint16_t address, uint8_t value);

    // Reset resets the CPU to its initial powerup state
    void Reset();

    // PrintInstruction prints the current CPU state
    void PrintInstruction();

    // pagesDiffer returns true if the two addresses reference different pages
    static bool pagesDiffer(uint16_t a, uint16_t b);

    // addBranchCycles adds a cycle for taking a branch and adds another cycle
    // if the branch jumps to a new page
    void addBranchCycles(const stepInfo &info);

    void compare(uint8_t a, uint8_t b);

    // Read16 reads two bytes using Read to return a double-word value
    uint16_t Read16(uint16_t address);

    // read16bug emulates a 6502 bug that caused the low byte to wrap without
    // incrementing the high byte
    uint16_t read16bug(uint16_t address);

    // push pushes a byte onto the stack
    void push(uint8_t value);

    // pull pops a byte from the stack
    uint8_t pull();

    // push16 pushes two bytes onto the stack
    void push16(uint16_t value);

    // pull16 pops two bytes from the stack
    uint16_t pull16();

    // Flags returns the processor status flags
    uint8_t Flags() const;

    // SetFlags sets the processor status flags
    void SetFlags(uint8_t flags);

    // setZ sets the zero flag if the argument is zero
    void setZ(uint8_t value);

    // setN sets the negative flag if the argument is negative (high bit is set)
    void setN(uint8_t value);

    // setZN sets the zero flag and the negative flag
    void setZN(uint8_t value);

    // triggerNMI causes a non-maskable interrupt to occur on the next cycle
    void triggerNMI();

    // triggerIRQ causes an IRQ interrupt to occur on the next cycle
    void triggerIRQ();

    // step executes a single CPU instruction
    int step();

    // NMI - Non-Maskable Interrupt
    void nmi();

    // IRQ - IRQ Interrupt
    void irq();

    // ADC - Add with Carry
    void adc(const stepInfo &info);

    // AND - Logical AND (and is a reserved word in C++)
    void AND(const stepInfo &info);

    // ASL - Arithmetic Shift Left
    void asl(const stepInfo &info);

    // BCC - Branch if Carry Clear
    void bcc(const stepInfo &info);

    // BCS - Branch if Carry Set
    void bcs(const stepInfo &info);

    // BEQ - Branch if Equal
    void beq(const stepInfo &info);

    // BIT - Bit Test
    void bit(const stepInfo &info);

    // BMI - Branch if Minus
    void bmi(const stepInfo &info);

    // BNE - Branch if Not Equal
    void bne(const stepInfo &info);

    // BPL - Branch if Positive
    void bpl(const stepInfo &info);

    // BRK - Force Interrupt
    void brk(const stepInfo &info);

    // BVC - Branch if Overflow Clear
    void bvc(const stepInfo &info);

    // BVS - Branch if Overflow Set
    void bvs(const stepInfo &info);

    // CLC - Clear Carry Flag
    void clc(const stepInfo &info);

    // CLD - Clear Decimal Mode
    void cld(const stepInfo &info);

    // CLI - Clear Interrupt Disable
    void cli(const stepInfo &info);

    // CLV - Clear Overflow Flag
    void clv(const stepInfo &info);

    // CMP - Compare
    void cmp(const stepInfo &info);

    // CPX - Compare X Register
    void cpx(const stepInfo &info);

    // CPY - Compare Y Register
    void cpy(const stepInfo &info);

    // DEC - Decrement Memory
    void dec(const stepInfo &info);

    // DEX - Decrement X Register
    void dex(const stepInfo &info);

    // DEY - Decrement Y Register
    void dey(const stepInfo &info);

    // EOR - Exclusive OR
    void eor(const stepInfo &info);

    // INC - Increment Memory
    void inc(const stepInfo &info);

    // INX - Increment X Register
    void inx(const stepInfo &info);

    // INY - Increment Y Register
    void iny(const stepInfo &info);

    // JMP - Jump
    void jmp(const stepInfo &info);

    // JSR - Jump to Subroutine
    void jsr(const stepInfo &info);

    // LDA - Load Accumulator
    void lda(const stepInfo &info);

    // LDX - Load X Register
    void ldx(const stepInfo &info);

    // LDY - Load Y Register
    void ldy(const stepInfo &info);

    // LSR - Logical Shift Right
    void lsr(const stepInfo &info);

    // NOP - No Operation
    void nop(const stepInfo &info);

    // ORA - Logical Inclusive OR
    void ora(const stepInfo &info);

    // PHA - Push Accumulator
    void pha(const stepInfo &info);

    // PHP - Push Processor Status
    void php(const stepInfo &info);

    // PLA - Pull Accumulator
    void pla(const stepInfo &info);

    // PLP - Pull Processor Status
    void plp(const stepInfo &info);

    // ROL - Rotate Left
    void rol(const stepInfo &info);

    // ROR - Rotate Right
    void ror(const stepInfo &info);

    // RTI - Return from Interrupt
    void rti(const stepInfo &info);

    // RTS - Return from Subroutine
    void rts(const stepInfo &info);

    // SBC - Subtract with Carry
    void sbc(const stepInfo &info);

    // SEC - Set Carry Flag
    void sec(const stepInfo &info);

    // SED - Set Decimal Flag
    void sed(const stepInfo &info);

    // SEI - Set Interrupt Disable
    void sei(const stepInfo &info);

    // STA - Store Accumulator
    void sta(const stepInfo &info);

    // STX - Store X Register
    void stx(const stepInfo &info);

    // STY - Store Y Register
    void sty(const stepInfo &info);

    // TAX - Transfer Accumulator to X
    void tax(const stepInfo &info);

    // TAY - Transfer Accumulator to Y
    void tay(const stepInfo &info);

    // TSX - Transfer Stack Pointer to X
    void tsx(const stepInfo &info);

    // TXA - Transfer X to Accumulator
    void txa(const stepInfo &info);

    // TXS - Transfer X to Stack Pointer
    void txs(const stepInfo &info);

    // TYA - Transfer Y to Accumulator
    void tya(const stepInfo &info);

    // illegal opcodes below

    void ahx(const stepInfo &info);

    void alr(const stepInfo &info);

    void anc(const stepInfo &info);

    void arr(const stepInfo &info);

    void axs(const stepInfo &info);

    void dcp(const stepInfo &info);

    void isc(const stepInfo &info);

    void kil(const stepInfo &info);

    void las(const stepInfo &info);

    void lax(const stepInfo &info);

    void rla(const stepInfo &info);

    void rra(const stepInfo &info);

    void sax(const stepInfo &info);

    void shx(const stepInfo &info);

    void shy(const stepInfo &info);

    void slo(const stepInfo &info);

    void sre(const stepInfo &info);

    void tas(const stepInfo &info);

    void xaa(const stepInfo &info);
};

#endif  // CPPTEST_CPU_H
