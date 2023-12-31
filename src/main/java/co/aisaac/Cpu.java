package co.aisaac;

public class Cpu {
	// special
	// The 2-byte program counter PC supports 65536 direct (unbanked) memory locations,
	// however not all values are sent to the cartridge. It can be accessed either by
	// allowing CPU's internal fetch logic increment the address bus, an interrupt
	// (NMI, Reset, IRQ/BRQ), and using the RTS/JMP/JSR/Branch instructions.
	// program counter
	short PC;

	// status register
	// The 8-bit status register P contains flags which are set or cleared to reflect
	// the results of operations. The flags are:
	// N - Negative
	// V - Overflow
	// - - ignored
	// B - Break
	// D - Decimal (use BCD for arithmetics)
	// I - Interrupt (IRQ disable)
	// Z - Zero
	// C - Carry
	// NV-BDIZC
	byte SR;

	// stack pointer
	byte SP;

	// general
	// A is byte-wide and along with the arithmetic logic unit (ALU), supports
	// using the status register for carrying, overflow detection, and so on.
	byte accumulator;  // $A
	byte x_register;   // $X
	byte y_register;   // $Y

	Instruction[] table;

	final Ram ram;

	Cpu(Ram ram) {
		this.ram = ram;
		createTable();
		reset();
	}

