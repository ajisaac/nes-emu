package co.aisaac;

import java.io.ByteArrayInputStream;
import java.io.FileInputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.HexFormat;

public class INes {

	public static void main(String[] args) throws IOException {
		Cartridge cartridge = loadCartridge("/Users/aaron/Code/nes-emu/data/Nintendo/2-in-1 Super Mario Bros - Duck Hunt.nes");
	}

	// the beginning of every ines file format
	public static final int MAGIC = 0x1a53454e;

	// magic number for ines file format
	int magicNumber;

	// number of PRG-ROM banks, 16kb each
	byte numPRG;

	// the number of CHR-ROM banks, 8kb each
	byte numCHR;

	// control bits
	byte control1;

	// control bits
	byte control2;

	// PRG-RAM size, x 8KB
	byte numRam;

	static Cartridge loadCartridge(String nesFile) throws IOException {

		// open file
		Path path = Path.of(nesFile);
		byte[] bytes = Files.readAllBytes(path);

		// read file header
		int i = bytes[0] | bytes[1] << 8 | bytes[2] << 16 | bytes[3] << 24;
		String magicString = HexFormat.of().toHexDigits(i);
		System.out.println("0x" + magicString);
		System.out.println(i == MAGIC);

		// verify magic number

		// mapper type

		// mirroring type

		// battery-backed ram

		// read trainer if present

		// read prg-rom banks

		// read chr-rom banks

		// provide chr-rom/ram

		return new Cartridge(nesFile);
	}
}