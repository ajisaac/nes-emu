#ifndef CPPTEST_CPU_H
#define CPPTEST_CPU_H

#include "Ram.h"

class Cpu {
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

    Ram* ram;
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

    Cpu(Ram* pRam) {
        ram = pRam;
        interrupt = interruptNone;
        stall = 0;
    }

    short Read(short address) {
        return 0;
    }

    void Write(short address, char byte) {}


    //        table     [256]func(*stepInfo)

    //    Cpu(Console &console) {
    //        cpu = CPU{Memory: NewCPUMemory(console)}
    //        cpu.createTable()
    //        cpu.Reset()
    //        return &cpu
    //    }

    // createTable builds a function table for each instruction
    //    void createTable() {
    //        c.table = [256]func(*stepInfo){
    //            c.brk, c.ora, c.kil, c.slo, c.nop, c.ora, c.asl, c.slo,
    //                    c.php, c.ora, c.asl, c.anc, c.nop, c.ora, c.asl, c.slo,
    //                    c.bpl, c.ora, c.kil, c.slo, c.nop, c.ora, c.asl, c.slo,
    //                    c.clc, c.ora, c.nop, c.slo, c.nop, c.ora, c.asl, c.slo,
    //                    c.jsr, c.and, c.kil, c.rla, c.bit, c.and, c.rol, c.rla,
    //                    c.plp, c.and, c.rol, c.anc, c.bit, c.and, c.rol, c.rla,
    //                    c.bmi, c.and, c.kil, c.rla, c.nop, c.and, c.rol, c.rla,
    //                    c.sec, c.and, c.nop, c.rla, c.nop, c.and, c.rol, c.rla,
    //                    c.rti, c.eor, c.kil, c.sre, c.nop, c.eor, c.lsr, c.sre,
    //                    c.pha, c.eor, c.lsr, c.alr, c.jmp, c.eor, c.lsr, c.sre,
    //                    c.bvc, c.eor, c.kil, c.sre, c.nop, c.eor, c.lsr, c.sre,
    //                    c.cli, c.eor, c.nop, c.sre, c.nop, c.eor, c.lsr, c.sre,
    //                    c.rts, c.adc, c.kil, c.rra, c.nop, c.adc, c.ror, c.rra,
    //                    c.pla, c.adc, c.ror, c.arr, c.jmp, c.adc, c.ror, c.rra,
    //                    c.bvs, c.adc, c.kil, c.rra, c.nop, c.adc, c.ror, c.rra,
    //                    c.sei, c.adc, c.nop, c.rra, c.nop, c.adc, c.ror, c.rra,
    //                    c.nop, c.sta, c.nop, c.sax, c.sty, c.sta, c.stx, c.sax,
    //                    c.dey, c.nop, c.txa, c.xaa, c.sty, c.sta, c.stx, c.sax,
    //                    c.bcc, c.sta, c.kil, c.ahx, c.sty, c.sta, c.stx, c.sax,
    //                    c.tya, c.sta, c.txs, c.tas, c.shy, c.sta, c.shx, c.ahx,
    //                    c.ldy, c.lda, c.ldx, c.lax, c.ldy, c.lda, c.ldx, c.lax,
    //                    c.tay, c.lda, c.tax, c.lax, c.ldy, c.lda, c.ldx, c.lax,
    //                    c.bcs, c.lda, c.kil, c.lax, c.ldy, c.lda, c.ldx, c.lax,
    //                    c.clv, c.lda, c.tsx, c.las, c.ldy, c.lda, c.ldx, c.lax,
    //                    c.cpy, c.cmp, c.nop, c.dcp, c.cpy, c.cmp, c.dec, c.dcp,
    //                    c.iny, c.cmp, c.dex, c.axs, c.cpy, c.cmp, c.dec, c.dcp,
    //                    c.bne, c.cmp, c.kil, c.dcp, c.nop, c.cmp, c.dec, c.dcp,
    //                    c.cld, c.cmp, c.nop, c.dcp, c.nop, c.cmp, c.dec, c.dcp,
    //                    c.cpx, c.sbc, c.nop, c.isc, c.cpx, c.sbc, c.inc, c.isc,
    //                    c.inx, c.sbc, c.nop, c.sbc, c.cpx, c.sbc, c.inc, c.isc,
    //                    c.beq, c.sbc, c.kil, c.isc, c.nop, c.sbc, c.inc, c.isc,
    //                    c.sed, c.sbc, c.nop, c.isc, c.nop, c.sbc, c.inc, c.isc,
    //        }
    //    }

