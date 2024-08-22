
#include "Emulator.h"

#include <utility>


Emulator::Emulator(std::string rom) {

    Cartridge cartridge = Cartridge{std::move(rom)};
    console = Console{cartridge};
//    controller1 = Controller{};
//    controller2 = Controller{};

//    graphics = Graphics();
//    audio = Audio();
//    graphics.createWindow();

}

bool Emulator::step() {

    // todo improve game loop
    long time = get_time();
    long diff = time - this->timestamp;
    this->timestamp = time;
    std::cout << diff << "\n";

//         handle controller input or something

    console.step(diff);
    graphics.draw(console);

    return true;
}

void Emulator::run() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return;
    }

    if (SDL_CreateWindowAndRenderer(800, 800, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return;
    }

    SDL_Event event;
    bool running = true;
    while (running) {
        running = step();
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) {
            running = false;
        }
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_j) {}
            if (event.key.keysym.sym == SDLK_k) {}
            if (event.key.keysym.sym == SDLK_l) {}
            if (event.key.keysym.sym == SDLK_i) {}
        }

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}

long Emulator::get_time() {
    return std::chrono::high_resolution_clock::now().time_since_epoch().count();
}
