package co.aisaac;

import java.io.IOException;

/**
 * The main class, represents the actual emulator program
 */
public class Emulator {

	public static void main(String[] args) throws IOException {
		String rom = "/Users/aaron/Code/nes-emu/data/Nintendo/1942.nes";
		Emulator emulator = new Emulator(rom);
		emulator.run();
	}

	long timestamp = System.nanoTime();

	Console console;
	Graphics graphics;
	Cartridge cartridge;
	Controller controller1;
	Controller controller2;
	Audio audio;

	public Emulator(String rom) throws IOException {

		this.cartridge = new INes().loadCartridge(rom);
		this.console = new Console(this.cartridge);
		this.graphics = new Graphics();
		this.audio = new Audio();
		this.graphics.createWindow();

	}

	public void run() {
		// initialize everything, resources, opengl, etc
		// should have a game here we want to play

		// loop continuously
		// run update on view

		boolean running = true;
		while (running) {
			running = step();
		}
	}

	private boolean step() {

		// todo improve game loop
		long time = System.nanoTime();
		long diff = time - this.timestamp;
		this.timestamp = time;

		// handle controller input or something

		console.step(diff);
		graphics.draw(console);

		return true;
	}
}
