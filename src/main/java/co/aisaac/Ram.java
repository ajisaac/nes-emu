package co.aisaac;

public class Ram {
	/*


    /* this is all our memory mapping information
	std::byte total_ram[0xFFFF]{std::byte{0}};

	/* first 2kb are internal ram
	std::byte *zero_page = &total_ram[0];
	int zero_page_len = 0x100;
	std::byte *cpu_stack = &total_ram[0x100];
	int cpu_stack_len = 0x100;
	std::byte *cpu_ram = &total_ram[0x200];
	int cpu_ram_len = 0x600;

	/* then 3 x 2kb mirrors of first 2kb
	int mirror_len = 0x800; // 2 kb each
	std::byte *mirror1 = &total_ram[0];
	std::byte *mirror2 = &total_ram[0];
	std::byte *mirror3 = &total_ram[0];

	/* 8 bytes for PPU registers
	std::byte *ppu_reg = &total_ram[0x2000];

	/* 8kb - 8b for mirrors of PPU reg
	std::byte *ppu_reg_mirror = &total_ram[0x2000]; // todo make 8k of mirrors, or don't

	/* 24b for APU, 8b disabled, $4000 to 401F, 32b total
	std::byte *apu = &total_ram[0x4000]; // this goes to 0x4020

	/* now we hit the cartridge, it goes from 0x4020 to 0x8000, 32kb - 32b
	std::byte *cartridge = &total_ram[0x4020];
	 */
}
