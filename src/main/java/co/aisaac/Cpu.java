package co.aisaac;

public class Cpu {

	double cycles;

	// program counter
	short PC = 0;
	// stack pointer
	byte SP = 0;
	// accumulator
	byte A = 0;
	// general
	byte X = 0;
	// general
	byte Y = 0;

	// status flags
	byte C = 0; // carry
	byte Z = 0; // zero
	byte I = 0; // interrupt disable
	byte D = 0; // decimal mode
	byte B = 0; // break command
	byte U = 0; // unused flag
	byte V = 0; // overflow
	byte N = 0; // negative flag

	final Ram ram;

	// for use by instructions
	short address;
	byte mode;


	Instruction[] instructions = new Instruction[256];

	Cpu(Ram ram) {
		this.ram = ram;
		createTable();
		reset();
	}

	private void createTable() {
		instructions[0] = new Instruction("BRK", (byte) 0, (byte) 7, (byte) 2, (byte) 6, this::BRK);
		instructions[1] = new Instruction("ORA", (byte) 0, (byte) 6, (byte) 2, (byte) 7, this::ORA);
		instructions[2] = new Instruction("KIL", (byte) 0, (byte) 2, (byte) 0, (byte) 6, this::KIL);
		instructions[3] = new Instruction("SLO", (byte) 0, (byte) 8, (byte) 0, (byte) 7, this::SLO);
		instructions[4] = new Instruction("NOP", (byte) 0, (byte) 3, (byte) 2, (byte) 11, this::NOP);
		instructions[5] = new Instruction("ORA", (byte) 0, (byte) 3, (byte) 2, (byte) 11, this::ORA);
		instructions[6] = new Instruction("ASL", (byte) 0, (byte) 5, (byte) 2, (byte) 11, this::ASL);
		instructions[7] = new Instruction("SLO", (byte) 0, (byte) 5, (byte) 0, (byte) 11, this::SLO);
		instructions[8] = new Instruction("PHP", (byte) 0, (byte) 3, (byte) 1, (byte) 6, this::PHP);
		instructions[9] = new Instruction("ORA", (byte) 0, (byte) 2, (byte) 2, (byte) 5, this::ORA);
		instructions[10] = new Instruction("ASL", (byte) 0, (byte) 2, (byte) 1, (byte) 4, this::ASL);
		instructions[11] = new Instruction("ANC", (byte) 0, (byte) 2, (byte) 0, (byte) 5, this::ANC);
		instructions[12] = new Instruction("NOP", (byte) 0, (byte) 4, (byte) 3, (byte) 1, this::NOP);
		instructions[13] = new Instruction("ORA", (byte) 0, (byte) 4, (byte) 3, (byte) 1, this::ORA);
		instructions[14] = new Instruction("ASL", (byte) 0, (byte) 6, (byte) 3, (byte) 1, this::ASL);
		instructions[15] = new Instruction("SLO", (byte) 0, (byte) 6, (byte) 0, (byte) 1, this::SLO);
		instructions[16] = new Instruction("BPL", (byte) 1, (byte) 2, (byte) 2, (byte) 10, this::BPL);
		instructions[17] = new Instruction("ORA", (byte) 1, (byte) 5, (byte) 2, (byte) 9, this::ORA);
		instructions[18] = new Instruction("KIL", (byte) 0, (byte) 2, (byte) 0, (byte) 6, this::KIL);
		instructions[19] = new Instruction("SLO", (byte) 0, (byte) 8, (byte) 0, (byte) 9, this::SLO);
		instructions[20] = new Instruction("NOP", (byte) 0, (byte) 4, (byte) 2, (byte) 12, this::NOP);
		instructions[21] = new Instruction("ORA", (byte) 0, (byte) 4, (byte) 2, (byte) 12, this::ORA);
		instructions[22] = new Instruction("ASL", (byte) 0, (byte) 6, (byte) 2, (byte) 12, this::ASL);
		instructions[23] = new Instruction("SLO", (byte) 0, (byte) 6, (byte) 0, (byte) 12, this::SLO);
		instructions[24] = new Instruction("CLC", (byte) 0, (byte) 2, (byte) 1, (byte) 6, this::CLC);
		instructions[25] = new Instruction("ORA", (byte) 1, (byte) 4, (byte) 3, (byte) 3, this::ORA);
		instructions[26] = new Instruction("NOP", (byte) 0, (byte) 2, (byte) 1, (byte) 6, this::NOP);
		instructions[27] = new Instruction("SLO", (byte) 0, (byte) 7, (byte) 0, (byte) 3, this::SLO);
		instructions[28] = new Instruction("NOP", (byte) 1, (byte) 4, (byte) 3, (byte) 2, this::NOP);
		instructions[29] = new Instruction("ORA", (byte) 1, (byte) 4, (byte) 3, (byte) 2, this::ORA);
		instructions[30] = new Instruction("ASL", (byte) 0, (byte) 7, (byte) 3, (byte) 2, this::ASL);
		instructions[31] = new Instruction("SLO", (byte) 0, (byte) 7, (byte) 0, (byte) 2, this::SLO);
		instructions[32] = new Instruction("JSR", (byte) 0, (byte) 6, (byte) 3, (byte) 1, this::JSR);
		instructions[33] = new Instruction("AND", (byte) 0, (byte) 6, (byte) 2, (byte) 7, this::AND);
		instructions[34] = new Instruction("KIL", (byte) 0, (byte) 2, (byte) 0, (byte) 6, this::KIL);
		instructions[35] = new Instruction("RLA", (byte) 0, (byte) 8, (byte) 0, (byte) 7, this::RLA);
		instructions[36] = new Instruction("BIT", (byte) 0, (byte) 3, (byte) 2, (byte) 11, this::BIT);
		instructions[37] = new Instruction("AND", (byte) 0, (byte) 3, (byte) 2, (byte) 11, this::AND);
		instructions[38] = new Instruction("ROL", (byte) 0, (byte) 5, (byte) 2, (byte) 11, this::ROL);
		instructions[39] = new Instruction("RLA", (byte) 0, (byte) 5, (byte) 0, (byte) 11, this::RLA);
		instructions[40] = new Instruction("PLP", (byte) 0, (byte) 4, (byte) 1, (byte) 6, this::PLP);
		instructions[41] = new Instruction("AND", (byte) 0, (byte) 2, (byte) 2, (byte) 5, this::AND);
		instructions[42] = new Instruction("ROL", (byte) 0, (byte) 2, (byte) 1, (byte) 4, this::ROL);
		instructions[43] = new Instruction("ANC", (byte) 0, (byte) 2, (byte) 0, (byte) 5, this::ANC);
		instructions[44] = new Instruction("BIT", (byte) 0, (byte) 4, (byte) 3, (byte) 1, this::BIT);
		instructions[45] = new Instruction("AND", (byte) 0, (byte) 4, (byte) 3, (byte) 1, this::AND);
		instructions[46] = new Instruction("ROL", (byte) 0, (byte) 6, (byte) 3, (byte) 1, this::ROL);
		instructions[47] = new Instruction("RLA", (byte) 0, (byte) 6, (byte) 0, (byte) 1, this::RLA);
		instructions[48] = new Instruction("BMI", (byte) 1, (byte) 2, (byte) 2, (byte) 10, this::BMI);
		instructions[49] = new Instruction("AND", (byte) 1, (byte) 5, (byte) 2, (byte) 9, this::AND);
		instructions[50] = new Instruction("KIL", (byte) 0, (byte) 2, (byte) 0, (byte) 6, this::KIL);
		instructions[51] = new Instruction("RLA", (byte) 0, (byte) 8, (byte) 0, (byte) 9, this::RLA);
		instructions[52] = new Instruction("NOP", (byte) 0, (byte) 4, (byte) 2, (byte) 12, this::NOP);
		instructions[53] = new Instruction("AND", (byte) 0, (byte) 4, (byte) 2, (byte) 12, this::AND);
		instructions[54] = new Instruction("ROL", (byte) 0, (byte) 6, (byte) 2, (byte) 12, this::ROL);
		instructions[55] = new Instruction("RLA", (byte) 0, (byte) 6, (byte) 0, (byte) 12, this::RLA);
		instructions[56] = new Instruction("SEC", (byte) 0, (byte) 2, (byte) 1, (byte) 6, this::SEC);
		instructions[57] = new Instruction("AND", (byte) 1, (byte) 4, (byte) 3, (byte) 3, this::AND);
		instructions[58] = new Instruction("NOP", (byte) 0, (byte) 2, (byte) 1, (byte) 6, this::NOP);
		instructions[59] = new Instruction("RLA", (byte) 0, (byte) 7, (byte) 0, (byte) 3, this::RLA);
		instructions[60] = new Instruction("NOP", (byte) 1, (byte) 4, (byte) 3, (byte) 2, this::NOP);
		instructions[61] = new Instruction("AND", (byte) 1, (byte) 4, (byte) 3, (byte) 2, this::AND);
		instructions[62] = new Instruction("ROL", (byte) 0, (byte) 7, (byte) 3, (byte) 2, this::ROL);
		instructions[63] = new Instruction("RLA", (byte) 0, (byte) 7, (byte) 0, (byte) 2, this::RLA);
		instructions[64] = new Instruction("RTI", (byte) 0, (byte) 6, (byte) 1, (byte) 6, this::RTI);
		instructions[65] = new Instruction("EOR", (byte) 0, (byte) 6, (byte) 2, (byte) 7, this::EOR);
		instructions[66] = new Instruction("KIL", (byte) 0, (byte) 2, (byte) 0, (byte) 6, this::KIL);
		instructions[67] = new Instruction("SRE", (byte) 0, (byte) 8, (byte) 0, (byte) 7, this::SRE);
		instructions[68] = new Instruction("NOP", (byte) 0, (byte) 3, (byte) 2, (byte) 11, this::NOP);
		instructions[69] = new Instruction("EOR", (byte) 0, (byte) 3, (byte) 2, (byte) 11, this::EOR);
		instructions[70] = new Instruction("LSR", (byte) 0, (byte) 5, (byte) 2, (byte) 11, this::LSR);
		instructions[71] = new Instruction("SRE", (byte) 0, (byte) 5, (byte) 0, (byte) 11, this::SRE);
		instructions[72] = new Instruction("PHA", (byte) 0, (byte) 3, (byte) 1, (byte) 6, this::PHA);
		instructions[73] = new Instruction("EOR", (byte) 0, (byte) 2, (byte) 2, (byte) 5, this::EOR);
		instructions[74] = new Instruction("LSR", (byte) 0, (byte) 2, (byte) 1, (byte) 4, this::LSR);
		instructions[75] = new Instruction("ALR", (byte) 0, (byte) 2, (byte) 0, (byte) 5, this::ALR);
		instructions[76] = new Instruction("JMP", (byte) 0, (byte) 3, (byte) 3, (byte) 1, this::JMP);
		instructions[77] = new Instruction("EOR", (byte) 0, (byte) 4, (byte) 3, (byte) 1, this::EOR);
		instructions[78] = new Instruction("LSR", (byte) 0, (byte) 6, (byte) 3, (byte) 1, this::LSR);
		instructions[79] = new Instruction("SRE", (byte) 0, (byte) 6, (byte) 0, (byte) 1, this::SRE);
		instructions[80] = new Instruction("BVC", (byte) 1, (byte) 2, (byte) 2, (byte) 10, this::BVC);
		instructions[81] = new Instruction("EOR", (byte) 1, (byte) 5, (byte) 2, (byte) 9, this::EOR);
		instructions[82] = new Instruction("KIL", (byte) 0, (byte) 2, (byte) 0, (byte) 6, this::KIL);
		instructions[83] = new Instruction("SRE", (byte) 0, (byte) 8, (byte) 0, (byte) 9, this::SRE);
		instructions[84] = new Instruction("NOP", (byte) 0, (byte) 4, (byte) 2, (byte) 12, this::NOP);
		instructions[85] = new Instruction("EOR", (byte) 0, (byte) 4, (byte) 2, (byte) 12, this::EOR);
		instructions[86] = new Instruction("LSR", (byte) 0, (byte) 6, (byte) 2, (byte) 12, this::LSR);
		instructions[87] = new Instruction("SRE", (byte) 0, (byte) 6, (byte) 0, (byte) 12, this::SRE);
		instructions[88] = new Instruction("CLI", (byte) 0, (byte) 2, (byte) 1, (byte) 6, this::CLI);
		instructions[89] = new Instruction("EOR", (byte) 1, (byte) 4, (byte) 3, (byte) 3, this::EOR);
		instructions[90] = new Instruction("NOP", (byte) 0, (byte) 2, (byte) 1, (byte) 6, this::NOP);
		instructions[91] = new Instruction("SRE", (byte) 0, (byte) 7, (byte) 0, (byte) 3, this::SRE);
		instructions[92] = new Instruction("NOP", (byte) 1, (byte) 4, (byte) 3, (byte) 2, this::NOP);
		instructions[93] = new Instruction("EOR", (byte) 1, (byte) 4, (byte) 3, (byte) 2, this::EOR);
		instructions[94] = new Instruction("LSR", (byte) 0, (byte) 7, (byte) 3, (byte) 2, this::LSR);
		instructions[95] = new Instruction("SRE", (byte) 0, (byte) 7, (byte) 0, (byte) 2, this::SRE);
		instructions[96] = new Instruction("RTS", (byte) 0, (byte) 6, (byte) 1, (byte) 6, this::RTS);
		instructions[97] = new Instruction("ADC", (byte) 0, (byte) 6, (byte) 2, (byte) 7, this::ADC);
		instructions[98] = new Instruction("KIL", (byte) 0, (byte) 2, (byte) 0, (byte) 6, this::KIL);
		instructions[99] = new Instruction("RRA", (byte) 0, (byte) 8, (byte) 0, (byte) 7, this::RRA);
		instructions[100] = new Instruction("NOP", (byte) 0, (byte) 3, (byte) 2, (byte) 11, this::NOP);
		instructions[101] = new Instruction("ADC", (byte) 0, (byte) 3, (byte) 2, (byte) 11, this::ADC);
		instructions[102] = new Instruction("ROR", (byte) 0, (byte) 5, (byte) 2, (byte) 11, this::ROR);
		instructions[103] = new Instruction("RRA", (byte) 0, (byte) 5, (byte) 0, (byte) 11, this::RRA);
		instructions[104] = new Instruction("PLA", (byte) 0, (byte) 4, (byte) 1, (byte) 6, this::PLA);
		instructions[105] = new Instruction("ADC", (byte) 0, (byte) 2, (byte) 2, (byte) 5, this::ADC);
		instructions[106] = new Instruction("ROR", (byte) 0, (byte) 2, (byte) 1, (byte) 4, this::ROR);
		instructions[107] = new Instruction("ARR", (byte) 0, (byte) 2, (byte) 0, (byte) 5, this::ARR);
		instructions[108] = new Instruction("JMP", (byte) 0, (byte) 5, (byte) 3, (byte) 8, this::JMP);
		instructions[109] = new Instruction("ADC", (byte) 0, (byte) 4, (byte) 3, (byte) 1, this::ADC);
		instructions[110] = new Instruction("ROR", (byte) 0, (byte) 6, (byte) 3, (byte) 1, this::ROR);
		instructions[111] = new Instruction("RRA", (byte) 0, (byte) 6, (byte) 0, (byte) 1, this::RRA);
		instructions[112] = new Instruction("BVS", (byte) 1, (byte) 2, (byte) 2, (byte) 10, this::BVS);
		instructions[113] = new Instruction("ADC", (byte) 1, (byte) 5, (byte) 2, (byte) 9, this::ADC);
		instructions[114] = new Instruction("KIL", (byte) 0, (byte) 2, (byte) 0, (byte) 6, this::KIL);
		instructions[115] = new Instruction("RRA", (byte) 0, (byte) 8, (byte) 0, (byte) 9, this::RRA);
		instructions[116] = new Instruction("NOP", (byte) 0, (byte) 4, (byte) 2, (byte) 12, this::NOP);
		instructions[117] = new Instruction("ADC", (byte) 0, (byte) 4, (byte) 2, (byte) 12, this::ADC);
		instructions[118] = new Instruction("ROR", (byte) 0, (byte) 6, (byte) 2, (byte) 12, this::ROR);
		instructions[119] = new Instruction("RRA", (byte) 0, (byte) 6, (byte) 0, (byte) 12, this::RRA);
		instructions[120] = new Instruction("SEI", (byte) 0, (byte) 2, (byte) 1, (byte) 6, this::SEI);
		instructions[121] = new Instruction("ADC", (byte) 1, (byte) 4, (byte) 3, (byte) 3, this::ADC);
		instructions[122] = new Instruction("NOP", (byte) 0, (byte) 2, (byte) 1, (byte) 6, this::NOP);
		instructions[123] = new Instruction("RRA", (byte) 0, (byte) 7, (byte) 0, (byte) 3, this::RRA);
		instructions[124] = new Instruction("NOP", (byte) 1, (byte) 4, (byte) 3, (byte) 2, this::NOP);
		instructions[125] = new Instruction("ADC", (byte) 1, (byte) 4, (byte) 3, (byte) 2, this::ADC);
		instructions[126] = new Instruction("ROR", (byte) 0, (byte) 7, (byte) 3, (byte) 2, this::ROR);
		instructions[127] = new Instruction("RRA", (byte) 0, (byte) 7, (byte) 0, (byte) 2, this::RRA);
		instructions[128] = new Instruction("NOP", (byte) 0, (byte) 2, (byte) 2, (byte) 5, this::NOP);
		instructions[129] = new Instruction("STA", (byte) 0, (byte) 6, (byte) 2, (byte) 7, this::STA);
		instructions[130] = new Instruction("NOP", (byte) 0, (byte) 2, (byte) 0, (byte) 5, this::NOP);
		instructions[131] = new Instruction("SAX", (byte) 0, (byte) 6, (byte) 0, (byte) 7, this::SAX);
		instructions[132] = new Instruction("STY", (byte) 0, (byte) 3, (byte) 2, (byte) 11, this::STY);
		instructions[133] = new Instruction("STA", (byte) 0, (byte) 3, (byte) 2, (byte) 11, this::STA);
		instructions[134] = new Instruction("STX", (byte) 0, (byte) 3, (byte) 2, (byte) 11, this::STX);
		instructions[135] = new Instruction("SAX", (byte) 0, (byte) 3, (byte) 0, (byte) 11, this::SAX);
		instructions[136] = new Instruction("DEY", (byte) 0, (byte) 2, (byte) 1, (byte) 6, this::DEY);
		instructions[137] = new Instruction("NOP", (byte) 0, (byte) 2, (byte) 0, (byte) 5, this::NOP);
		instructions[138] = new Instruction("TXA", (byte) 0, (byte) 2, (byte) 1, (byte) 6, this::TXA);
		instructions[139] = new Instruction("XAA", (byte) 0, (byte) 2, (byte) 0, (byte) 5, this::XAA);
		instructions[140] = new Instruction("STY", (byte) 0, (byte) 4, (byte) 3, (byte) 1, this::STY);
		instructions[141] = new Instruction("STA", (byte) 0, (byte) 4, (byte) 3, (byte) 1, this::STA);
		instructions[142] = new Instruction("STX", (byte) 0, (byte) 4, (byte) 3, (byte) 1, this::STX);
		instructions[143] = new Instruction("SAX", (byte) 0, (byte) 4, (byte) 0, (byte) 1, this::SAX);
		instructions[144] = new Instruction("BCC", (byte) 1, (byte) 2, (byte) 2, (byte) 10, this::BCC);
		instructions[145] = new Instruction("STA", (byte) 0, (byte) 6, (byte) 2, (byte) 9, this::STA);
		instructions[146] = new Instruction("KIL", (byte) 0, (byte) 2, (byte) 0, (byte) 6, this::KIL);
		instructions[147] = new Instruction("AHX", (byte) 0, (byte) 6, (byte) 0, (byte) 9, this::AHX);
		instructions[148] = new Instruction("STY", (byte) 0, (byte) 4, (byte) 2, (byte) 12, this::STY);
		instructions[149] = new Instruction("STA", (byte) 0, (byte) 4, (byte) 2, (byte) 12, this::STA);
		instructions[150] = new Instruction("STX", (byte) 0, (byte) 4, (byte) 2, (byte) 13, this::STX);
		instructions[151] = new Instruction("SAX", (byte) 0, (byte) 4, (byte) 0, (byte) 13, this::SAX);
		instructions[152] = new Instruction("TYA", (byte) 0, (byte) 2, (byte) 1, (byte) 6, this::TYA);
		instructions[153] = new Instruction("STA", (byte) 0, (byte) 5, (byte) 3, (byte) 3, this::STA);
		instructions[154] = new Instruction("TXS", (byte) 0, (byte) 2, (byte) 1, (byte) 6, this::TXS);
		instructions[155] = new Instruction("TAS", (byte) 0, (byte) 5, (byte) 0, (byte) 3, this::TAS);
		instructions[156] = new Instruction("SHY", (byte) 0, (byte) 5, (byte) 0, (byte) 2, this::SHY);
		instructions[157] = new Instruction("STA", (byte) 0, (byte) 5, (byte) 3, (byte) 2, this::STA);
		instructions[158] = new Instruction("SHX", (byte) 0, (byte) 5, (byte) 0, (byte) 3, this::SHX);
		instructions[159] = new Instruction("AHX", (byte) 0, (byte) 5, (byte) 0, (byte) 3, this::AHX);
		instructions[160] = new Instruction("LDY", (byte) 0, (byte) 2, (byte) 2, (byte) 5, this::LDY);
		instructions[161] = new Instruction("LDA", (byte) 0, (byte) 6, (byte) 2, (byte) 7, this::LDA);
		instructions[162] = new Instruction("LDX", (byte) 0, (byte) 2, (byte) 2, (byte) 5, this::LDX);
		instructions[163] = new Instruction("LAX", (byte) 0, (byte) 6, (byte) 0, (byte) 7, this::LAX);
		instructions[164] = new Instruction("LDY", (byte) 0, (byte) 3, (byte) 2, (byte) 11, this::LDY);
		instructions[165] = new Instruction("LDA", (byte) 0, (byte) 3, (byte) 2, (byte) 11, this::LDA);
		instructions[166] = new Instruction("LDX", (byte) 0, (byte) 3, (byte) 2, (byte) 11, this::LDX);
		instructions[167] = new Instruction("LAX", (byte) 0, (byte) 3, (byte) 0, (byte) 11, this::LAX);
		instructions[168] = new Instruction("TAY", (byte) 0, (byte) 2, (byte) 1, (byte) 6, this::TAY);
		instructions[169] = new Instruction("LDA", (byte) 0, (byte) 2, (byte) 2, (byte) 5, this::LDA);
		instructions[170] = new Instruction("TAX", (byte) 0, (byte) 2, (byte) 1, (byte) 6, this::TAX);
		instructions[171] = new Instruction("LAX", (byte) 0, (byte) 2, (byte) 0, (byte) 5, this::LAX);
		instructions[172] = new Instruction("LDY", (byte) 0, (byte) 4, (byte) 3, (byte) 1, this::LDY);
		instructions[173] = new Instruction("LDA", (byte) 0, (byte) 4, (byte) 3, (byte) 1, this::LDA);
		instructions[174] = new Instruction("LDX", (byte) 0, (byte) 4, (byte) 3, (byte) 1, this::LDX);
		instructions[175] = new Instruction("LAX", (byte) 0, (byte) 4, (byte) 0, (byte) 1, this::LAX);
		instructions[176] = new Instruction("BCS", (byte) 1, (byte) 2, (byte) 2, (byte) 10, this::BCS);
		instructions[177] = new Instruction("LDA", (byte) 1, (byte) 5, (byte) 2, (byte) 9, this::LDA);
		instructions[178] = new Instruction("KIL", (byte) 0, (byte) 2, (byte) 0, (byte) 6, this::KIL);
		instructions[179] = new Instruction("LAX", (byte) 1, (byte) 5, (byte) 0, (byte) 9, this::LAX);
		instructions[180] = new Instruction("LDY", (byte) 0, (byte) 4, (byte) 2, (byte) 12, this::LDY);
		instructions[181] = new Instruction("LDA", (byte) 0, (byte) 4, (byte) 2, (byte) 12, this::LDA);
		instructions[182] = new Instruction("LDX", (byte) 0, (byte) 4, (byte) 2, (byte) 13, this::LDX);
		instructions[183] = new Instruction("LAX", (byte) 0, (byte) 4, (byte) 0, (byte) 13, this::LAX);
		instructions[184] = new Instruction("CLV", (byte) 0, (byte) 2, (byte) 1, (byte) 6, this::CLV);
		instructions[185] = new Instruction("LDA", (byte) 1, (byte) 4, (byte) 3, (byte) 3, this::LDA);
		instructions[186] = new Instruction("TSX", (byte) 0, (byte) 2, (byte) 1, (byte) 6, this::TSX);
		instructions[187] = new Instruction("LAS", (byte) 1, (byte) 4, (byte) 0, (byte) 3, this::LAS);
		instructions[188] = new Instruction("LDY", (byte) 1, (byte) 4, (byte) 3, (byte) 2, this::LDY);
		instructions[189] = new Instruction("LDA", (byte) 1, (byte) 4, (byte) 3, (byte) 2, this::LDA);
		instructions[190] = new Instruction("LDX", (byte) 1, (byte) 4, (byte) 3, (byte) 3, this::LDX);
		instructions[191] = new Instruction("LAX", (byte) 1, (byte) 4, (byte) 0, (byte) 3, this::LAX);
		instructions[192] = new Instruction("CPY", (byte) 0, (byte) 2, (byte) 2, (byte) 5, this::CPY);
		instructions[193] = new Instruction("CMP", (byte) 0, (byte) 6, (byte) 2, (byte) 7, this::CMP);
		instructions[194] = new Instruction("NOP", (byte) 0, (byte) 2, (byte) 0, (byte) 5, this::NOP);
		instructions[195] = new Instruction("DCP", (byte) 0, (byte) 8, (byte) 0, (byte) 7, this::DCP);
		instructions[196] = new Instruction("CPY", (byte) 0, (byte) 3, (byte) 2, (byte) 11, this::CPY);
		instructions[197] = new Instruction("CMP", (byte) 0, (byte) 3, (byte) 2, (byte) 11, this::CMP);
		instructions[198] = new Instruction("DEC", (byte) 0, (byte) 5, (byte) 2, (byte) 11, this::DEC);
		instructions[199] = new Instruction("DCP", (byte) 0, (byte) 5, (byte) 0, (byte) 11, this::DCP);
		instructions[200] = new Instruction("INY", (byte) 0, (byte) 2, (byte) 1, (byte) 6, this::INY);
		instructions[201] = new Instruction("CMP", (byte) 0, (byte) 2, (byte) 2, (byte) 5, this::CMP);
		instructions[202] = new Instruction("DEX", (byte) 0, (byte) 2, (byte) 1, (byte) 6, this::DEX);
		instructions[203] = new Instruction("AXS", (byte) 0, (byte) 2, (byte) 0, (byte) 5, this::AXS);
		instructions[204] = new Instruction("CPY", (byte) 0, (byte) 4, (byte) 3, (byte) 1, this::CPY);
		instructions[205] = new Instruction("CMP", (byte) 0, (byte) 4, (byte) 3, (byte) 1, this::CMP);
		instructions[206] = new Instruction("DEC", (byte) 0, (byte) 6, (byte) 3, (byte) 1, this::DEC);
		instructions[207] = new Instruction("DCP", (byte) 0, (byte) 6, (byte) 0, (byte) 1, this::DCP);
		instructions[208] = new Instruction("BNE", (byte) 1, (byte) 2, (byte) 2, (byte) 10, this::BNE);
		instructions[209] = new Instruction("CMP", (byte) 1, (byte) 5, (byte) 2, (byte) 9, this::CMP);
		instructions[210] = new Instruction("KIL", (byte) 0, (byte) 2, (byte) 0, (byte) 6, this::KIL);
		instructions[211] = new Instruction("DCP", (byte) 0, (byte) 8, (byte) 0, (byte) 9, this::DCP);
		instructions[212] = new Instruction("NOP", (byte) 0, (byte) 4, (byte) 2, (byte) 12, this::NOP);
		instructions[213] = new Instruction("CMP", (byte) 0, (byte) 4, (byte) 2, (byte) 12, this::CMP);
		instructions[214] = new Instruction("DEC", (byte) 0, (byte) 6, (byte) 2, (byte) 12, this::DEC);
		instructions[215] = new Instruction("DCP", (byte) 0, (byte) 6, (byte) 0, (byte) 12, this::DCP);
		instructions[216] = new Instruction("CLD", (byte) 0, (byte) 2, (byte) 1, (byte) 6, this::CLD);
		instructions[217] = new Instruction("CMP", (byte) 1, (byte) 4, (byte) 3, (byte) 3, this::CMP);
		instructions[218] = new Instruction("NOP", (byte) 0, (byte) 2, (byte) 1, (byte) 6, this::NOP);
		instructions[219] = new Instruction("DCP", (byte) 0, (byte) 7, (byte) 0, (byte) 3, this::DCP);
		instructions[220] = new Instruction("NOP", (byte) 1, (byte) 4, (byte) 3, (byte) 2, this::NOP);
		instructions[221] = new Instruction("CMP", (byte) 1, (byte) 4, (byte) 3, (byte) 2, this::CMP);
		instructions[222] = new Instruction("DEC", (byte) 0, (byte) 7, (byte) 3, (byte) 2, this::DEC);
		instructions[223] = new Instruction("DCP", (byte) 0, (byte) 7, (byte) 0, (byte) 2, this::DCP);
		instructions[224] = new Instruction("CPX", (byte) 0, (byte) 2, (byte) 2, (byte) 5, this::CPX);
		instructions[225] = new Instruction("SBC", (byte) 0, (byte) 6, (byte) 2, (byte) 7, this::SBC);
		instructions[226] = new Instruction("NOP", (byte) 0, (byte) 2, (byte) 0, (byte) 5, this::NOP);
		instructions[227] = new Instruction("ISC", (byte) 0, (byte) 8, (byte) 0, (byte) 7, this::ISC);
		instructions[228] = new Instruction("CPX", (byte) 0, (byte) 3, (byte) 2, (byte) 11, this::CPX);
		instructions[229] = new Instruction("SBC", (byte) 0, (byte) 3, (byte) 2, (byte) 11, this::SBC);
		instructions[230] = new Instruction("INC", (byte) 0, (byte) 5, (byte) 2, (byte) 11, this::INC);
		instructions[231] = new Instruction("ISC", (byte) 0, (byte) 5, (byte) 0, (byte) 11, this::ISC);
		instructions[232] = new Instruction("INX", (byte) 0, (byte) 2, (byte) 1, (byte) 6, this::INX);
		instructions[233] = new Instruction("SBC", (byte) 0, (byte) 2, (byte) 2, (byte) 5, this::SBC);
		instructions[234] = new Instruction("NOP", (byte) 0, (byte) 2, (byte) 1, (byte) 6, this::NOP);
		instructions[235] = new Instruction("SBC", (byte) 0, (byte) 2, (byte) 0, (byte) 5, this::SBC);
		instructions[236] = new Instruction("CPX", (byte) 0, (byte) 4, (byte) 3, (byte) 1, this::CPX);
		instructions[237] = new Instruction("SBC", (byte) 0, (byte) 4, (byte) 3, (byte) 1, this::SBC);
		instructions[238] = new Instruction("INC", (byte) 0, (byte) 6, (byte) 3, (byte) 1, this::INC);
		instructions[239] = new Instruction("ISC", (byte) 0, (byte) 6, (byte) 0, (byte) 1, this::ISC);
		instructions[240] = new Instruction("BEQ", (byte) 1, (byte) 2, (byte) 2, (byte) 10, this::BEQ);
		instructions[241] = new Instruction("SBC", (byte) 1, (byte) 5, (byte) 2, (byte) 9, this::SBC);
		instructions[242] = new Instruction("KIL", (byte) 0, (byte) 2, (byte) 0, (byte) 6, this::KIL);
		instructions[243] = new Instruction("ISC", (byte) 0, (byte) 8, (byte) 0, (byte) 9, this::ISC);
		instructions[244] = new Instruction("NOP", (byte) 0, (byte) 4, (byte) 2, (byte) 12, this::NOP);
		instructions[245] = new Instruction("SBC", (byte) 0, (byte) 4, (byte) 2, (byte) 12, this::SBC);
		instructions[246] = new Instruction("INC", (byte) 0, (byte) 6, (byte) 2, (byte) 12, this::INC);
		instructions[247] = new Instruction("ISC", (byte) 0, (byte) 6, (byte) 0, (byte) 12, this::ISC);
		instructions[248] = new Instruction("SED", (byte) 0, (byte) 2, (byte) 1, (byte) 6, this::SED);
		instructions[249] = new Instruction("SBC", (byte) 1, (byte) 4, (byte) 3, (byte) 3, this::SBC);
		instructions[250] = new Instruction("NOP", (byte) 0, (byte) 2, (byte) 1, (byte) 6, this::NOP);
		instructions[251] = new Instruction("ISC", (byte) 0, (byte) 7, (byte) 0, (byte) 3, this::ISC);
		instructions[252] = new Instruction("NOP", (byte) 1, (byte) 4, (byte) 3, (byte) 2, this::NOP);
		instructions[253] = new Instruction("SBC", (byte) 1, (byte) 4, (byte) 3, (byte) 2, this::SBC);
		instructions[254] = new Instruction("INC", (byte) 0, (byte) 7, (byte) 3, (byte) 2, this::INC);
		instructions[255] = new Instruction("ISC", (byte) 0, (byte) 7, (byte) 0, (byte) 2, this::ISC);

	}

