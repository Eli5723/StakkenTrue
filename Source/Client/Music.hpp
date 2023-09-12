#pragma once

namespace Music {
    void Setup();
    void Shutdown();
    
    void NextSong();
    void PreviousSong();

    void SetVolume(float volume);
    float GetVolume();

    void SetMute(bool mute);
    bool GetMute();

    void SetPaused(bool paused);
    bool GetPaused();
}