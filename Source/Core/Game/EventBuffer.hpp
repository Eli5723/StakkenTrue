#pragma once

#include <defines.h>

#include <Game/GameInput.hpp>

namespace Core
{
    namespace Game
    {
        struct InputBuffer {
            GameInput* events = new GameInput[32];
            u32 size = 32;
            u32 capacity = 32;

            void push(GameInput event);
            InputStream getStream();
            void clear();
        };
    }
}