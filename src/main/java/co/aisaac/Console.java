package co.aisaac;

public class Console {
	Ram ram;
	Cpu cpu;
	Ppu ppu;
	Apu apu;

	Cartridge cartridge;
	Controller controller1;
	Controller controller2;
	Mapper mapper;

	public Console(String rom) {
		Cartridge cartridge = new Cartridge(rom);

		controller1 = new Controller();
		controller2 = new Controller();

		// ram starts out blank
		Ram ram = new Ram();
		Mapper mapper = new Mapper(ram);
		Cpu cpu = new Cpu(ram);
		Ppu ppu = new Ppu(ram);
		Apu apu = new Apu(ram);
	}

	public void run() {
		// start the console
		while (true) {
			cpu.step();
		}
	}

	public void loadCartridge(String rom) {
		/// the .nes format should return a cartridge
		/// this format is documented
	}

	public void reset() {
		// reset the cpu
		cpu.reset();
	}
}