    //    func (cpu *CPU) Save(encoder *gob.Encoder) error {
    //        encoder.Encode(cpu.Cycles)
    //        encoder.Encode(cpu.PC)
    //        encoder.Encode(cpu.SP)
    //        encoder.Encode(cpu.A)
    //        encoder.Encode(cpu.X)
    //        encoder.Encode(cpu.Y)
    //        encoder.Encode(cpu.C)
    //        encoder.Encode(cpu.Z)
    //        encoder.Encode(cpu.I)
    //        encoder.Encode(cpu.D)
    //        encoder.Encode(cpu.B)
    //        encoder.Encode(cpu.U)
    //        encoder.Encode(cpu.V)
    //        encoder.Encode(cpu.N)
    //        encoder.Encode(cpu.interrupt)
    //        encoder.Encode(cpu.stall)
    //        return nil
    //    }
    //
    //    func (cpu *CPU) Load(decoder *gob.Decoder) error {
    //        decoder.Decode(&cpu.Cycles)
    //        decoder.Decode(&cpu.PC)
    //        decoder.Decode(&cpu.SP)
    //        decoder.Decode(&cpu.A)
    //        decoder.Decode(&cpu.X)
    //        decoder.Decode(&cpu.Y)
    //        decoder.Decode(&cpu.C)
    //        decoder.Decode(&cpu.Z)
    //        decoder.Decode(&cpu.I)
    //        decoder.Decode(&cpu.D)
    //        decoder.Decode(&cpu.B)
    //        decoder.Decode(&cpu.U)
    //        decoder.Decode(&cpu.V)
    //        decoder.Decode(&cpu.N)
    //        decoder.Decode(&cpu.interrupt)
    //        decoder.Decode(&cpu.stall)
    //        return nil
    //    }

    // Reset resets the CPU to its initial powerup state
    void Reset() {
        PC = Read16(0xFFFC);
        SP = 0xFD;
        SetFlags(0x24);
    }


    // Step executes a single CPU instruction
    int Step() {
        if (stall > 0) {
            stall--;
            return 1;
        }

        //        cycles = Cycles;

        switch (interrupt) {
        case interruptNMI:
            nmi();
        case interruptIRQ:
            irq();
        }
        interrupt = interruptNone;

        auto opcode = Read(PC);
        auto mode = instructionModes[opcode];

        short address;
        bool pageCrossed;
        short offset = 0;
        switch (mode) {
        case modeAbsolute:
            address = Read16(PC + 1);
            break;
        case modeAbsoluteX:
            address = Read16(PC + 1) + short(X);
            pageCrossed = pagesDiffer(address - short(X), address);
            break;
        case modeAbsoluteY:
            address = Read16(PC + 1) + short(Y);
            pageCrossed = pagesDiffer(address - short(Y), address);
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
            address = read16bug(short(Read(PC + 1) + X));
            break;
        case modeIndirect:
            address = read16bug(Read16(PC + 1));
            break;
        case modeIndirectIndexed:
            address = read16bug(short(Read(PC + 1))) + short(Y);
            pageCrossed = pagesDiffer(address - short(Y), address);
            break;
        case modeRelative:
            offset = short(Read(PC + 1));
            if (offset < 0x80) {
                address = PC + 2 + offset;
            }
            else {
                address = PC + 2 + offset - 0x100;
            }
            break;
        case modeZeroPage:
            address = short(Read(PC + 1));
            break;
        case modeZeroPageX:
            address = short(Read(PC + 1) + X) & 0xff;
            break;
        case modeZeroPageY:
            address = short(Read(PC + 1) + Y) & 0xff;
            break;
        }

        PC += short(instructionSizes[opcode]);
        Cycles += long(instructionCycles[opcode]);
        if (pageCrossed) {
            Cycles += long(instructionPageCycles[opcode]);
        }
        auto info = stepInfo{address, PC, mode};
        //        table[opcode](info);

        //        return int(Cycles - cycles);
        return Cycles;
    }

