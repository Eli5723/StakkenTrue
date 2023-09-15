#pragma once

#include <Audio/Music.hpp>
#include <Audio/Sounds.hpp>

#include <SDL2/SDL_mixer.h>

namespace Audio {
    bool InitializeMixer();
    void Shutdown();

    bool Init();

    SDL_AudioSpec& GetSpec();
}