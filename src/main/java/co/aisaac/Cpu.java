package co.aisaac;

public class Cpu {
	/*


#include <bitset>

#include "Ram.h"

class Cpu {
   private:
    // special
    // The 2-byte program counter PC supports 65536 direct (unbanked) memory
    // locations, however not all values are sent to the cartridge. It can be
    // accessed either by  allowing CPU's internal fetch logic increment the
    // address bus, an interrupt (NMI, Reset, IRQ/BRQ), and using the
    // RTS/JMP/JSR/Branch instructions.
    std::bitset<16> program_counter{0x0};  // $PC
    std::bitset<8> status_register;        // $SR
    std::bitset<8> stack_pointer{0x0};     // $SP

    // general
    // A is byte-wide and along with the arithmetic logic unit (ALU), supports
    // using the status register for carrying, overflow detection, and so on.
    std::bitset<8> accumulator{0x0};  // $A
    std::bitset<8> x_register{0x0};   // $X
    std::bitset<8> y_register{0x0};   // $Y

    template<size_t N>
    static void increment(std::bitset<N> &bitset);

    Ram *ram;

    enum class instruction;

    enum class addressing_mode;

    static instruction decode();

   public:
    explicit Cpu(Ram *ram);

    void step();

    void execute(instruction instruction1);
};

#endif  // CPPTEST_CPU_H


	#include "Cpu.h"

#include <iostream>

Cpu::Cpu(Ram *ram) { this->ram = ram; }

enum class Cpu::addressing_mode {
    INDEXED_D_X,       // val = PEEK((arg + X) % 256)
    INDEXED_D_Y,       // val = PEEK((arg + Y) % 256)
    INDEXED_A_X,       // val = PEEK(arg + X)
    INDEXED_A_Y,       // val = PEEK(arg + Y)
    INDEXED_INDIRECT,  // val = PEEK(PEEK((arg + X) % 256) + PEEK((arg + X + 1) % 256) * 256)
    INDIRECT_INDEXED,  // val = PEEK(PEEK(arg) + PEEK((arg + 1) % 256) * 256 + Y)
};

template<size_t N>
void Cpu::increment(std::bitset<N> &bitset) {
    for (size_t s = 0; s < N; s++) {
        if (bitset[s] == 0) {
            bitset[s] = 1;
            break;
        }
        bitset[s] = 0;
    }
    std::cout << bitset << std::endl;
}

void Cpu::step() {
    // Step 1, we may be waiting some cycles for some reason

    // Step 2, we should handle interupts

    // Step 3, decode opcode by looking at the current program counter
    instruction inst = decode();

    // Step 4, increase program counter by 1

    // Step 5, execute instruction
    execute(inst);

    // step 4
    // set $pc to next instruction
    increment(program_counter);

    // step 5
    // fetch data per memory access mode

    // step 6
    // execute instruction with memory fetched
}

Cpu::instruction Cpu::decode() {
    instruction result;
    return result;
}


enum class Cpu::instruction {

    // LOAD AND STORE operations.
    // You load a register from memory, you store a register to memory.
    // Transfers a single byte between memory and one of the registers
    // Load operations set the negative and zero flags depending on the value
    // transferred.
    // Store operations do not affect the flag
    LDA,  // LOAD A  - N,Z
    LDX,  // LOAD X  - N,Z
    LDY,  // LOAD Y  - N,Z
    STA,  // STORE A
    STX,  // STORE X
    STY,  // STORE Y

    // REGISTER TRANSFERS
    // The contents of the X and Y registers can be moved to or from the
    // accumulator, setting the negative (N) and
    // zero (Z) flags as appropriate.
    TAX,  // Transfer accumulator to X - N,Z
    TAY,  // Transfer accumulator to Y - N,Z
    TXA,  // Transfer X to accumulator - N,Z
    TYA,  // Transfer Y to accumulator - N,Z

    // STACK OPERATIONS
    // The 6502 microprocessor supports a 256 byte stack fixed between memory
    // locations $0100 and $01FF. A special 8-bit
    // register, S, is used to keep track of the next free byte of stack space.
    // Pushing a byte on to the stack causes
    // the value to be stored at the current free location (e.g. $0100,S) and
    // then
    // the stack pointer is post
    // decremented. Pull operations reverse this procedure.

    // The stack register can only be accessed by transferring its value to or
    // from the X register. Its value is
    // automatically modified by push/pull instructions, subroutine calls and
    // returns, interrupts and returns from
    // interrupts.
    TSX,  // Transfer stack pointer to X	      - N,Z
    TXS,  // Transfer X to stack pointer
    PHA,  // Push accumulator on stack
    PHP,  // Push processor status on stack
    PLA,  // Pull accumulator from stack	      - N,Z
    PLP,  // Pull processor status from stack  - All

    // LOGICAL
    // The following instructions perform logical operations on the contents of
    // the accumulator and another value held
    // in memory. The BIT instruction performs a logical AND to test the
    // presence
    // of bits in the memory value to set the
    // flags but does not keep the result.
    AND,  // 	Logical AND               - N,Z
    EOR,  // 	Exclusive OR              - N,Z
    ORA,  // 	Logical Inclusive OR      - N,Z
    BIT,  // 	Bit Test                  - N,V,Z

    // ARITHMETIC
    // The arithmetic operations perform addition and subtraction on the
    // contents
    // of the accumulator. The compare
    // operations allow the comparison of the accumulator and X or Y with memory
    // values.
    ADC,  // Add with Carry                - N,V,Z,C
    SBC,  // Subtract with Carry           - N,V,Z,C
    CMP,  // Compare accumulator           - N,Z,C
    CPX,  // Compare X register            - N,Z,C
    CPY,  // Compare Y register            - N,Z,C

    // INCREMENTS & DECREMENTS
    // Increment or decrement a memory location or one of the X or Y registers
    // by
    // one setting the negative (N) and zero
    // (Z) flags as appropriate,
    INC,  // Increment a memory location   - N,Z
    INX,  // Increment the X register      - N,Z
    INY,  // Increment the Y register      - N,Z
    DEC,  // Decrement a memory location   - N,Z
    DEX,  // Decrement the X register      - N,Z
    DEY,  // Decrement the Y register      - N,Z

    // SHIFTS
    // Shift instructions cause the bits within either a memory location or the
    // accumulator to be shifted by one bit
    // position. The rotate instructions use the contents if the carry flag (C)
    // to
    // fill the vacant position generated
    // by the shift and to catch the overflowing bit. The arithmetic and logical
    // shifts shift in an appropriate 0 or 1
    // bit as appropriate but catch the overflow bit in the carry flag (C).
    ASL,  // Arithmetic Shift Left         - N,Z,C
    LSR,  // Logical Shift Right           - N,Z,C
    ROL,  // Rotate Left                   - N,Z,C
    ROR,  // Rotate Right                  - N,Z,C

    // JUMPS & CALLS
    // The following instructions modify the program counter causing a break to
    // normal sequential execution. The JSR
    // instruction pushes the old PC onto the stack before changing it to the
    // new
    // location allowing a subsequent RTS to
    // return execution to the instruction after the call.
    JMP,  // Jump to another location
    JSR,  // Jump to a subroutine
    RTS,  // Return from subroutine

    // BRANCHES
    // Branch instructions break the normal sequential flow of execution by
    // changing the program counter if a specified
    // condition is met. All the conditions are based on examining a single bit
    // within the processor status.

    // Branch instructions use relative address to identify the target
    // instruction
    // if they are executed. As relative
    // addresses are stored using a signed 8 bit byte the target instruction
    // must
    // be within 126 bytes before the branch
    // or 128 bytes after the branch.
    BCC,  // Branch if carry flag clear
    BCS,  // Branch if carry flag set
    BEQ,  // Branch if zero flag set
    BMI,  // Branch if negative flag set
    BNE,  // Branch if zero flag clear
    BPL,  // Branch if negative flag clear
    BVC,  // Branch if overflow flag clear
    BVS,  // Branch if overflow flag set

    // STATUS FLAG CHANGES
    // The following instructions change the values of specific status flags.
    CLC,  // Clear carry flag
    CLD,  // Clear decimal mode flag
    CLI,  // Clear interrupt disable flag
    CLV,  // Clear overflow flag
    SEC,  // Set carry flag
    SED,  // Set decimal mode flag
    SEI,  // Set interrupt disable flag

    // SYSTEM FUNCTIONS
    // The remaining instructions perform useful but rarely used functions.
    BRK,  // Force an interrupt
    NOP,  // No Operation
    RTI,  // Return from Interrupt

};

void Cpu::execute(Cpu::instruction inst) {
    switch (inst) {

        // load store instructions
        case instruction::LDA:
            break;
        case instruction::LDX:
            break;
        case instruction::LDY:
            break;
        case instruction::STA:
            break;
        case instruction::STX:
            break;
        case instruction::STY:
            break;

        case instruction::TAX:
            break;
        case instruction::TAY:
            break;
        case instruction::TXA:
            break;
        case instruction::TYA:
            break;
        case instruction::TSX:
            break;
        case instruction::TXS:
            break;
        case instruction::PHA:
            break;
        case instruction::PHP:
            break;
        case instruction::PLA:
            break;
        case instruction::PLP:
            break;
        case instruction::AND:
            break;
        case instruction::EOR:
            break;
        case instruction::ORA:
            break;
        case instruction::BIT:
            break;
        case instruction::ADC:
            break;
        case instruction::SBC:
            break;
        case instruction::CMP:
            break;
        case instruction::CPX:
            break;
        case instruction::CPY:
            break;
        case instruction::INC:
            break;
        case instruction::INX:
            break;
        case instruction::INY:
            break;
        case instruction::DEC:
            break;
        case instruction::DEX:
            break;
        case instruction::DEY:
            break;
        case instruction::ASL:
            break;
        case instruction::LSR:
            break;
        case instruction::ROL:
            break;
        case instruction::ROR:
            break;
        case instruction::JMP:
            break;
        case instruction::JSR:
            break;
        case instruction::RTS:
            break;
        case instruction::BCC:
            break;
        case instruction::BCS:
            break;
        case instruction::BEQ:
            break;
        case instruction::BMI:
            break;
        case instruction::BNE:
            break;
        case instruction::BPL:
            break;
        case instruction::BVC:
            break;
        case instruction::BVS:
            break;
        case instruction::CLC:
            break;
        case instruction::CLD:
            break;
        case instruction::CLI:
            break;
        case instruction::CLV:
            break;
        case instruction::SEC:
            break;
        case instruction::SED:
            break;
        case instruction::SEI:
            break;
        case instruction::BRK:
            break;
        case instruction::NOP:
            break;
        case instruction::RTI:
            break;
    }
}


	 */
}