    // PrintInstruction prints the current CPU state
    //    void PrintInstruction() {
    //        opcode = Read(PC);
    //        bytes = instructionSizes[opcode];
    //        name = instructionNames[opcode];
    //        w0 = fmt.Sprintf("%02X", cpu.Read(cpu.PC+0));
    //        w1 = fmt.Sprintf("%02X", cpu.Read(cpu.PC+1));
    //        w2 = fmt.Sprintf("%02X", cpu.Read(cpu.PC+2));
    //        if bytes < 2 {
    //            w1 = "  ";
    //        }
    //        if bytes < 3 {
    //            w2 = "  ";
    //        }
    //        fmt.Printf(
    //                "%4X  %s %s %s  %s %28s"+
    //                "A:%02X X:%02X Y:%02X P:%02X SP:%02X CYC:%3d\n",
    //                cpu.PC, w0, w1, w2, name, "",
    //                cpu.A, cpu.X, cpu.Y, cpu.Flags(), cpu.SP, (cpu.Cycles*3)%341);
    //    }

    // pagesDiffer returns true if the two addresses reference different pages
    bool pagesDiffer(short a, short b) {
        return (a & 0xFF00) != (b & 0xFF00);
    }


    // stepInfo contains information that the instruction functions use
    struct stepInfo {
        short address;
        short pc;
        char mode;
    };

    // addBranchCycles adds a cycle for taking a branch and adds another cycle
    // if the branch jumps to a new page
    void addBranchCycles(stepInfo info) {
        Cycles++;
        if (pagesDiffer(info.pc, info.address)) {
            Cycles++;
        }
    }

    void compare(char a, char b) {
        setZN(a - b);
        if (a >= b) {
            C = 1;
        }
        else {
            C = 0;
        }
    }

    // Read16 reads two bytes using Read to return a double-word value
    short Read16(short address) {
        char lo = short(Read(address));
        char hi = short(Read(address + 1));
        return hi << 8 | lo;
    }

    // read16bug emulates a 6502 bug that caused the low byte to wrap without
    // incrementing the high byte
    short read16bug(short address) {
        short a = address;
        short b = (a & 0xFF00) | short(char(a) + 1);
        char lo = Read(a);
        char hi = Read(b);
        return short(hi) << 8 | short(lo);
    }

    // push pushes a byte onto the stack
    void push(char value) {
        Write(0x100 | short(SP), value);
        SP--;
    }

    // pull pops a byte from the stack
    char pull() {
        SP++;
        return (Read(0x100 | short(SP)));
    }

    // push16 pushes two bytes onto the stack
    void push16(short value) {
        char hi = char(value >> 8);
        char lo = char(value & 0xFF);
        push(hi);
        push(lo);
    }

    // pull16 pops two bytes from the stack
    short pull16() {
        short lo = short(pull());
        short hi = short(pull());
        return hi << 8 | lo;
    }

