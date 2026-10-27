#include "Graphics.h"

#include <stdexcept>

#include <SDL3/SDL.h>

Graphics::Graphics(const int scale) {
    if (!SDL_CreateWindowAndRenderer("NES", width * scale, height * scale,
                                     SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY, &window, &renderer)) {
        throw std::runtime_error{std::string{"couldn't create window/renderer: "} + SDL_GetError()};
    }

    // wait for the display between frames instead of drawing as fast as possible
    SDL_SetRenderVSync(renderer, 1);

    // RGBA32 means the bytes are in R, G, B, A order in memory on any machine
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, width, height);
    if (!texture) {
        const std::string error = SDL_GetError();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        throw std::runtime_error{"couldn't create texture: " + error};
    }

    // like Go's createTexture: sharp pixels when scaled up, and alpha ignored
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_NONE);
}

Graphics::~Graphics() {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void Graphics::setTitle(const std::string &title) {
    SDL_SetWindowTitle(window, title.c_str());
}

void Graphics::draw(const uint8_t *pixels) {
    SDL_UpdateTexture(texture, nullptr, pixels, width * 4);

    // fit the frame inside the window and center it, like Go's drawBuffer
    int w = 0;
    int h = 0;
    SDL_GetRenderOutputSize(renderer, &w, &h);
    const float s1 = static_cast<float>(w) / width;
    const float s2 = static_cast<float>(h) / height;
    SDL_FRect dst{};
    if (s1 >= s2) {
        dst.w = width * s2;
        dst.h = static_cast<float>(h);
    } else {
        dst.w = static_cast<float>(w);
        dst.h = height * s1;
    }
    dst.x = (static_cast<float>(w) - dst.w) / 2;
    dst.y = (static_cast<float>(h) - dst.h) / 2;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, nullptr, &dst);
    SDL_RenderPresent(renderer);
}
