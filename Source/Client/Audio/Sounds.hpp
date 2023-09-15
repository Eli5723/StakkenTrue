#pragma once

#include <defines.h>

namespace Audio::Sound
{
    enum class SoundID : u32
    {
        Countdown,
        SonicDrop,
        HardDrop,
        Lock,
        Clear,
        Win,
        GameOver,
        Combo5,
        Combo7,
        Combo10,
        Combo12,
        Combo14,
        Count
    };

    // String map of sounds to sound id
    void Init();
    void Play(SoundID sound);

    void SetVolume(float volume);
    float GetVolume();

    void LoadSoundDirectory(const char* directory);
} // namespace Audio