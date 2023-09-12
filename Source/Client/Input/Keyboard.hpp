#pragma once

#include <Input/IController.hpp>

#include <SDL2/SDL.h>

#include <Game/EventBuffer.hpp>

#include <string>

namespace Client
{
    namespace Input
    {
        struct KeyboardProfile {
            SDL_Scancode left = SDL_SCANCODE_A;
            SDL_Scancode right = SDL_SCANCODE_D;
            SDL_Scancode rotateLeft = SDL_SCANCODE_KP_4;
            SDL_Scancode rotateRight = SDL_SCANCODE_KP_6;
            SDL_Scancode flip = SDL_SCANCODE_KP_8;
            SDL_Scancode softDrop = SDL_SCANCODE_KP_5;
            SDL_Scancode sonicDrop = SDL_SCANCODE_LSHIFT;
            SDL_Scancode hardDrop = SDL_SCANCODE_SPACE;
            SDL_Scancode instantRight = SDL_SCANCODE_UNKNOWN;
            SDL_Scancode instantLeft = SDL_SCANCODE_UNKNOWN;

            int repeatPeriod = 0;
            int repeatDelay = 110;
            int softdropPeriod = 50;
            int softdropDelay = 300;

            void ReadKeyboardProfile(const std::string& filename);
            void WriteKeyboardProfile(const std::string& filename);
        };
 
        class Keyboard : public IController
        {
        public:
            Core::Game::EventStream GetInputStream() override;
            void Update(int elapsedMilliseconds);
            void KeyDown(const SDL_KeyboardEvent& event);
            void Clear() { output.clear();}

            // Config
            KeyboardProfile profile;
        private:
            // State
            Core::Game::EventBuffer output;
            int lastKey;
            int timeUntilRepeat;
            int timeUntilSoftdrop;
        };
    }
}