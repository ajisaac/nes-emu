package co.aisaac;

import java.io.IOException;

public class Main {
	public static void main(String[] args) throws IOException {
		System.out.println("Hello world!");
		String rom = "/Users/Shared/roms/Nintendo/Castlevania.nes";
		Console console = new Console(rom);
		console.run();
	}
}