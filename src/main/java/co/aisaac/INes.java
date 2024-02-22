package co.aisaac;

import java.io.ByteArrayInputStream;
import java.io.FileInputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Arrays;
import java.util.HexFormat;

public class INes {

	public static void main(String[] args) throws IOException {
		Cartridge cartridge = loadCartridge("/Users/aaron/Code/nes-emu/data/Nintendo/2-in-1 Super Mario Bros - Duck Hunt.nes");
	}

	// the beginning of every ines file format
	public static final int MAGIC = 0x1a53454e;

	static Cartridge loadCartridge(String nesFile) throws IOException {

		// open file
		Path path = Path.of(nesFile);
		byte[] bytes = Files.readAllBytes(path);

		// read file header
		byte[] header = Arrays.copyOfRange(bytes, 0, 16);

		// verify magic number, first 4 bytes
		int i = header[0] | header[1] << 8 | header[2] << 16 | header[3] << 24;
		if (i != MAGIC) throw new IllegalStateException("Magic number unexpected: 0x" + HexFormat.of().toHexDigits(i));

		// num of PRG-ROM in 16kb chunks
		byte numPrg = header[4];

		// num of CHR-ROM in 8kb chunks, 0 means board uses CHR-RAM
		byte numChr = header[5];

		// header[6] = flags 6
		// flags 7
		// flags 8
		// flags 9
		// flags 10

		// magic number for ines file format
		int magicNumber;


		// control bits
		byte control1;

		// control bits
		byte control2;

		// PRG-RAM size, x 8KB
		byte numRam;

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