	int step() {
		// Step 1, we may be waiting some cycles for some reason
		// cpu stalled

		// Step 2, we should handle interupts
		// something may have set an interupt, we should handle

		// Step 3, decode opcode by looking at the current program counter
		byte opcode = ram.read(PC);
		Instruction inst = instructions[opcode];
		mode = inst.addressingMode;

		PC += inst.size;
		/*
		cycles += inst.instructionCycles;
		if pageCrossed {
			cycles += inst.instructionPageCycles;
		}
		*/

		instructions[opcode].inst.execute();
		return 0;
	}

	interface Inst {
		void execute();
	}

	static class Instruction {
		Instruction(String name, byte pageCycles, byte cycles, byte size, byte addressingMode, Inst inst) {
			this.name = name;
			this.pageCycles = pageCycles;
			this.cycles = cycles;
			this.size = size;
			this.addressingMode = addressingMode;
			this.inst = inst;
		}

		// the opcode name
		String name;

		// the number of cycles this instruction takes when a page is crossed
		byte pageCycles;

		// the number of cycles this instruction takes
		byte cycles;

		// the size of the instruction in bytes
		byte size;

		// the addressing mode for each instruction
		byte addressingMode;

		// the function to execute
		Inst inst;


	}

	// reset to power up state
	void reset() {
	}

