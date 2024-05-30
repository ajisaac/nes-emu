package co.aisaac;

public class Utils {
	/**
	 * Gives the unsigned version of this byte, as an int.
	 *
	 * @param b byte to convert
	 * @return unsigned version of this byte
	 */
	public static int ub(byte b) {
		return Byte.toUnsignedInt(b);
	}

	/**
	 * Gives the unsigned version of this byte, as an int.
	 *
	 * @param i int to convert
	 * @return unsigned version of this byte
	 */
	public static int ub(int i) {
		return i & 0xFF;
	}
}
