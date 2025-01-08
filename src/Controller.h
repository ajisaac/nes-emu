#ifndef NES_EMU_CONTROLLER_H
#define NES_EMU_CONTROLLER_H

class Controller {
private:
    int buttons;
    char index;
    char strobe;

public:
    void setButtons(const int buttons) {
        this->buttons = buttons;
    }

    char read() {
        return 0;
    }

    void write(char i) {}
};

#endif //NES_EMU_CONTROLLER_H
