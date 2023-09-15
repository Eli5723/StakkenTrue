#include <Audio/Audio.hpp>

#include <spdlog/spdlog.h>

namespace Audio {
    SDL_AudioSpec spec;

    bool Init() {
        // Initialize SDL_mixer
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            spdlog::error("SDL_mixer could not initialize! SDL_mixer Error: {}\n", Mix_GetError());
            return false;
        }

        spec.freq = MIX_DEFAULT_FREQUENCY;
        spec.format = MIX_DEFAULT_FORMAT;
        spec.channels = MIX_DEFAULT_CHANNELS;

        Mix_SetMusicCMD(SDL_getenv("MUSIC_CMD"));
        Mix_QuerySpec(&spec.freq, &spec.format, (int*)&spec.channels);

        spdlog::info("Audio initialized.");
        return true;
    }

    void Shutdown() {
        Mix_Quit();
    }

    SDL_AudioSpec& GetSpec() {
        return spec;
    }
}