#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "Console.h"


/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = nullptr;
static SDL_Renderer *renderer = nullptr;
static SDL_Texture *texture = nullptr;

constexpr uint32_t NES_WIDTH = 256 * 4;
constexpr uint32_t NES_HEIGHT = 240 * 4;
constexpr uint32_t WINDOW_WIDTH = NES_WIDTH;
constexpr uint32_t WINDOW_HEIGHT = NES_HEIGHT;

struct pixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

static Console *console = nullptr;
uint64_t timestamp = SDL_GetTicks();

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    SDL_SetAppMetadata("Example Renderer Clear", "1.0", "com.example.renderer-clear");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("NES", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!texture) {
        SDL_Log("Couldn't create texture: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    const std::string path{"/Users/aaron/Code/nes-emu/data/Nintendo/Castlevania.nes"};
    console = new Console{path};

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    uint64_t now_ms = SDL_GetTicks();
    uint64_t delta = now_ms - timestamp;
    timestamp = now_ms;

    // run game logic, update buffer in console

    if (delta > 1) {
        delta = 0;
    }

    // grab window, grab console, to do controller update stuff
    // do some sort of joystickReset thing and possibly show menu

    // grab controller input from SDL or whereever
    // read keys, joysticks, etc
    // update the console state or something

    // console->stepSeconds(delta);
    // grab the buffer from the console, draw it with sdl
    // possibly do other things


    SDL_FRect dst_rect;
    SDL_Surface *surface = nullptr;


    /* we'll have some color move around over a few seconds. */

    if (SDL_LockTextureToSurface(texture, nullptr, &surface)) {
        // we want an array of rectangles and we'll do the fillsurfacerect each frame with whatever is in the buffer.


        pixel pixels[NES_WIDTH * NES_HEIGHT];
        for (uint32_t i = 0; i < NES_WIDTH * NES_HEIGHT; i++) {
            pixels[i] = pixel{static_cast<uint8_t>(i % NES_WIDTH), static_cast<uint8_t>(i / NES_HEIGHT), static_cast<uint8_t>(i / NES_WIDTH)};
        }

        SDL_Rect r;
        r.w = 1;
        r.h = 1;
        for (uint32_t i = 0; i < NES_WIDTH * NES_HEIGHT; i++) {
            r.x = i % NES_WIDTH;
            r.y = i / NES_HEIGHT;
            SDL_FillSurfaceRect(surface, &r, SDL_MapRGB(SDL_GetPixelFormatDetails(surface->format), nullptr, pixels[i].r, pixels[i].g, pixels[i].b));
        }


        SDL_UnlockTexture(texture);  /* upload the changes (and frees the temporary surface)! */
    }
    /* as you can see from this, rendering draws over whatever was drawn before it. */
    SDL_SetRenderDrawColor(renderer, 66, 66, 66, SDL_ALPHA_OPAQUE);  /* grey, full alpha */
    SDL_RenderClear(renderer);  /* start with a blank canvas. */

    /* Just draw the static texture a few times. You can think of it like a
       stamp, there isn't a limit to the number of times you can draw with it. */

    /* Center this one. It'll draw the latest version of the texture we drew while it was locked. */
    dst_rect.x = 0.0f;
    dst_rect.y = 0.0f;
    dst_rect.w = WINDOW_WIDTH;
    dst_rect.h = WINDOW_HEIGHT;
    SDL_RenderTexture(renderer, texture, nullptr, &dst_rect);
    SDL_RenderPresent(renderer);  /* put it all on the screen! */

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    delete console;
}
