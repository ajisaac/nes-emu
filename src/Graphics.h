#ifndef NES_EMU_GRAPHICS_H
#define NES_EMU_GRAPHICS_H
#include <cstdint>
#include <string>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

// Graphics shows the NES picture in an SDL window. SDL's video subsystem has to
// be initialized before one is created.
class Graphics {
public:
    static constexpr int width = 256;
    static constexpr int height = 240;

    SDL_Window *window{};
    SDL_Renderer *renderer{};
    SDL_Texture *texture{};

    // scale sets the starting window size as a multiple of the NES resolution
    explicit Graphics(int scale);

    Graphics(const Graphics &) = delete;

    Graphics &operator=(const Graphics &) = delete;

    ~Graphics();

    void setTitle(const std::string &title);

    // draw shows one 256x240 frame of RGBA pixels (4 bytes each in R, G, B, A
    // order, rows top to bottom), scaled to fit the window without stretching
    void draw(const uint8_t *pixels);
};

#endif //NES_EMU_GRAPHICS_H
