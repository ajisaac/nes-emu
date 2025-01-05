#ifndef NES_EMU_EMULATOR_H
#define NES_EMU_EMULATOR_H

#include <SDL.h>
#include "GameView.h"
#include "MenuView.h"

// the god class
class Emulator
{
public:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    GameView* gameView = nullptr;
    MenuView* menuView = nullptr;
    // Audio audio
    unsigned long long timestamp = 0;
    std::string path;

    Emulator(std::string path)
    {
        this->path = std::move(path);
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
            throw std::runtime_error{SDL_GetError()};
        }

        if (SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_RESIZABLE, &this->window, &this->renderer))
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
            throw std::runtime_error{SDL_GetError()};
        }

        SDL_SetWindowTitle(window, this->path.c_str());

        this->timestamp = SDL_GetTicks64();
        gameView = new GameView{"", ""};
        menuView = new MenuView{};

        // todo create audio and image resources as needed
    }

    ~Emulator()
    {
        delete gameView;
        gameView = nullptr;
        delete menuView;
        menuView = nullptr;

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void run()
    {
        // main loop
        SDL_Event event;
        while (true)
        {
            SDL_PollEvent(&event);
            if (event.type == SDL_QUIT)
            {
                break;
            }

            const unsigned long long now = SDL_GetTicks64();
            const unsigned long long delta = now - this->timestamp;
            this->timestamp = now;

            // run game logic, update buffer in console
            gameView->update(now, delta);
            std::cout << "time: " << delta << std::endl;

            // todo want to be doing some sort of buffer swap or whatever
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
            SDL_RenderClear(renderer);
            SDL_RenderPresent(renderer);
        }
    }
};


#endif //NES_EMU_EMULATOR_H
