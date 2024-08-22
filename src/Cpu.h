#ifndef CPPTEST_CPU_H
#define CPPTEST_CPU_H

#include <bitset>

#include "Ram.h"


const Instruction[256] instructions;

struct Inst {
    void execute();
};

class Instruction {
public:

    // the opcode name
    std::string name;

    // the number of cycles this instruction takes when a page is crossed
    unsigned char pageCycles;

    // the number of cycles this instruction takes
    unsigned char cycles;

    // the size of the instruction in bytes
    unsigned char size;

    // the addressing mode for each instruction
    unsigned char addressingMode;

    // the function to execute
    Inst inst;

};

class Cpu {
private:
    // special
    // The 2-byte program counter PC supports 65536 direct (unbanked) memory
    // locations, however not all values are sent to the cartridge. It can be
    // accessed either by  allowing CPU's internal fetch logic increment the
    // address bus, an interrupt (NMI, Reset, IRQ/BRQ), and using the
    // RTS/JMP/JSR/Branch instructions.
    unsigned short PC{0x0};  // $PC
    char SR;                 // $SR
    char SP{0x0};            // $SP

    // general
    // A is byte-wide and along with the arithmetic logic unit (ALU), supports
    // using the status register for carrying, overflow detection, and so on.

    // Accumulator // $A
    char A{0x0};
    // General // $X
    char X{0x0};
    // General // $Y
    char Y{0x0};

    // status flags
    unsigned char C = 0; // carry
    unsigned char Z = 0; // zero
    unsigned char I = 0; // interrupt disable
    unsigned char D = 0; // decimal mode
    unsigned char B = 0; // break command
    unsigned char U = 0; // unused flag
    unsigned char V = 0; // overflow
    unsigned char N = 0; // negative flag

    double cycles;
    unsigned char mode;
    short address;

    Ram *ram;

    enum class instruction;
    Instruction instructions[256];

    enum class addressing_mode;

    static instruction decode();

public:
    explicit Cpu(Ram *ram);

    void execute(instruction instruction1);

    int step();


    // reset to power up state
    void reset();

    // print current cpu stat
    void printInstruction();

    // are 2 addresses on the same page
    // TODO what is a page?
    bool pagesDiffer(short a, short b);

    // adds extra cpu cycle for branching, and another if pages differ
    void addBranchCycles();

    void compare(unsigned char a, unsigned char b);

    // reads 2 bytes at address
    short read16(short a);

    // 6502 bug that caused the low byte to wrap without incrementing high byte
    short read16bug(short address);

    // push a byte onto stack
    void push(unsigned char b);

    // pops a byte from the stack
    unsigned char pop();

    // push 2 bytes on stack
    void push16(short a);

    // pop 2 bytes from stack
    short pop16() {
        short low = pop();
        short hi = pop();
        return (short) ((hi << 8) | low);
    }

    // returns flags of processor status register
    unsigned char flags() {
        unsigned char flags = 0;
        flags |= C;
        flags |= Z << 1;
        flags |= I << 2;
        flags |= D << 3;
        flags |= B << 4;
        flags |= U << 5;
        flags |= V << 6;
        flags |= N << 7;
        return flags;
    }

    // set the flags from a byte
    void setFlags(unsigned char b) {


    }

    // set the zero flag
    void setZ(unsigned char b) {

    }

    // set the negative flag
    void setN(unsigned char b) {

    }

    // set zero and negative flag
    void setZN(unsigned char b) {

    }

    // triggers the non maskable interrupt on next cpu interation
    void triggerNMI() {

    }

    // triggers the IRQ interupt on next cpu iteration
    void triggerIRQ() {

    }

    // NMI handler
    void NMI() {

    }

    // IRQ handler
    void IRQ() {
    }

    // INSTRUCTIONS

    // ADD with carry
    void ADC() {
    }

    // logical AND
    void AND() {
    }

    // arithmetic shift left
    void ASL() {
    }

    // branch if carry is clear
    void BCC() {
    }

    // branch if carry is set
    void BCS() {
    }

    // branch if equal
    void BEQ() {
    }

    // bit test
    void BIT() {
    }

    // branch if minus
    void BMI() {
    }

    // branch if not equal
    void BNE() {
    }

    // branch if positive
    void BPL() {
    }

    // force interrupt
    void BRK() {
    }

    // branch if overflow clear
    void BVC() {
    }

    // branch if overflow set
    void BVS() {
    }

    // clear carry flag
    void CLC() {
    }

    // clear decimal mode
    void CLD() {
    }

    // clear interrupt disable
    void CLI() {
    }

    // clear overflow flag
    void CLV() {
    }

    // compare
    void CMP() {
    }

    // compare x register
    void CPX() {
    }

    // compare y register
    void CPY() {
    }

    // decrement memory
    void DEC() {
    }

    // decrement x register
    void DEX() {
    }

    // decrement y register
    void DEY() {
    }

    // exclusive or
    void EOR() {
    }

    // increment memory
    void INC() {
    }

    // increment x register
    void INX() {
    }

    // increment y register
    void INY() {
    }

    // jump
    void JMP() {
    }

    // jump to subroutine
    void JSR() {
    }

    // load accumulator
    void LDA() {
    }

    // load x register
    void LDX() {
    }

    // load y register
    void LDY() {
    }

    // logical shift right
    void LSR() {
    }

    // no operation
    void NOP() {
    }

    // logical includive or
    void ORA() {
    }

    // push accumulator
    void PHA() {
    }

    // push processor status
    void PHP() {
    }

    // pull accumulator
    void PLA() {
    }

    // pull processor status
    void PLP() {
    }

    // rotate left
    void ROL() {
    }

    // rotate right
    void ROR() {
    }

    // return from interrupt
    void RTI() {
    }

    // return from subroutine
    void RTS() {
    }

    // subtract with carry
    void SBC() {
    }

    // set carry flag
    void SEC() {
    }

    // set decimal flag
    void SED() {
    }

    // set interupt disable
    void SEI() {
    }

    // store accumulator
    void STA() {
    }

    // store x register
    void STX() {
    }

    // store y register
    void STY() {
    }

    // transfer accumulator to x
    void TAX() {
    }

    // transfer accumulator to y
    void TAY() {
    }

    // transfer stack pointer to x
    void TSX() {
    }

    // transfer x to accumulator
    void TXA() {
    }

    // transfer x to stack pointer
    void TXS() {
    }

    // transfer y to stack pointer
    void TYA() {
    }

    void ARR() {
        // noop
    }

    void AHX() {
        // noop
    }

    void ALR() {
        // noop
    }

    void ANC() {
        // noop
    }

    void AXS() {
        // noop
    }

    void DCP() {
        // noop
    }

    void ISC() {
        // noop
    }

    void KIL() {
        // noop
    }

    void LAS() {
        // noop
    }

    void LAX() {
        // noop
    }

    void RLA() {
        // noop
    }

    void RRA() {
        // noop
    }

    void SAX() {
        // noop
    }

    void TAS() {
        // noop
    }

    void SHX() {
        // noop
    }

    void SHY() {
        // noop
    }

    void SLO() {
        // noop
    }

    void SRE() {
        // noop
    }

    void XAA() {
        // noop
    }

    void execute_instruction(unsigned char opcode);
}

#endif  // CPPTEST_CPU_H
