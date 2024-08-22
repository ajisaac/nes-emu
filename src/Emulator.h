#ifndef NES_EMU_EMULATOR_H
#define NES_EMU_EMULATOR_H

#include "INes.h"
#include "Console.h"
#include "Graphics.h"
#include "Audio.h"
#include "Cartridge.h"
#include "Controller.h"
#include <SDL.h>
#include <iostream>
#include <string>

// manager class for the console
// It doesn't know what happens in the console, it can only observe it
// This will manage the game loop, rendering, etc
class Emulator {
public:

    long timestamp = get_time();
    Console console;
    Graphics graphics;
//    Controller controller1;
//    Controller controller2;

//    Audio audio;

    SDL_Window *window;
    SDL_Renderer *renderer;

    explicit Emulator(std::string rom);

    void run();

    bool step();

private:
    static long get_time();
};


#endif //NES_EMU_EMULATOR_H
