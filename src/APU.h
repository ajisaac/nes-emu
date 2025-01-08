#ifndef NES_EMU_APU_H
#define NES_EMU_APU_H

class APU {
public:
    char readRegister(short i) {
        return 0;
    }

    void writeRegister(short i, char i1) {}
    void step();
};


#endif //NES_EMU_APU_H