	private void createTable() {
		table = new Instruction[256];
		instructions[0] = new Instruction("BRK", (byte) 0, (byte) 7, (byte) 2, (byte) 6, ()->{BRK();});
		instructions[1] = new Instruction("ORA", (byte) 0, (byte) 6, (byte) 2, (byte) 7, ()->{ORA();});
		instructions[2] = new Instruction("KIL", (byte) 0, (byte) 2, (byte) 0, (byte) 6, ()->{KIL();});
		instructions[3] = new Instruction("SLO", (byte) 0, (byte) 8, (byte) 0, (byte) 7, ()->{SLO();});
		instructions[4] = new Instruction("NOP", (byte) 0, (byte) 3, (byte) 2, (byte) 11, ()->{NOP();});
		instructions[5] = new Instruction("ORA", (byte) 0, (byte) 3, (byte) 2, (byte) 11, ()->{ORA();});
		instructions[6] = new Instruction("ASL", (byte) 0, (byte) 5, (byte) 2, (byte) 11, ()->{ASL();});
		instructions[7] = new Instruction("SLO", (byte) 0, (byte) 5, (byte) 0, (byte) 11, ()->{SLO();});
		instructions[8] = new Instruction("PHP", (byte) 0, (byte) 3, (byte) 1, (byte) 6, ()->{PHP();});
		instructions[9] = new Instruction("ORA", (byte) 0, (byte) 2, (byte) 2, (byte) 5, ()->{ORA();});
		instructions[10] = new Instruction("ASL", (byte) 0, (byte) 2, (byte) 1, (byte) 4, ()->{ASL();});
		instructions[11] = new Instruction("ANC", (byte) 0, (byte) 2, (byte) 0, (byte) 5, ()->{ANC();});
		instructions[12] = new Instruction("NOP", (byte) 0, (byte) 4, (byte) 3, (byte) 1, ()->{NOP();});
		instructions[13] = new Instruction("ORA", (byte) 0, (byte) 4, (byte) 3, (byte) 1, ()->{ORA();});
		instructions[14] = new Instruction("ASL", (byte) 0, (byte) 6, (byte) 3, (byte) 1, ()->{ASL();});
		instructions[15] = new Instruction("SLO", (byte) 0, (byte) 6, (byte) 0, (byte) 1, ()->{SLO();});
		instructions[16] = new Instruction("BPL", (byte) 1, (byte) 2, (byte) 2, (byte) 10, ()->{BPL();});
		instructions[17] = new Instruction("ORA", (byte) 1, (byte) 5, (byte) 2, (byte) 9, ()->{ORA();});
		instructions[18] = new Instruction("KIL", (byte) 0, (byte) 2, (byte) 0, (byte) 6, ()->{KIL();});
		instructions[19] = new Instruction("SLO", (byte) 0, (byte) 8, (byte) 0, (byte) 9, ()->{SLO();});
		instructions[20] = new Instruction("NOP", (byte) 0, (byte) 4, (byte) 2, (byte) 12, ()->{NOP();});
		instructions[21] = new Instruction("ORA", (byte) 0, (byte) 4, (byte) 2, (byte) 12, ()->{ORA();});
		instructions[22] = new Instruction("ASL", (byte) 0, (byte) 6, (byte) 2, (byte) 12, ()->{ASL();});
		instructions[23] = new Instruction("SLO", (byte) 0, (byte) 6, (byte) 0, (byte) 12, ()->{SLO();});
		instructions[24] = new Instruction("CLC", (byte) 0, (byte) 2, (byte) 1, (byte) 6, ()->{CLC();});
		instructions[25] = new Instruction("ORA", (byte) 1, (byte) 4, (byte) 3, (byte) 3, ()->{ORA();});
		instructions[26] = new Instruction("NOP", (byte) 0, (byte) 2, (byte) 1, (byte) 6, ()->{NOP();});
		instructions[27] = new Instruction("SLO", (byte) 0, (byte) 7, (byte) 0, (byte) 3, ()->{SLO();});
		instructions[28] = new Instruction("NOP", (byte) 1, (byte) 4, (byte) 3, (byte) 2, ()->{NOP();});
		instructions[29] = new Instruction("ORA", (byte) 1, (byte) 4, (byte) 3, (byte) 2, ()->{ORA();});
		instructions[30] = new Instruction("ASL", (byte) 0, (byte) 7, (byte) 3, (byte) 2, ()->{ASL();});
		instructions[31] = new Instruction("SLO", (byte) 0, (byte) 7, (byte) 0, (byte) 2, ()->{SLO();});
		instructions[32] = new Instruction("JSR", (byte) 0, (byte) 6, (byte) 3, (byte) 1, ()->{JSR();});
		instructions[33] = new Instruction("AND", (byte) 0, (byte) 6, (byte) 2, (byte) 7, ()->{AND();});
		instructions[34] = new Instruction("KIL", (byte) 0, (byte) 2, (byte) 0, (byte) 6, ()->{KIL();});
		instructions[35] = new Instruction("RLA", (byte) 0, (byte) 8, (byte) 0, (byte) 7, ()->{RLA();});
		instructions[36] = new Instruction("BIT", (byte) 0, (byte) 3, (byte) 2, (byte) 11, ()->{BIT();});
		instructions[37] = new Instruction("AND", (byte) 0, (byte) 3, (byte) 2, (byte) 11, ()->{AND();});
		instructions[38] = new Instruction("ROL", (byte) 0, (byte) 5, (byte) 2, (byte) 11, ()->{ROL();});
		instructions[39] = new Instruction("RLA", (byte) 0, (byte) 5, (byte) 0, (byte) 11, ()->{RLA();});
		instructions[40] = new Instruction("PLP", (byte) 0, (byte) 4, (byte) 1, (byte) 6, ()->{PLP();});
		instructions[41] = new Instruction("AND", (byte) 0, (byte) 2, (byte) 2, (byte) 5, ()->{AND();});
		instructions[42] = new Instruction("ROL", (byte) 0, (byte) 2, (byte) 1, (byte) 4, ()->{ROL();});
		instructions[43] = new Instruction("ANC", (byte) 0, (byte) 2, (byte) 0, (byte) 5, ()->{ANC();});
		instructions[44] = new Instruction("BIT", (byte) 0, (byte) 4, (byte) 3, (byte) 1, ()->{BIT();});
		instructions[45] = new Instruction("AND", (byte) 0, (byte) 4, (byte) 3, (byte) 1, ()->{AND();});
		instructions[46] = new Instruction("ROL", (byte) 0, (byte) 6, (byte) 3, (byte) 1, ()->{ROL();});
		instructions[47] = new Instruction("RLA", (byte) 0, (byte) 6, (byte) 0, (byte) 1, ()->{RLA();});
		instructions[48] = new Instruction("BMI", (byte) 1, (byte) 2, (byte) 2, (byte) 10, ()->{BMI();});
		instructions[49] = new Instruction("AND", (byte) 1, (byte) 5, (byte) 2, (byte) 9, ()->{AND();});
		instructions[50] = new Instruction("KIL", (byte) 0, (byte) 2, (byte) 0, (byte) 6, ()->{KIL();});
		instructions[51] = new Instruction("RLA", (byte) 0, (byte) 8, (byte) 0, (byte) 9, ()->{RLA();});
		instructions[52] = new Instruction("NOP", (byte) 0, (byte) 4, (byte) 2, (byte) 12, ()->{NOP();});
		instructions[53] = new Instruction("AND", (byte) 0, (byte) 4, (byte) 2, (byte) 12, ()->{AND();});
		instructions[54] = new Instruction("ROL", (byte) 0, (byte) 6, (byte) 2, (byte) 12, ()->{ROL();});
		instructions[55] = new Instruction("RLA", (byte) 0, (byte) 6, (byte) 0, (byte) 12, ()->{RLA();});
		instructions[56] = new Instruction("SEC", (byte) 0, (byte) 2, (byte) 1, (byte) 6, ()->{SEC();});
		instructions[57] = new Instruction("AND", (byte) 1, (byte) 4, (byte) 3, (byte) 3, ()->{AND();});
		instructions[58] = new Instruction("NOP", (byte) 0, (byte) 2, (byte) 1, (byte) 6, ()->{NOP();});
		instructions[59] = new Instruction("RLA", (byte) 0, (byte) 7, (byte) 0, (byte) 3, ()->{RLA();});
		instructions[60] = new Instruction("NOP", (byte) 1, (byte) 4, (byte) 3, (byte) 2, ()->{NOP();});
		instructions[61] = new Instruction("AND", (byte) 1, (byte) 4, (byte) 3, (byte) 2, ()->{AND();});
		instructions[62] = new Instruction("ROL", (byte) 0, (byte) 7, (byte) 3, (byte) 2, ()->{ROL();});
		instructions[63] = new Instruction("RLA", (byte) 0, (byte) 7, (byte) 0, (byte) 2, ()->{RLA();});
		instructions[64] = new Instruction("RTI", (byte) 0, (byte) 6, (byte) 1, (byte) 6, ()->{RTI();});
		instructions[65] = new Instruction("EOR", (byte) 0, (byte) 6, (byte) 2, (byte) 7, ()->{EOR();});
		instructions[66] = new Instruction("KIL", (byte) 0, (byte) 2, (byte) 0, (byte) 6, ()->{KIL();});
		instructions[67] = new Instruction("SRE", (byte) 0, (byte) 8, (byte) 0, (byte) 7, ()->{SRE();});
		instructions[68] = new Instruction("NOP", (byte) 0, (byte) 3, (byte) 2, (byte) 11, ()->{NOP();});
		instructions[69] = new Instruction("EOR", (byte) 0, (byte) 3, (byte) 2, (byte) 11, ()->{EOR();});
		instructions[70] = new Instruction("LSR", (byte) 0, (byte) 5, (byte) 2, (byte) 11, ()->{LSR();});
		instructions[71] = new Instruction("SRE", (byte) 0, (byte) 5, (byte) 0, (byte) 11, ()->{SRE();});
		instructions[72] = new Instruction("PHA", (byte) 0, (byte) 3, (byte) 1, (byte) 6, ()->{PHA();});
		instructions[73] = new Instruction("EOR", (byte) 0, (byte) 2, (byte) 2, (byte) 5, ()->{EOR();});
		instructions[74] = new Instruction("LSR", (byte) 0, (byte) 2, (byte) 1, (byte) 4, ()->{LSR();});
		instructions[75] = new Instruction("ALR", (byte) 0, (byte) 2, (byte) 0, (byte) 5, ()->{ALR();});
		instructions[76] = new Instruction("JMP", (byte) 0, (byte) 3, (byte) 3, (byte) 1, ()->{JMP();});
		instructions[77] = new Instruction("EOR", (byte) 0, (byte) 4, (byte) 3, (byte) 1, ()->{EOR();});
		instructions[78] = new Instruction("LSR", (byte) 0, (byte) 6, (byte) 3, (byte) 1, ()->{LSR();});
		instructions[79] = new Instruction("SRE", (byte) 0, (byte) 6, (byte) 0, (byte) 1, ()->{SRE();});
		instructions[80] = new Instruction("BVC", (byte) 1, (byte) 2, (byte) 2, (byte) 10, ()->{BVC();});
		instructions[81] = new Instruction("EOR", (byte) 1, (byte) 5, (byte) 2, (byte) 9, ()->{EOR();});
		instructions[82] = new Instruction("KIL", (byte) 0, (byte) 2, (byte) 0, (byte) 6, ()->{KIL();});
		instructions[83] = new Instruction("SRE", (byte) 0, (byte) 8, (byte) 0, (byte) 9, ()->{SRE();});
		instructions[84] = new Instruction("NOP", (byte) 0, (byte) 4, (byte) 2, (byte) 12, ()->{NOP();});
		instructions[85] = new Instruction("EOR", (byte) 0, (byte) 4, (byte) 2, (byte) 12, ()->{EOR();});
		instructions[86] = new Instruction("LSR", (byte) 0, (byte) 6, (byte) 2, (byte) 12, ()->{LSR();});
		instructions[87] = new Instruction("SRE", (byte) 0, (byte) 6, (byte) 0, (byte) 12, ()->{SRE();});
		instructions[88] = new Instruction("CLI", (byte) 0, (byte) 2, (byte) 1, (byte) 6, ()->{CLI();});
		instructions[89] = new Instruction("EOR", (byte) 1, (byte) 4, (byte) 3, (byte) 3, ()->{EOR();});
		instructions[90] = new Instruction("NOP", (byte) 0, (byte) 2, (byte) 1, (byte) 6, ()->{NOP();});
		instructions[91] = new Instruction("SRE", (byte) 0, (byte) 7, (byte) 0, (byte) 3, ()->{SRE();});
		instructions[92] = new Instruction("NOP", (byte) 1, (byte) 4, (byte) 3, (byte) 2, ()->{NOP();});
		instructions[93] = new Instruction("EOR", (byte) 1, (byte) 4, (byte) 3, (byte) 2, ()->{EOR();});
		instructions[94] = new Instruction("LSR", (byte) 0, (byte) 7, (byte) 3, (byte) 2, ()->{LSR();});
		instructions[95] = new Instruction("SRE", (byte) 0, (byte) 7, (byte) 0, (byte) 2, ()->{SRE();});
		instructions[96] = new Instruction("RTS", (byte) 0, (byte) 6, (byte) 1, (byte) 6, ()->{RTS();});
		instructions[97] = new Instruction("ADC", (byte) 0, (byte) 6, (byte) 2, (byte) 7, ()->{ADC();});
		instructions[98] = new Instruction("KIL", (byte) 0, (byte) 2, (byte) 0, (byte) 6, ()->{KIL();});
		instructions[99] = new Instruction("RRA", (byte) 0, (byte) 8, (byte) 0, (byte) 7, ()->{RRA();});
		instructions[100] = new Instruction("NOP", (byte) 0, (byte) 3, (byte) 2, (byte) 11, ()->{NOP();});
		instructions[101] = new Instruction("ADC", (byte) 0, (byte) 3, (byte) 2, (byte) 11, ()->{ADC();});
		instructions[102] = new Instruction("ROR", (byte) 0, (byte) 5, (byte) 2, (byte) 11, ()->{ROR();});
		instructions[103] = new Instruction("RRA", (byte) 0, (byte) 5, (byte) 0, (byte) 11, ()->{RRA();});
		instructions[104] = new Instruction("PLA", (byte) 0, (byte) 4, (byte) 1, (byte) 6, ()->{PLA();});
		instructions[105] = new Instruction("ADC", (byte) 0, (byte) 2, (byte) 2, (byte) 5, ()->{ADC();});
		instructions[106] = new Instruction("ROR", (byte) 0, (byte) 2, (byte) 1, (byte) 4, ()->{ROR();});
		instructions[107] = new Instruction("ARR", (byte) 0, (byte) 2, (byte) 0, (byte) 5, ()->{ARR();});
		instructions[108] = new Instruction("JMP", (byte) 0, (byte) 5, (byte) 3, (byte) 8, ()->{JMP();});
		instructions[109] = new Instruction("ADC", (byte) 0, (byte) 4, (byte) 3, (byte) 1, ()->{ADC();});
		instructions[110] = new Instruction("ROR", (byte) 0, (byte) 6, (byte) 3, (byte) 1, ()->{ROR();});
		instructions[111] = new Instruction("RRA", (byte) 0, (byte) 6, (byte) 0, (byte) 1, ()->{RRA();});
		instructions[112] = new Instruction("BVS", (byte) 1, (byte) 2, (byte) 2, (byte) 10, ()->{BVS();});
		instructions[113] = new Instruction("ADC", (byte) 1, (byte) 5, (byte) 2, (byte) 9, ()->{ADC();});
		instructions[114] = new Instruction("KIL", (byte) 0, (byte) 2, (byte) 0, (byte) 6, ()->{KIL();});
		instructions[115] = new Instruction("RRA", (byte) 0, (byte) 8, (byte) 0, (byte) 9, ()->{RRA();});
		instructions[116] = new Instruction("NOP", (byte) 0, (byte) 4, (byte) 2, (byte) 12, ()->{NOP();});
		instructions[117] = new Instruction("ADC", (byte) 0, (byte) 4, (byte) 2, (byte) 12, ()->{ADC();});
		instructions[118] = new Instruction("ROR", (byte) 0, (byte) 6, (byte) 2, (byte) 12, ()->{ROR();});
		instructions[119] = new Instruction("RRA", (byte) 0, (byte) 6, (byte) 0, (byte) 12, ()->{RRA();});
		instructions[120] = new Instruction("SEI", (byte) 0, (byte) 2, (byte) 1, (byte) 6, ()->{SEI();});
		instructions[121] = new Instruction("ADC", (byte) 1, (byte) 4, (byte) 3, (byte) 3, ()->{ADC();});
		instructions[122] = new Instruction("NOP", (byte) 0, (byte) 2, (byte) 1, (byte) 6, ()->{NOP();});
		instructions[123] = new Instruction("RRA", (byte) 0, (byte) 7, (byte) 0, (byte) 3, ()->{RRA();});
		instructions[124] = new Instruction("NOP", (byte) 1, (byte) 4, (byte) 3, (byte) 2, ()->{NOP();});
		instructions[125] = new Instruction("ADC", (byte) 1, (byte) 4, (byte) 3, (byte) 2, ()->{ADC();});
		instructions[126] = new Instruction("ROR", (byte) 0, (byte) 7, (byte) 3, (byte) 2, ()->{ROR();});
		instructions[127] = new Instruction("RRA", (byte) 0, (byte) 7, (byte) 0, (byte) 2, ()->{RRA();});
		instructions[128] = new Instruction("NOP", (byte) 0, (byte) 2, (byte) 2, (byte) 5, ()->{NOP();});
		instructions[129] = new Instruction("STA", (byte) 0, (byte) 6, (byte) 2, (byte) 7, ()->{STA();});
		instructions[130] = new Instruction("NOP", (byte) 0, (byte) 2, (byte) 0, (byte) 5, ()->{NOP();});
		instructions[131] = new Instruction("SAX", (byte) 0, (byte) 6, (byte) 0, (byte) 7, ()->{SAX();});
		instructions[132] = new Instruction("STY", (byte) 0, (byte) 3, (byte) 2, (byte) 11, ()->{STY();});
		instructions[133] = new Instruction("STA", (byte) 0, (byte) 3, (byte) 2, (byte) 11, ()->{STA();});
		instructions[134] = new Instruction("STX", (byte) 0, (byte) 3, (byte) 2, (byte) 11, ()->{STX();});
		instructions[135] = new Instruction("SAX", (byte) 0, (byte) 3, (byte) 0, (byte) 11, ()->{SAX();});
		instructions[136] = new Instruction("DEY", (byte) 0, (byte) 2, (byte) 1, (byte) 6, ()->{DEY();});
		instructions[137] = new Instruction("NOP", (byte) 0, (byte) 2, (byte) 0, (byte) 5, ()->{NOP();});
		instructions[138] = new Instruction("TXA", (byte) 0, (byte) 2, (byte) 1, (byte) 6, ()->{TXA();});
		instructions[139] = new Instruction("XAA", (byte) 0, (byte) 2, (byte) 0, (byte) 5, ()->{XAA();});
		instructions[140] = new Instruction("STY", (byte) 0, (byte) 4, (byte) 3, (byte) 1, ()->{STY();});
		instructions[141] = new Instruction("STA", (byte) 0, (byte) 4, (byte) 3, (byte) 1, ()->{STA();});
		instructions[142] = new Instruction("STX", (byte) 0, (byte) 4, (byte) 3, (byte) 1, ()->{STX();});
		instructions[143] = new Instruction("SAX", (byte) 0, (byte) 4, (byte) 0, (byte) 1, ()->{SAX();});
		instructions[144] = new Instruction("BCC", (byte) 1, (byte) 2, (byte) 2, (byte) 10, ()->{BCC();});
		instructions[145] = new Instruction("STA", (byte) 0, (byte) 6, (byte) 2, (byte) 9, ()->{STA();});
		instructions[146] = new Instruction("KIL", (byte) 0, (byte) 2, (byte) 0, (byte) 6, ()->{KIL();});
		instructions[147] = new Instruction("AHX", (byte) 0, (byte) 6, (byte) 0, (byte) 9, ()->{AHX();});
		instructions[148] = new Instruction("STY", (byte) 0, (byte) 4, (byte) 2, (byte) 12, ()->{STY();});
		instructions[149] = new Instruction("STA", (byte) 0, (byte) 4, (byte) 2, (byte) 12, ()->{STA();});
		instructions[150] = new Instruction("STX", (byte) 0, (byte) 4, (byte) 2, (byte) 13, ()->{STX();});
		instructions[151] = new Instruction("SAX", (byte) 0, (byte) 4, (byte) 0, (byte) 13, ()->{SAX();});
		instructions[152] = new Instruction("TYA", (byte) 0, (byte) 2, (byte) 1, (byte) 6, ()->{TYA();});
		instructions[153] = new Instruction("STA", (byte) 0, (byte) 5, (byte) 3, (byte) 3, ()->{STA();});
		instructions[154] = new Instruction("TXS", (byte) 0, (byte) 2, (byte) 1, (byte) 6, ()->{TXS();});
		instructions[155] = new Instruction("TAS", (byte) 0, (byte) 5, (byte) 0, (byte) 3, ()->{TAS();});
		instructions[156] = new Instruction("SHY", (byte) 0, (byte) 5, (byte) 0, (byte) 2, ()->{SHY();});
		instructions[157] = new Instruction("STA", (byte) 0, (byte) 5, (byte) 3, (byte) 2, ()->{STA();});
		instructions[158] = new Instruction("SHX", (byte) 0, (byte) 5, (byte) 0, (byte) 3, ()->{SHX();});
		instructions[159] = new Instruction("AHX", (byte) 0, (byte) 5, (byte) 0, (byte) 3, ()->{AHX();});
		instructions[160] = new Instruction("LDY", (byte) 0, (byte) 2, (byte) 2, (byte) 5, ()->{LDY();});
		instructions[161] = new Instruction("LDA", (byte) 0, (byte) 6, (byte) 2, (byte) 7, ()->{LDA();});
		instructions[162] = new Instruction("LDX", (byte) 0, (byte) 2, (byte) 2, (byte) 5, ()->{LDX();});
		instructions[163] = new Instruction("LAX", (byte) 0, (byte) 6, (byte) 0, (byte) 7, ()->{LAX();});
		instructions[164] = new Instruction("LDY", (byte) 0, (byte) 3, (byte) 2, (byte) 11, ()->{LDY();});
		instructions[165] = new Instruction("LDA", (byte) 0, (byte) 3, (byte) 2, (byte) 11, ()->{LDA();});
		instructions[166] = new Instruction("LDX", (byte) 0, (byte) 3, (byte) 2, (byte) 11, ()->{LDX();});
		instructions[167] = new Instruction("LAX", (byte) 0, (byte) 3, (byte) 0, (byte) 11, ()->{LAX();});
		instructions[168] = new Instruction("TAY", (byte) 0, (byte) 2, (byte) 1, (byte) 6, ()->{TAY();});
		instructions[169] = new Instruction("LDA", (byte) 0, (byte) 2, (byte) 2, (byte) 5, ()->{LDA();});
		instructions[170] = new Instruction("TAX", (byte) 0, (byte) 2, (byte) 1, (byte) 6, ()->{TAX();});
		instructions[171] = new Instruction("LAX", (byte) 0, (byte) 2, (byte) 0, (byte) 5, ()->{LAX();});
		instructions[172] = new Instruction("LDY", (byte) 0, (byte) 4, (byte) 3, (byte) 1, ()->{LDY();});
		instructions[173] = new Instruction("LDA", (byte) 0, (byte) 4, (byte) 3, (byte) 1, ()->{LDA();});
		instructions[174] = new Instruction("LDX", (byte) 0, (byte) 4, (byte) 3, (byte) 1, ()->{LDX();});
		instructions[175] = new Instruction("LAX", (byte) 0, (byte) 4, (byte) 0, (byte) 1, ()->{LAX();});
		instructions[176] = new Instruction("BCS", (byte) 1, (byte) 2, (byte) 2, (byte) 10, ()->{BCS();});
		instructions[177] = new Instruction("LDA", (byte) 1, (byte) 5, (byte) 2, (byte) 9, ()->{LDA();});
		instructions[178] = new Instruction("KIL", (byte) 0, (byte) 2, (byte) 0, (byte) 6, ()->{KIL();});
		instructions[179] = new Instruction("LAX", (byte) 1, (byte) 5, (byte) 0, (byte) 9, ()->{LAX();});
		instructions[180] = new Instruction("LDY", (byte) 0, (byte) 4, (byte) 2, (byte) 12, ()->{LDY();});
		instructions[181] = new Instruction("LDA", (byte) 0, (byte) 4, (byte) 2, (byte) 12, ()->{LDA();});
		instructions[182] = new Instruction("LDX", (byte) 0, (byte) 4, (byte) 2, (byte) 13, ()->{LDX();});
		instructions[183] = new Instruction("LAX", (byte) 0, (byte) 4, (byte) 0, (byte) 13, ()->{LAX();});
		instructions[184] = new Instruction("CLV", (byte) 0, (byte) 2, (byte) 1, (byte) 6, ()->{CLV();});
		instructions[185] = new Instruction("LDA", (byte) 1, (byte) 4, (byte) 3, (byte) 3, ()->{LDA();});
		instructions[186] = new Instruction("TSX", (byte) 0, (byte) 2, (byte) 1, (byte) 6, ()->{TSX();});
		instructions[187] = new Instruction("LAS", (byte) 1, (byte) 4, (byte) 0, (byte) 3, ()->{LAS();});
		instructions[188] = new Instruction("LDY", (byte) 1, (byte) 4, (byte) 3, (byte) 2, ()->{LDY();});
		instructions[189] = new Instruction("LDA", (byte) 1, (byte) 4, (byte) 3, (byte) 2, ()->{LDA();});
		instructions[190] = new Instruction("LDX", (byte) 1, (byte) 4, (byte) 3, (byte) 3, ()->{LDX();});
		instructions[191] = new Instruction("LAX", (byte) 1, (byte) 4, (byte) 0, (byte) 3, ()->{LAX();});
		instructions[192] = new Instruction("CPY", (byte) 0, (byte) 2, (byte) 2, (byte) 5, ()->{CPY();});
		instructions[193] = new Instruction("CMP", (byte) 0, (byte) 6, (byte) 2, (byte) 7, ()->{CMP();});
		instructions[194] = new Instruction("NOP", (byte) 0, (byte) 2, (byte) 0, (byte) 5, ()->{NOP();});
		instructions[195] = new Instruction("DCP", (byte) 0, (byte) 8, (byte) 0, (byte) 7, ()->{DCP();});
		instructions[196] = new Instruction("CPY", (byte) 0, (byte) 3, (byte) 2, (byte) 11, ()->{CPY();});
		instructions[197] = new Instruction("CMP", (byte) 0, (byte) 3, (byte) 2, (byte) 11, ()->{CMP();});
		instructions[198] = new Instruction("DEC", (byte) 0, (byte) 5, (byte) 2, (byte) 11, ()->{DEC();});
		instructions[199] = new Instruction("DCP", (byte) 0, (byte) 5, (byte) 0, (byte) 11, ()->{DCP();});
		instructions[200] = new Instruction("INY", (byte) 0, (byte) 2, (byte) 1, (byte) 6, ()->{INY();});
		instructions[201] = new Instruction("CMP", (byte) 0, (byte) 2, (byte) 2, (byte) 5, ()->{CMP();});
		instructions[202] = new Instruction("DEX", (byte) 0, (byte) 2, (byte) 1, (byte) 6, ()->{DEX();});
		instructions[203] = new Instruction("AXS", (byte) 0, (byte) 2, (byte) 0, (byte) 5, ()->{AXS();});
		instructions[204] = new Instruction("CPY", (byte) 0, (byte) 4, (byte) 3, (byte) 1, ()->{CPY();});
		instructions[205] = new Instruction("CMP", (byte) 0, (byte) 4, (byte) 3, (byte) 1, ()->{CMP();});
		instructions[206] = new Instruction("DEC", (byte) 0, (byte) 6, (byte) 3, (byte) 1, ()->{DEC();});
		instructions[207] = new Instruction("DCP", (byte) 0, (byte) 6, (byte) 0, (byte) 1, ()->{DCP();});
		instructions[208] = new Instruction("BNE", (byte) 1, (byte) 2, (byte) 2, (byte) 10, ()->{BNE();});
		instructions[209] = new Instruction("CMP", (byte) 1, (byte) 5, (byte) 2, (byte) 9, ()->{CMP();});
		instructions[210] = new Instruction("KIL", (byte) 0, (byte) 2, (byte) 0, (byte) 6, ()->{KIL();});
		instructions[211] = new Instruction("DCP", (byte) 0, (byte) 8, (byte) 0, (byte) 9, ()->{DCP();});
		instructions[212] = new Instruction("NOP", (byte) 0, (byte) 4, (byte) 2, (byte) 12, ()->{NOP();});
		instructions[213] = new Instruction("CMP", (byte) 0, (byte) 4, (byte) 2, (byte) 12, ()->{CMP();});
		instructions[214] = new Instruction("DEC", (byte) 0, (byte) 6, (byte) 2, (byte) 12, ()->{DEC();});
		instructions[215] = new Instruction("DCP", (byte) 0, (byte) 6, (byte) 0, (byte) 12, ()->{DCP();});
		instructions[216] = new Instruction("CLD", (byte) 0, (byte) 2, (byte) 1, (byte) 6, ()->{CLD();});
		instructions[217] = new Instruction("CMP", (byte) 1, (byte) 4, (byte) 3, (byte) 3, ()->{CMP();});
		instructions[218] = new Instruction("NOP", (byte) 0, (byte) 2, (byte) 1, (byte) 6, ()->{NOP();});
		instructions[219] = new Instruction("DCP", (byte) 0, (byte) 7, (byte) 0, (byte) 3, ()->{DCP();});
		instructions[220] = new Instruction("NOP", (byte) 1, (byte) 4, (byte) 3, (byte) 2, ()->{NOP();});
		instructions[221] = new Instruction("CMP", (byte) 1, (byte) 4, (byte) 3, (byte) 2, ()->{CMP();});
		instructions[222] = new Instruction("DEC", (byte) 0, (byte) 7, (byte) 3, (byte) 2, ()->{DEC();});
		instructions[223] = new Instruction("DCP", (byte) 0, (byte) 7, (byte) 0, (byte) 2, ()->{DCP();});
		instructions[224] = new Instruction("CPX", (byte) 0, (byte) 2, (byte) 2, (byte) 5, ()->{CPX();});
		instructions[225] = new Instruction("SBC", (byte) 0, (byte) 6, (byte) 2, (byte) 7, ()->{SBC();});
		instructions[226] = new Instruction("NOP", (byte) 0, (byte) 2, (byte) 0, (byte) 5, ()->{NOP();});
		instructions[227] = new Instruction("ISC", (byte) 0, (byte) 8, (byte) 0, (byte) 7, ()->{ISC();});
		instructions[228] = new Instruction("CPX", (byte) 0, (byte) 3, (byte) 2, (byte) 11, ()->{CPX();});
		instructions[229] = new Instruction("SBC", (byte) 0, (byte) 3, (byte) 2, (byte) 11, ()->{SBC();});
		instructions[230] = new Instruction("INC", (byte) 0, (byte) 5, (byte) 2, (byte) 11, ()->{INC();});
		instructions[231] = new Instruction("ISC", (byte) 0, (byte) 5, (byte) 0, (byte) 11, ()->{ISC();});
		instructions[232] = new Instruction("INX", (byte) 0, (byte) 2, (byte) 1, (byte) 6, ()->{INX();});
		instructions[233] = new Instruction("SBC", (byte) 0, (byte) 2, (byte) 2, (byte) 5, ()->{SBC();});
		instructions[234] = new Instruction("NOP", (byte) 0, (byte) 2, (byte) 1, (byte) 6, ()->{NOP();});
		instructions[235] = new Instruction("SBC", (byte) 0, (byte) 2, (byte) 0, (byte) 5, ()->{SBC();});
		instructions[236] = new Instruction("CPX", (byte) 0, (byte) 4, (byte) 3, (byte) 1, ()->{CPX();});
		instructions[237] = new Instruction("SBC", (byte) 0, (byte) 4, (byte) 3, (byte) 1, ()->{SBC();});
		instructions[238] = new Instruction("INC", (byte) 0, (byte) 6, (byte) 3, (byte) 1, ()->{INC();});
		instructions[239] = new Instruction("ISC", (byte) 0, (byte) 6, (byte) 0, (byte) 1, ()->{ISC();});
		instructions[240] = new Instruction("BEQ", (byte) 1, (byte) 2, (byte) 2, (byte) 10, ()->{BEQ();});
		instructions[241] = new Instruction("SBC", (byte) 1, (byte) 5, (byte) 2, (byte) 9, ()->{SBC();});
		instructions[242] = new Instruction("KIL", (byte) 0, (byte) 2, (byte) 0, (byte) 6, ()->{KIL();});
		instructions[243] = new Instruction("ISC", (byte) 0, (byte) 8, (byte) 0, (byte) 9, ()->{ISC();});
		instructions[244] = new Instruction("NOP", (byte) 0, (byte) 4, (byte) 2, (byte) 12, ()->{NOP();});
		instructions[245] = new Instruction("SBC", (byte) 0, (byte) 4, (byte) 2, (byte) 12, ()->{SBC();});
		instructions[246] = new Instruction("INC", (byte) 0, (byte) 6, (byte) 2, (byte) 12, ()->{INC();});
		instructions[247] = new Instruction("ISC", (byte) 0, (byte) 6, (byte) 0, (byte) 12, ()->{ISC();});
		instructions[248] = new Instruction("SED", (byte) 0, (byte) 2, (byte) 1, (byte) 6, ()->{SED();});
		instructions[249] = new Instruction("SBC", (byte) 1, (byte) 4, (byte) 3, (byte) 3, ()->{SBC();});
		instructions[250] = new Instruction("NOP", (byte) 0, (byte) 2, (byte) 1, (byte) 6, ()->{NOP();});
		instructions[251] = new Instruction("ISC", (byte) 0, (byte) 7, (byte) 0, (byte) 3, ()->{ISC();});
		instructions[252] = new Instruction("NOP", (byte) 1, (byte) 4, (byte) 3, (byte) 2, ()->{NOP();});
		instructions[253] = new Instruction("SBC", (byte) 1, (byte) 4, (byte) 3, (byte) 2, ()->{SBC();});
		instructions[254] = new Instruction("INC", (byte) 0, (byte) 7, (byte) 3, (byte) 2, ()->{INC();});
		instructions[255] = new Instruction("ISC", (byte) 0, (byte) 7, (byte) 0, (byte) 2, ()->{ISC();});

	}

