#ifndef CPPTEST_CPU_H
#define CPPTEST_CPU_H
#include <string>

class Ram;

class CPU {
public:
    // interrupt types
    enum interupt_types {
        interruptNone,
        interruptNMI,
        interruptIRQ
    };

    // addressing modes
    enum addressing_mode {
        modeAbsolute,
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
    char instructionModes[256] = {
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
    char instructionSizes[256] = {
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
    char instructionCycles[256] = {
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
    char instructionPageCycles[256] = {
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
    std::string instructionNames[256] = {
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


    const int CPUFrequency = 1789773;

    Ram *ram;
    long Cycles{}; // number of cycles
    short PC{}; // program counter
    char SP{}; // stack pointer
    char A{}; // accumulator
    char X{}; // x register
    char Y{}; // y register
    char C{}; // carry flag
    char Z{}; // zero flag
    char I{}; // interrupt disable flag
    char D{}; // decimal mode flag
    char B{}; // break command flag
    char U{}; // unused flag
    char V{}; // overflow flag
    char N{}; // negative flag
    int stall{}; // number of cycles to stall

    // interrupt type to perform
    interupt_types interrupt{interruptNone};

    explicit CPU(Ram *pRam);

    short Read(short address);

    void Write(short address, char byte);

    // Reset resets the CPU to its initial powerup state
    void Reset();

    // Step executes a single CPU instruction
    int step();

    // pagesDiffer returns true if the two addresses reference different pages
    bool pagesDiffer(short a, short b);

    // stepInfo contains information that the instruction functions use
    struct stepInfo {
        short address;
        short pc;
        char mode;
    };

    // addBranchCycles adds a cycle for taking a branch and adds another cycle
    // if the branch jumps to a new page
    void addBranchCycles(stepInfo info);

    void compare(char a, char b);

    // Read16 reads two bytes using Read to return a double-word value
    short Read16(short address);

    // read16bug emulates a 6502 bug that caused the low byte to wrap without
    // incrementing the high byte
    short read16bug(short address);

    // push pushes a byte onto the stack
    void push(char value);

    // pull pops a byte from the stack
    char pull();

    // push16 pushes two bytes onto the stack
    void push16(short value);

    // pull16 pops two bytes from the stack
    short pull16();

    // Flags returns the processor status flags
    char Flags();

    // SetFlags sets the processor status flags
    void SetFlags(char flags);

    // setZ sets the zero flag if the argument is zero
    void setZ(char value);

    // setN sets the negative flag if the argument is negative (high bit is set)
    void setN(char value);

    // setZN sets the zero flag and the negative flag
    void setZN(char value);

    // triggerNMI causes a non-maskable interrupt to occur on the next cycle
    void triggerNMI();

    // triggerIRQ causes an IRQ interrupt to occur on the next cycle
    void triggerIRQ();

    // NMI - Non-Maskable Interrupt
    void nmi();

    // IRQ - IRQ Interrupt
    void irq();

    // ADC - Add with Carry
    void adc(stepInfo info);

    // AND - Logical AND
    void AND(stepInfo info);

    // ASL - Arithmetic Shift Left
    void asl(stepInfo info);

    // BCC - Branch if Carry Clear
    void bcc(stepInfo info);

    // BCS - Branch if Carry Set
    void bcs(stepInfo info);

    // BEQ - Branch if Equal
    void beq(stepInfo info);

    // BIT - Bit Test
    void bit(stepInfo info);

    // BMI - Branch if Minus
    void bmi(stepInfo info);

    // BNE - Branch if Not Equal
    void bne(stepInfo info);

    // BPL - Branch if Positive
    void bpl(stepInfo info);

    // BRK - Force Interrupt
    void brk(stepInfo info);

    // BVC - Branch if Overflow Clear
    void bvc(stepInfo info);

    // BVS - Branch if Overflow Set
    void bvs(stepInfo info);

    // CLC - Clear Carry Flag
    void clc(stepInfo info);

    // CLD - Clear Decimal Mode
    void cld(stepInfo info);

    // CLI - Clear Interrupt Disable
    void cli(stepInfo info);

    // CLV - Clear Overflow Flag
    void clv(stepInfo info);

    // CMP - Compare
    void cmp(stepInfo info);

    // CPX - Compare X Register
    void cpx(stepInfo info);

    // CPY - Compare Y Register
    void cpy(stepInfo info);

    // DEC - Decrement Memory
    void dec(stepInfo info);

    // DEX - Decrement X Register
    void dex(stepInfo info);

    // DEY - Decrement Y Register
    void dey(stepInfo info);

    // EOR - Exclusive OR
    void eor(stepInfo info);

    // INC - Increment Memory
    void inc(stepInfo info);

    // INX - Increment X Register
    void inx(stepInfo info);

    // INY - Increment Y Register
    void iny(stepInfo info);

    // JMP - Jump
    void jmp(stepInfo info);

    // JSR - Jump to Subroutine
    void jsr(stepInfo info);

    // LDA - Load Accumulator
    void lda(stepInfo info);

    // LDX - Load X Register
    void ldx(stepInfo info);

    // LDY - Load Y Register
    void ldy(stepInfo info);

    // LSR - Logical Shift Right
    void lsr(stepInfo info);

    // NOP - No Operation
    void nop(stepInfo info);

    // ORA - Logical Inclusive OR
    void ora(stepInfo info);

    // PHA - Push Accumulator
    void pha(stepInfo info);

    // PHP - Push Processor Status
    void php(stepInfo info);

    void php();

    // PLA - Pull Accumulator
    void pla(stepInfo info);

    // PLP - Pull Processor Status
    void plp(stepInfo info);

    // ROL - Rotate Left
    void rol(stepInfo info);

    // ROR - Rotate Right
    void ror(stepInfo info);

    // RTI - Return from Interrupt
    void rti(stepInfo info);

    // RTS - Return from Subroutine
    void rts(stepInfo info);

    // SBC - Subtract with Carry
    void sbc(stepInfo info);

    // SEC - Set Carry Flag
    void sec(stepInfo info);

    // SED - Set Decimal Flag
    void sed(stepInfo info);

    // SEI - Set Interrupt Disable
    void sei(stepInfo info);

    // STA - Store Accumulator
    void sta(stepInfo info);

    // STX - Store X Register
    void stx(stepInfo info);

    // STY - Store Y Register
    void sty(stepInfo info);

    // TAX - Transfer Accumulator to X
    void tax(stepInfo info);

    // TAY - Transfer Accumulator to Y
    void tay(stepInfo info);

    // TSX - Transfer Stack Pointer to X
    void tsx(stepInfo info);

    // TXA - Transfer X to Accumulator
    void txa(stepInfo info);

    // TXS - Transfer X to Stack Pointer
    void txs(stepInfo info);

    // TYA - Transfer Y to Accumulator
    void tya(stepInfo info);

    // illegal opcodes below

    void ahx(stepInfo info);

    void alr(stepInfo info);

    void anc(stepInfo info);

    void arr(stepInfo info);

    void axs(stepInfo info);

    void dcp(stepInfo info);

    void isc(stepInfo info);

    void kil(stepInfo info);

    void las(stepInfo info);

    void lax(stepInfo info);

    void rla(stepInfo info);

    void rra(stepInfo info);

    void sax(stepInfo info);

    void shx(stepInfo info);

    void shy(stepInfo info);

    void slo(stepInfo info);

    void sre(stepInfo info);

    void tas(stepInfo info);

    void xaa(stepInfo info);
};

#endif  // CPPTEST_CPU_H