	// print current cpu stat
	void printInstruction() {

	}

	// are 2 addresses on the same page
	// TODO what is a page?
	boolean pagesDiffer(short a, short b) {
		return (a & 0xFF00) != (b & 0xFF00);
	}

	// adds extra cpu cycle for branching, and another if pages differ
	void addBranchCycles() {
		cycles++;
		if (pagesDiffer(PC, address)) {
			cycles++;
		}
	}

	void compare(byte a, byte b) {
		setZN((byte) (a - b));
		C = (byte) (a >= b ? 1 : 0);
	}

	// reads 2 bytes at address
	short read16(short a) {
		short low = ram.read(address);
		short hi = ram.read((short) (address + 1));
		return (short) ((hi << 8) | low);
	}

	// 6502 bug that caused the low byte to wrap without incrementing high byte
	short read16bug(short address) {
		short low = ram.read(address);
		short hi = ram.read((short) ((address & 0xFF00) | ((byte) address + 1)));
		return (short) ((hi << 8) | low);

	}

	// push a byte onto stack
	void push(byte b) {
		ram.write((short) (0x100 | this.SP), b);
		this.SP--;
	}

	// pops a byte from the stack
	byte pop() {
		this.SP++;
		return ram.read((short) (0x100 | this.SP));
	}

	// push 2 bytes on stack
	void push16(short a) {
		push((byte) ((a >> 8) & 0xFF));
		push((byte) (a & 0xFF));
	}

	// pop 2 bytes from stack
	short pop16() {
		short low = pop();
		short hi = pop();
		return (short) ((hi << 8) | low);
	}

	// returns flags of processor status register
	byte flags() {
		byte flags = 0;
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
	void setFlags(byte b) {


	}

	// set the zero flag
	void setZ(byte b) {

	}

	// set the negative flag
	void setN(byte b) {

	}

	// set zero and negative flag
	void setZN(byte b) {

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
}