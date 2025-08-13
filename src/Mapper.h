#ifndef NES_EMU_MAPPER_H
#define NES_EMU_MAPPER_H
#include <cstdint>

class Mapper {
public:
    char read(uint16_t i);
    void write(uint16_t i, uint8_t i1);
    void step();
};

#endif //NES_EMU_MAPPER_H
