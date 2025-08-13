#ifndef NES_EMU_CONTROLLER_H
#define NES_EMU_CONTROLLER_H

class Controller {
    int buttons{};
    char index{};
    char strobe{};

public:
    void setButtons(int buttons);

    char read();

    void write(char i);
};

#endif //NES_EMU_CONTROLLER_H
