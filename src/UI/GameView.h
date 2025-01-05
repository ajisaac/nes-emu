#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include "../Console.h"

// an abstraction over the actual console
class GameView {
public:
    // Emulator * emulator;
    Console* console = nullptr;
    // string title
    // string hash ?
    // texture uint32
    // record boolean
    // frames [] *image frames

    explicit GameView(std::string const& path): console{new Console{path}} {}

    ~GameView() {
        // clean up textures and such
        delete console;
    }

    void load(int snapshot) {
        // load state
        // view.console.loadState(hash, snapshot);
    }

    void save(int snapshot) {
        // cartridge == view.console.cartridge
        // if cartridge.battery
        //  writeSRAM(hash, snapshot, cartridge.sram

        // console.saveStaet(hash, snapshot)
    }

    void enter() {}

    void exit() {}

    void update(const unsigned long long t, unsigned long long dt) {
        if (dt > 1) {
            dt = 0;
        }

        // grab window, grab console, to do controller update stuff
        // do some sort of joystickReset thing and possibly show menu
        updateControllers();

        console->stepSeconds(dt);
        // grab the buffer from the console, draw it with sdl
        // possibly do other things
    }

    void onKey() {}

    void drawBuffer() {}

    void updateControllers() {
        // grab controller input from SDL or whereever
        // read keys, joysticks, etc
        // update the console state or something
    }
};

#endif //GAMEVIEW_H
