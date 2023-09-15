#include <Audio/Sounds.hpp>

#include <spdlog/spdlog.h>
#include <Audio/Audio.hpp>

#include <defines.h>
#include <filesystem>
#include <string>
#include <utility>

namespace Audio::Sound {
    std::pair<std::string, SoundID> sound_map[] = {
        { "countdown", SoundID::Countdown },
        { "hard_drop", SoundID::HardDrop },
        { "sonic_drop", SoundID::SonicDrop },
        { "lock", SoundID::Lock },
        { "clear", SoundID::Clear },
        { "win", SoundID::Win },
        { "game_over", SoundID::GameOver },
        { "combo_5", SoundID::Combo5 },
        { "combo_7", SoundID::Combo7 },
        { "combo_10", SoundID::Combo10 },
        { "combo_12", SoundID::Combo12 },
        { "combo_14", SoundID::Combo14 }
    };

    const std::string extensions[]{
        ".wav", 
        ".ogg",
        ".mp3"
    };

    struct Sound {
        Mix_Chunk* chunk;
    };

    Sound loadedSounds[(int)SoundID::Count]{0};

    void Init() {
        spdlog::info("Initializing sounds...");
        LoadSoundDirectory("Resources/Sounds/Default");
    }

    void UnloadActiveSounds() {
        for (auto& sound : loadedSounds) {
            if (sound.chunk) {
                Mix_FreeChunk(sound.chunk);
                sound.chunk = NULL;
            }
        }
    }

    void Play(SoundID sound) {
        Mix_PlayChannel(-1, loadedSounds[(int)sound].chunk, 0);
        SetVolume(1);
    }

    void SetVolume(float volume) {
        Mix_Volume(-1, volume * MIX_MAX_VOLUME);
    }

    void LoadSoundDirectory(const char* directory) {
        if (!std::filesystem::exists(directory)) {
            spdlog::error("Failed to load sounds from {}! Directory does not exist!", directory);
            return;
        }

        spdlog::info("Loading sounds from {}...", directory);
        for (const auto & entry : std::filesystem::directory_iterator(directory)) {
            for (auto& extension : extensions) {
                if (entry.path().extension() == extension) {
                    for (auto& sound : sound_map) {
                        if (entry.path().stem() == sound.first) {
                            spdlog::info("Loading sound {}...", entry.path().string());
                            loadedSounds[(int)sound.second].chunk = Mix_LoadWAV(entry.path().string().c_str());
                            if (loadedSounds[(int)sound.second].chunk == NULL) {
                                spdlog::error("Failed to load sound {}! SDL_mixer Error: {}\n", entry.path().string(), Mix_GetError());
                            }
                        }
                    }
                }
            }
        }
    }
}