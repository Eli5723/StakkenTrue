#include <Audio/Music.hpp>

#include <SDL2/SDL_mixer.h>
#include <spdlog/spdlog.h>

#include <defines.h>

#include <vector>
#include <string>
#include <filesystem>

namespace Audio::Music {
    struct Settings {
        float volume = 1.0f;
        bool mute = false;
        bool paused = false;
    } settings;

    struct MusicInfo {
        const char *tag_title = NULL;
        const char *tag_artist = NULL;
        const char *tag_album = NULL;
        const char *tag_copyright = NULL;
    };

    struct State {
        std::vector<std::string> songs;
        std::string current_song_name;
        u32 current_song = 0;
        MusicInfo info;
        Mix_Music *music = NULL;
        bool audio_open = false;
    } state;

    SDL_AudioSpec spec;

    void SetTrack(u32 track) {
        if (state.music) {
            Mix_FreeMusic(state.music);
            state.music = NULL;
        }

        // Stop the playing song        
        state.current_song = track;
        const char *song = state.songs[track].c_str();
        state.music = Mix_LoadMUS(song);
        if (state.music == NULL) {
            spdlog::error("Failed to load music! SDL_mixer Error: {}\n", Mix_GetError());
            NextSong();
            return;
        } else {
            Mix_FadeInMusic(state.music, false, 2000);
            state.current_song_name = song;
            spdlog::info("Now playing: {}", song);
        }
    }

    void SetRandomTrack() {
        SetTrack(rand() % state.songs.size());
    }

    void ScanMusicDirectory() {
        state.songs.clear();
        for (const auto & entry : std::filesystem::directory_iterator("./Resources/Music")) {
            if (entry.path().extension() == ".mod" || 
                entry.path().extension() == ".it" || 
                entry.path().extension() == ".s3m" || 
                entry.path().extension() == ".xm" || 
                entry.path().extension() == ".wav" || 
                entry.path().extension() == ".ogg" || 
                entry.path().extension() == ".mp3"
            ) {
                state.songs.push_back(entry.path());
            }
        }
        
        // If music is already playing, re-set the track
        if (state.music){
            u32 songIndex = 0;
            for (u32 i = 0; i < state.songs.size(); i++) {
                if (state.songs[i] == state.current_song_name) {
                    songIndex = i;
                    break;
                }
            }
        }
    }

    void Setup() {
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            spdlog::error("SDL_mixer could not initialize! SDL_mixer Error: {}\n", Mix_GetError());
            return;
        }
        state.audio_open = true;

        spec.freq = MIX_DEFAULT_FREQUENCY;
        spec.format = MIX_DEFAULT_FORMAT;
        spec.channels = MIX_DEFAULT_CHANNELS;

        Mix_SetMusicCMD(SDL_getenv("MUSIC_CMD"));
        Mix_QuerySpec(&spec.freq, &spec.format, (int*)&spec.channels);

        ScanMusicDirectory();
        if (state.songs.size() == 0) {
            spdlog::warn("No music files found! Shutting down audio subsystem.");
            Shutdown();
            return;
        }
        
        SetTrack(0);
    }

    void Shutdown() {
        if (Mix_PlayingMusic()) {
            Mix_FadeOutMusic(1500);
        }

        if (state.music) {
            Mix_FreeMusic(state.music);
            state.music = NULL;
        }
        if (state.audio_open) {
            Mix_CloseAudio();
            state.audio_open = false;
        }
    }

    void NextSong() {
        SetTrack((state.current_song + 1) % state.songs.size());
    }

    void PreviousSong(){ 
        SetTrack((state.current_song - 1) % state.songs.size());
    }

    void SetVolume(float volume) {
        settings.volume = volume;
        Mix_VolumeMusic((int)(volume * MIX_MAX_VOLUME));
    }

    float GetVolume() {
        return settings.volume;
    }

    void SetMute(bool mute) {
        settings.mute = mute;
    }

    bool GetMute() {
        return settings.mute;
    }

    void SetPaused(bool paused) {
        settings.paused = paused;
    }

    bool GetPaused() {
        return settings.paused;
    }
}