    // Flags returns the processor status flags
    char Flags() {
        char flags = 0;
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
    void SetFlags(char flags) {
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
    void setZ(char value) {
        if (value == 0) {
            Z = 1;
        }
        else {
            Z = 0;
        }
    }

    // setN sets the negative flag if the argument is negative (high bit is set)
    void setN(char value) {
        if (value & 0x80 != 0) {
            N = 1;
        }
        else {
            N = 0;
        }
    }

    // setZN sets the zero flag and the negative flag
    void setZN(char value) {
        setZ(value);
        setN(value);
    }

    // triggerNMI causes a non-maskable interrupt to occur on the next cycle
    void triggerNMI() {
        interrupt = interruptNMI;
    }

    // triggerIRQ causes an IRQ interrupt to occur on the next cycle
    void triggerIRQ() {
        if (I == 0) {
            interrupt = interruptIRQ;
        }
    }

    // NMI - Non-Maskable Interrupt
    void nmi() {
        push16(PC);
        php();
        PC = Read16(0xFFFA);
        I = 1;
        Cycles += 7;
    }

    // IRQ - IRQ Interrupt
    void irq() {
        push16(PC);
        php();
        PC = Read16(0xFFFE);
        I = 1;
        Cycles += 7;
    }

    // ADC - Add with Carry
    void adc(stepInfo info) {
        auto a = A;
        auto b = Read(info.address);
        auto c = C;
        A = a + b + c;
        setZN(A);
        if (int(a) + int(b) + int(c) > 0xFF) {
            C = 1;
        }
        else {
            C = 0;
        }
        if (((a ^ b) & 0x80 == 0) && ((a ^ A) & 0x80 != 0)) {
            V = 1;
        }
        else {
            V = 0;
        }
    }

    // AND - Logical AND
    void AND(stepInfo info) {
        A = A & Read(info.address);
        setZN(A);
    }

    // ASL - Arithmetic Shift Left
    void asl(stepInfo info) {
        if (info.mode == modeAccumulator) {
            C = (A >> 7) & 1;
            A <<= 1;
            setZN(A);
        }
        else {
            auto value = Read(info.address);
            C = (value >> 7) & 1;
            value <<= 1;
            Write(info.address, value);
            setZN(value);
        }
    }

    // BCC - Branch if Carry Clear
    void bcc(stepInfo info) {
        if (C == 0) {
            PC = info.address;
            addBranchCycles(info);
        }
    }

    // BCS - Branch if Carry Set
    void bcs(stepInfo info) {
        if (C != 0) {
            PC = info.address;
            addBranchCycles(info);
        }
    }

    // BEQ - Branch if Equal
    void beq(stepInfo info) {
        if (Z != 0) {
            auto PC = info.address;
            auto addBranchCycles(info);
        }
    }

    // BIT - Bit Test
    void bit(stepInfo info) {
        auto value = Read(info.address);
        V = (value >> 6) & 1;
        setZ(value & A);
        setN(value);
    }

    // BMI - Branch if Minus
    void bmi(stepInfo info) {
        if (N != 0) {
            PC = info.address;
            addBranchCycles(info);
        }
    }

    // BNE - Branch if Not Equal
    void bne(stepInfo info) {
        if (Z == 0) {
            PC = info.address;
            addBranchCycles(info);
        }
    }

    // BPL - Branch if Positive
    void bpl(stepInfo info) {
        if (N == 0) {
            PC = info.address;
            addBranchCycles(info);
        }
    }

    // BRK - Force Interrupt
    void brk(stepInfo info) {
        push16(PC);
        php(info);
        sei(info);
        PC = Read16(0xFFFE);
    }

    // BVC - Branch if Overflow Clear
    void bvc(stepInfo info) {
        if (V == 0) {
            PC = info.address;
            addBranchCycles(info);
        }
    }

    // BVS - Branch if Overflow Set
    void bvs(stepInfo info) {
        if (V != 0) {
            PC = info.address;
            addBranchCycles(info);
        }
    }

    // CLC - Clear Carry Flag
    void clc(stepInfo info) {
        C = 0;
    }

    // CLD - Clear Decimal Mode
    void cld(stepInfo info) {
        D = 0;
    }

    // CLI - Clear Interrupt Disable
    void cli(stepInfo info) {
        I = 0;
    }

    // CLV - Clear Overflow Flag
    void clv(stepInfo info) {
        V = 0;
    }

    // CMP - Compare
    void cmp(stepInfo info) {
        auto value = Read(info.address);
        compare(A, value);
    }

    // CPX - Compare X Register
    void cpx(stepInfo info) {
        auto value = Read(info.address);
        compare(X, value);
    }

    // CPY - Compare Y Register
    void cpy(stepInfo info) {
        auto value = Read(info.address);
        compare(Y, value);
    }

    // DEC - Decrement Memory
    void dec(stepInfo info) {
        auto value = Read(info.address) - 1;
        Write(info.address, value);
        setZN(value);
    }

    // DEX - Decrement X Register
    void dex(stepInfo info) {
        X--;
        setZN(X);
    }

    // DEY - Decrement Y Register
    void dey(stepInfo info) {
        Y--;
        setZN(Y);
    }

    // EOR - Exclusive OR
    void eor(stepInfo info) {
        A = A ^ Read(info.address);
        setZN(A);
    }

    // INC - Increment Memory
    void inc(stepInfo info) {
        auto value = Read(info.address) + 1;
        Write(info.address, value);
        setZN(value);
    }

    // INX - Increment X Register
    void inx(stepInfo info) {
        X++;
        setZN(X);
    }

    // INY - Increment Y Register
    void iny(stepInfo info) {
        Y++;
        setZN(Y);
    }

    // JMP - Jump
    void jmp(stepInfo info) {
        PC = info.address;
    }

    // JSR - Jump to Subroutine
    void jsr(stepInfo info) {
        push16(PC - 1);
        PC = info.address;
    }

    // LDA - Load Accumulator
    void lda(stepInfo info) {
        A = Read(info.address);
        setZN(A);
    }

    // LDX - Load X Register
    void ldx(stepInfo info) {
        X = Read(info.address);
        setZN(X);
    }

    // LDY - Load Y Register
    void ldy(stepInfo info) {
        Y = Read(info.address);
        setZN(Y);
    }

    // LSR - Logical Shift Right
    void lsr(stepInfo info) {
        if (info.mode == modeAccumulator) {
            C = A & 1;
            A >>= 1;
            setZN(A);
        }
        else {
            auto value = Read(info.address);
            C = value & 1;
            value >>= 1;
            Write(info.address, value);
            setZN(value);
        }
    }

    // NOP - No Operation
    void nop(stepInfo info) {}

    // ORA - Logical Inclusive OR
    void ora(stepInfo info) {
        A = A | Read(info.address);
        setZN(A);
    }

    // PHA - Push Accumulator
    void pha(stepInfo info) {
        push(A);
    }

    // PHP - Push Processor Status
    void php(stepInfo info) {
        push(Flags() | 0x10);
    }

    void php() {
        push(Flags() | 0x10);
    }

    // PLA - Pull Accumulator
    void pla(stepInfo info) {
        A = pull();
        setZN(A);
    }

    // PLP - Pull Processor Status
    void plp(stepInfo info) {
        SetFlags(pull() & 0xEF | 0x20);
    }

    // ROL - Rotate Left
    void rol(stepInfo info) {
        if (info.mode == modeAccumulator) {
            auto c = C;
            C = (A >> 7) & 1;
            A = (A << 1) | c;
            setZN(A);
        }
        else {
            auto c = C;
            auto value = Read(info.address);
            C = (value >> 7) & 1;
            value = (value << 1) | c;
            Write(info.address, value);
            setZN(value);
        }
    }

    // ROR - Rotate Right
    void ror(stepInfo info) {
        if (info.mode == modeAccumulator) {
            auto c = C;
            C = A & 1;
            A = (A >> 1) | (c << 7);
            setZN(A);
        }
        else {
            auto c = C;
            auto value = Read(info.address);
            C = value & 1;
            value = (value >> 1) | (c << 7);
            Write(info.address, value);
            setZN(value);
        }
    }

    // RTI - Return from Interrupt
    void rti(stepInfo info) {
        SetFlags(pull() & 0xEF | 0x20);
        PC = pull16();
    }

    // RTS - Return from Subroutine
    void rts(stepInfo info) {
        PC = pull16() + 1;
    }

    // SBC - Subtract with Carry
    void sbc(stepInfo info) {
        auto a = A;
        auto b = Read(info.address);
        auto c = C;
        A = a - b - (1 - c);
        setZN(A);
        if (int(a) - int(b) - int(1 - c) >= 0) {
            C = 1;
        }
        else {
            C = 0;
        }
        if ((a ^ b) & 0x80 != 0 && (a ^ A) & 0x80 != 0) {
            V = 1;
        }
        else {
            V = 0;
        }
    }

    // SEC - Set Carry Flag
    void sec(stepInfo info) {
        C = 1;
    }

    // SED - Set Decimal Flag
    void sed(stepInfo info) {
        D = 1;
    }

    // SEI - Set Interrupt Disable
    void sei(stepInfo info) {
        I = 1;
    }

    // STA - Store Accumulator
    void sta(stepInfo info) {
        Write(info.address, A);
    }

    // STX - Store X Register
    void stx(stepInfo info) {
        Write(info.address, X);
    }

    // STY - Store Y Register
    void sty(stepInfo info) {
        Write(info.address, Y);
    }

    // TAX - Transfer Accumulator to X
    void tax(stepInfo info) {
        X = A;
        setZN(X);
    }

    // TAY - Transfer Accumulator to Y
    void tay(stepInfo info) {
        Y = A;
        setZN(Y);
    }

    // TSX - Transfer Stack Pointer to X
    void tsx(stepInfo info) {
        X = SP;
        setZN(X);
    }

    // TXA - Transfer X to Accumulator
    void txa(stepInfo info) {
        A = X;
        setZN(A);
    }

    // TXS - Transfer X to Stack Pointer
    void txs(stepInfo info) {
        SP = X;
    }

    // TYA - Transfer Y to Accumulator
    void tya(stepInfo info) {
        A = Y;
        setZN(A);
    }

    // illegal opcodes below

    void ahx(stepInfo info) {}

    void alr(stepInfo info) {}

    void anc(stepInfo info) {}

    void arr(stepInfo info) {}

    void axs(stepInfo info) {}

    void dcp(stepInfo info) {}

    void isc(stepInfo info) {}

    void kil(stepInfo info) {}

    void las(stepInfo info) {}

    void lax(stepInfo info) {}

    void rla(stepInfo info) {}

    void rra(stepInfo info) {}

    void sax(stepInfo info) {}

    void shx(stepInfo info) {}

    void shy(stepInfo info) {}

    void slo(stepInfo info) {}

    void sre(stepInfo info) {}

    void tas(stepInfo info) {}

    void xaa(stepInfo info) {}
};

#endif  // CPPTEST_CPU_H
