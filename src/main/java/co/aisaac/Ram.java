package co.aisaac;

import static co.aisaac.Utils.ub;

public class Ram {
	/*
	We don't need buses, everything deals with ram

	NES supports 16 bit addressing, up to 64kb of ram

	0x0000 to 0x00FF -> 256 bytes of zero-page ram
	*/

	byte[] ram = new byte[2000];

	byte read(short address) {
		return ram[address];
	}

	void write(short address, int value) {
		if (address <= 0x00FF) {
			// 0000 -> 00FF
			// zero page

		} else if (address <= 0x01FF) {
			// 0100 -> 01FF
			// stack
			/*
			The stack is located at memory locations $0100-$01FF. The stack pointer is an 8-bit register
			which serves as an offset from $0100. The stack works top-down, so when a byte is pushed
			on to the stack, the stack pointer is decremented and when a byte is pulled from the stack,
			the stack pointer is incremented. There is no detection of stack overflow and the stack
			pointer will just wrap around from $00 to $FF.
			 */

		} else if (address <= 0x07FF) {
			// 0200 -> 07FF
			// ram
			/*
			$0000-$07FF are mirrored three times at $0800-$1FFF. This means that, for example, any
			data written to $0000 will also be written to $0800, $1000 and $1800
			 */

		} else if (address <= 0x1FFF) {
			// 0800 -> 1FFF
			// mirrors of range 0x0000 to 0x07FF

		} else if (address <= 0x4000) {
			// 2000 -> 2007
			// io registers
			/* The memory mapped
			I/O registers are located at $2000-$401F. Locations $2000-$2007 are mirrored every 8 bytes
			in the region $2008-$3FFF and the remaining registers follow this mirroring.
			 */

			// 2008 -> 3FFF
			// mirrors of io registers

		} else if (address <= 0x401F) {
			// 4000 -> 401F
			// io registers and mirrors

		} else if (address <= 0x5FFF) {
			// 4020 -> 5FFF
			// expansion rom

		} else if (address <= 0x7FFF) {
			// 6000 -> 7FFF
			/*
			SRAM (WRAM) is the Save RAM, the addresses used to access RAM in the cartridges for storing save games.
			 */

		} else if (address <= 0xBFFF) {
			// 8000 -> BFFF
			// PRG-ROM lower bank

		} else if (address <= 0xFFFF) {
			// C000 -> FFFF
			// PRG-ROM upper bank

		} else {
			return;
			// 0xFFFF
			// impossible

		}
		write(address, (byte) value);
	}

	void write(short address, byte value) {
		ram[address] = value;
	}

}
