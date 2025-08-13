
#include "Console.h"
#include <SDL3/SDL.h>

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;


int main() {
    const std::string path{"/Users/aaron/Code/personal/nes-emu/data/Nintendo/Castlevania.nes"};

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        throw std::runtime_error{SDL_GetError()};
    }
    if (SDL_CreateWindowAndRenderer("NES", 800, 600, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        throw std::runtime_error{SDL_GetError()};
    }
    SDL_SetWindowTitle(window, path.c_str());
    SDL_Event event;
    uint64_t timestamp = SDL_GetTicks();

    Console console{path};

    while (true) {
        SDL_PollEvent(&event);
        if (event.type == SDL_EVENT_QUIT) {
            break;
        }

        const uint64_t now = SDL_GetTicks();
        uint64_t delta = now - timestamp;
        timestamp = now;

        // run game logic, update buffer in console

        if (delta > 1) {
            delta = 0;
        }

        // grab window, grab console, to do controller update stuff
        // do some sort of joystickReset thing and possibly show menu

        // grab controller input from SDL or whereever
        // read keys, joysticks, etc
        // update the console state or something

        console.stepSeconds(delta);
        // grab the buffer from the console, draw it with sdl
        // possibly do other things

        // todo want to be doing some sort of buffer swap or whatever
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
