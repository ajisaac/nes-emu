#ifndef NES_EMU_FILTER_H
#define NES_EMU_FILTER_H
#include "APU.h"

class FilterChain {
public:
    float Step(float output);
};

#endif //NES_EMU_FILTER_H
