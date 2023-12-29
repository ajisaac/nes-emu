package co.aisaac;

public class Console {
	Ram ram;
	Cpu cpu;
	Ppu ppu;
	Apu apu;

	public Console() {
		/// create a new console
		Ram ram = new Ram();
		Cpu cpu = new Cpu(ram);
		Ppu ppu = new Ppu(ram);
		Apu apu = new Apu(ram);

		/// set up controllers
		/// load the cartridge
	}

	public void run() {
		// start the console
		while (true) {
			step();
		}
	}

	public void loadCartridge(String rom) {
		/// the .nes format should return a cartridge
		/// this format is documented
	}

	public void reset() {
		// reset the cpu
	}

	public void step() {
		System.out.println("step");
	}
}
