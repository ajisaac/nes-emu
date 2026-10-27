#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <array>
#include <exception>
#include <string>

#include "Console.h"
#include "Graphics.h"

constexpr int scale = 4;               // starting window size as a multiple of the NES resolution
constexpr int audioSampleRate = 44100; // SDL converts this to whatever the device uses

static Graphics *graphics = nullptr;
static Console *console = nullptr;
static SDL_AudioStream *audio = nullptr;
static uint64_t timestamp = 0; // nanoseconds, from SDL_GetTicksNS

// readKeys maps the keyboard to NES buttons. A and S are turbo A and B, which
// only press on the frames where turbo is true.
static std::array<bool, 8> readKeys(const bool turbo) {
    const bool *keys = SDL_GetKeyboardState(nullptr);
    std::array<bool, 8> result{};
    result[ButtonA] = keys[SDL_SCANCODE_Z] || (turbo && keys[SDL_SCANCODE_A]);
    result[ButtonB] = keys[SDL_SCANCODE_X] || (turbo && keys[SDL_SCANCODE_S]);
    result[ButtonSelect] = keys[SDL_SCANCODE_RSHIFT];
    result[ButtonStart] = keys[SDL_SCANCODE_RETURN];
    result[ButtonUp] = keys[SDL_SCANCODE_UP];
    result[ButtonDown] = keys[SDL_SCANCODE_DOWN];
    result[ButtonLeft] = keys[SDL_SCANCODE_LEFT];
    result[ButtonRight] = keys[SDL_SCANCODE_RIGHT];
    return result;
}

static void updateControllers() {
    const bool turbo = console->ppu->frame % 6 < 3;
    console->setButtons1(readKeys(turbo));
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    SDL_SetAppMetadata("NES", nullptr, nullptr);

    if (argc != 2) {
        SDL_Log("Usage: %s rom_file", argv[0]);
        return SDL_APP_FAILURE;
    }
    const std::string path{argv[1]};

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // one channel of float samples, like Go's chan float32
    const SDL_AudioSpec spec{.format = SDL_AUDIO_F32, .channels = 1, .freq = audioSampleRate};
    audio = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, nullptr, nullptr);
    if (!audio) {
        SDL_Log("Couldn't open audio: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_ResumeAudioStreamDevice(audio);

    try {
        graphics = new Graphics(scale);
        console = new Console{path};
    } catch (const std::exception &e) {
        SDL_Log("%s", e.what());
        return SDL_APP_FAILURE;
    }

    graphics->setTitle(path);
    console->setAudioChannel([](const float sample) {
        // Go's channel holds a second of samples and drops new ones when full
        if (SDL_GetAudioStreamQueued(audio) < audioSampleRate * static_cast<int>(sizeof(float))) {
            SDL_PutAudioStreamData(audio, &sample, sizeof(sample));
        }
    });
    console->setAudioSampleRate(audioSampleRate);

    timestamp = SDL_GetTicksNS();
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    if (event->type == SDL_EVENT_KEY_DOWN && !event->key.repeat) {
        switch (event->key.scancode) {
            case SDL_SCANCODE_ESCAPE:
                // Go goes back to the game menu here; there's no menu, so quit
                return SDL_APP_SUCCESS;
            case SDL_SCANCODE_R:
                console->reset();
                break;
            default:
                break;
        }
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    const uint64_t now = SDL_GetTicksNS();
    double dt = static_cast<double>(now - timestamp) / SDL_NS_PER_SECOND;
    timestamp = now;
    if (dt > 1) {
        dt = 0;
    }

    updateControllers();
    console->stepSeconds(dt);
    graphics->draw(console->buffer()->pix.data());

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    SDL_DestroyAudioStream(audio);
    delete console;
    delete graphics;
}
