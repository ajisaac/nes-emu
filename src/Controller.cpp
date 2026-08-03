#include "Controller.h"

void Controller::setButtons(bool buttons[8]) {
    for (int i = 0; i < 8; i++) {
        this->buttons[i] = buttons[i];
    }
}

uint8_t Controller::read() {
    uint8_t value = 0;
    if (this->index < 8 && this->buttons[this->index]) {
        value = 1;
    }
    this->index++;
    if ((this->strobe & 1) == 1) {
        this->index = 0;
    }
    return value;
}

void Controller::write(uint8_t value) {
    this->strobe = value;
    if ((this->strobe & 1) == 1) {
        this->index = 0;
    }
}
