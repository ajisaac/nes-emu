#ifndef NES_EMU_CONTROLLER_H
#define NES_EMU_CONTROLLER_H
#include <array>
#include <cstdint>

// unscoped so the buttons can index a button array, like Go's constants
enum Buttons {
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
    std::array<bool, 8> buttons{};
    uint8_t index{};
    uint8_t strobe{};

public:
    void setButtons(const std::array<bool, 8> &buttons);

    uint8_t read();

    void write(uint8_t value);
};

#endif //NES_EMU_CONTROLLER_H
