//
// Created by Aaron Isaac on 5/20/24.
//

#ifndef NES_EMU_UTILS_H
#define NES_EMU_UTILS_H


class Utils {

};


#endif //NES_EMU_UTILS_H
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