	private void BRK() {

	}

	void reset() {
		// set PC
		// set SP
		// set flags
	}

	void step() {
		// Step 1, we may be waiting some cycles for some reason
		// cpu stalled

		// Step 2, we should handle interupts
		// something may have set an interupt, we should handle

		// Step 3, decode opcode by looking at the current program counter
		byte opcode = ram.read(PC);

//		Instruction inst = instructions[ram.read(PC)];
		// instructionmode mode decode

		// Step 4, fetch data per memory accessmode

		// Step 4.5, decode addressing mode

		// Step 5, execute instruction
		;

		// step 4
		// set $pc to next instruction
		PC++;

		// step 5
		// fetch data per memory access mode

		// step 6
		// execute instruction with memory fetched
		instructions[opcode].nesInst.execute();
	}

	public static void main(String[] args) {
		System.out.println("Hello world!");

		String instructionModes = "6,7,6,7,11,11,11,11,6,5,4,5,1,1,1,1,10,9,6,9,12,12,12,12,6,3,6,3,2,2,2,2,1,7,6,7,11,11,11,11,6,5,4,5,1,1,1,1,10,9,6,9,12,12,12,12,6,3,6,3,2,2,2,2,6,7,6,7,11,11,11,11,6,5,4,5,1,1,1,1,10,9,6,9,12,12,12,12,6,3,6,3,2,2,2,2,6,7,6,7,11,11,11,11,6,5,4,5,8,1,1,1,10,9,6,9,12,12,12,12,6,3,6,3,2,2,2,2,5,7,5,7,11,11,11,11,6,5,6,5,1,1,1,1,10,9,6,9,12,12,13,13,6,3,6,3,2,2,3,3,5,7,5,7,11,11,11,11,6,5,6,5,1,1,1,1,10,9,6,9,12,12,13,13,6,3,6,3,2,2,3,3,5,7,5,7,11,11,11,11,6,5,6,5,1,1,1,1,10,9,6,9,12,12,12,12,6,3,6,3,2,2,2,2,5,7,5,7,11,11,11,11,6,5,6,5,1,1,1,1,10,9,6,9,12,12,12,12,6,3,6,3,2,2,2,2";
		String instructionSizes = "2,2,0,0,2,2,2,0,1,2,1,0,3,3,3,0,2,2,0,0,2,2,2,0,1,3,1,0,3,3,3,0,3,2,0,0,2,2,2,0,1,2,1,0,3,3,3,0,2,2,0,0,2,2,2,0,1,3,1,0,3,3,3,0,1,2,0,0,2,2,2,0,1,2,1,0,3,3,3,0,2,2,0,0,2,2,2,0,1,3,1,0,3,3,3,0,1,2,0,0,2,2,2,0,1,2,1,0,3,3,3,0,2,2,0,0,2,2,2,0,1,3,1,0,3,3,3,0,2,2,0,0,2,2,2,0,1,0,1,0,3,3,3,0,2,2,0,0,2,2,2,0,1,3,1,0,0,3,0,0,2,2,2,0,2,2,2,0,1,2,1,0,3,3,3,0,2,2,0,0,2,2,2,0,1,3,1,0,3,3,3,0,2,2,0,0,2,2,2,0,1,2,1,0,3,3,3,0,2,2,0,0,2,2,2,0,1,3,1,0,3,3,3,0,2,2,0,0,2,2,2,0,1,2,1,0,3,3,3,0,2,2,0,0,2,2,2,0,1,3,1,0,3,3,3,0";
		String instructionCycles = "7,6,2,8,3,3,5,5,3,2,2,2,4,4,6,6,2,5,2,8,4,4,6,6,2,4,2,7,4,4,7,7,6,6,2,8,3,3,5,5,4,2,2,2,4,4,6,6,2,5,2,8,4,4,6,6,2,4,2,7,4,4,7,7,6,6,2,8,3,3,5,5,3,2,2,2,3,4,6,6,2,5,2,8,4,4,6,6,2,4,2,7,4,4,7,7,6,6,2,8,3,3,5,5,4,2,2,2,5,4,6,6,2,5,2,8,4,4,6,6,2,4,2,7,4,4,7,7,2,6,2,6,3,3,3,3,2,2,2,2,4,4,4,4,2,6,2,6,4,4,4,4,2,5,2,5,5,5,5,5,2,6,2,6,3,3,3,3,2,2,2,2,4,4,4,4,2,5,2,5,4,4,4,4,2,4,2,4,4,4,4,4,2,6,2,8,3,3,5,5,2,2,2,2,4,4,6,6,2,5,2,8,4,4,6,6,2,4,2,7,4,4,7,7,2,6,2,8,3,3,5,5,2,2,2,2,4,4,6,6,2,5,2,8,4,4,6,6,2,4,2,7,4,4,7,7";
		String instructionPageCycles = "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,0,0,0,0,0,1,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,0,0,1,1,0,0";
		String instructionNames = "BRK,ORA,KIL,SLO,NOP,ORA,ASL,SLO,PHP,ORA,ASL,ANC,NOP,ORA,ASL,SLO,BPL,ORA,KIL,SLO,NOP,ORA,ASL,SLO,CLC,ORA,NOP,SLO,NOP,ORA,ASL,SLO,JSR,AND,KIL,RLA,BIT,AND,ROL,RLA,PLP,AND,ROL,ANC,BIT,AND,ROL,RLA,BMI,AND,KIL,RLA,NOP,AND,ROL,RLA,SEC,AND,NOP,RLA,NOP,AND,ROL,RLA,RTI,EOR,KIL,SRE,NOP,EOR,LSR,SRE,PHA,EOR,LSR,ALR,JMP,EOR,LSR,SRE,BVC,EOR,KIL,SRE,NOP,EOR,LSR,SRE,CLI,EOR,NOP,SRE,NOP,EOR,LSR,SRE,RTS,ADC,KIL,RRA,NOP,ADC,ROR,RRA,PLA,ADC,ROR,ARR,JMP,ADC,ROR,RRA,BVS,ADC,KIL,RRA,NOP,ADC,ROR,RRA,SEI,ADC,NOP,RRA,NOP,ADC,ROR,RRA,NOP,STA,NOP,SAX,STY,STA,STX,SAX,DEY,NOP,TXA,XAA,STY,STA,STX,SAX,BCC,STA,KIL,AHX,STY,STA,STX,SAX,TYA,STA,TXS,TAS,SHY,STA,SHX,AHX,LDY,LDA,LDX,LAX,LDY,LDA,LDX,LAX,TAY,LDA,TAX,LAX,LDY,LDA,LDX,LAX,BCS,LDA,KIL,LAX,LDY,LDA,LDX,LAX,CLV,LDA,TSX,LAS,LDY,LDA,LDX,LAX,CPY,CMP,NOP,DCP,CPY,CMP,DEC,DCP,INY,CMP,DEX,AXS,CPY,CMP,DEC,DCP,BNE,CMP,KIL,DCP,NOP,CMP,DEC,DCP,CLD,CMP,NOP,DCP,NOP,CMP,DEC,DCP,CPX,SBC,NOP,ISC,CPX,SBC,INC,ISC,INX,SBC,NOP,SBC,CPX,SBC,INC,ISC,BEQ,SBC,KIL,ISC,NOP,SBC,INC,ISC,SED,SBC,NOP,ISC,NOP,SBC,INC,ISC";

		String[] i1 = instructionModes.split(",");
		String[] i2 = instructionSizes.split(",");
		String[] i3 = instructionCycles.split(",");
		String[] i4 = instructionPageCycles.split(",");
		String[] i5 = instructionNames.split(",");

		for (int i = 0; i < 256; i++) {
			System.out.println("instructions[" + i + "] = new Instruction(\"" + i5[i] + "\", (byte)" + i4[i] + ", (byte)" + i3[i] + ", (byte)" + i2[i] + ", (byte)" + i1[i] + ");");
		}


	}

