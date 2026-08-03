#ifndef NES_EMU_CONTROLLER_H
#define NES_EMU_CONTROLLER_H
#include <cstdint>

enum class Buttons {
    ButtonA,
    ButtonB,
    ButtonSelect,
    ButtonStart,
    ButtonUp,
    ButtonDown,
    ButtonLeft,
    ButtonRight
};

class Controller {
    bool buttons[8]{};
    uint8_t index{};
    uint8_t strobe{};

public:
    void setButtons(bool buttons[8]);

    uint8_t read();

    void write(uint8_t value);
};

#endif //NES_EMU_CONTROLLER_H