	Instruction[] instructions = new Instruction[256];
//	instructions[0] = new Instruction("BRK", 7, 1, 1, 0);

	interface NesInst {
		void execute();
	}

	static class Instruction {
		Instruction(String name, byte instructionPageCycles, byte instructionCycles, byte instructionSize, byte instructionAddressingMode, NesInst nesInst) {
			this.name = name;
			this.instructionPageCycles = instructionPageCycles;
			this.instructionCycles = instructionCycles;
			this.instructionSize = instructionSize;
			this.instructionAddressingMode = instructionAddressingMode;
			this.nesInst = nesInst;
		}

		// the opcode name
		String name;

		// the number of cycles this instruction takes when a page is crossed
		byte instructionPageCycles;

		// the number of cycles this instruction takes
		byte instructionCycles;

		// the size of the instruction in bytes
		byte instructionSize;

		// the addressing mode for each instruction
		byte instructionAddressingMode;

		// the function to execute
		NesInst nesInst;


	}

	enum Instructionc {

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
		// The arithmetic operations perform addition and subtraction on the contents of the accumulator. The compare
		// operations allow the comparison of the accumulator and X or Y with memory values.
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
		RTI;  // Return from Interrupt

		void decode() {
		}

	}

	enum addressing_mode {
		INDEXED_D_X,       // val = PEEK((arg + X) % 256)
		INDEXED_D_Y,       // val = PEEK((arg + Y) % 256)
		INDEXED_A_X,       // val = PEEK(arg + X)
		INDEXED_A_Y,       // val = PEEK(arg + Y)
		INDEXED_INDIRECT,  // val = PEEK(PEEK((arg + X) % 256) + PEEK((arg + X + 1) % 256) * 256)
		INDIRECT_INDEXED,  // val = PEEK(PEEK(arg) + PEEK((arg + 1) % 256) * 256 + Y)
	}

}