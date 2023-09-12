#pragma once

#include <defines.h>

#include <Game/GameEvent.hpp>

namespace Core
{
    namespace Game
    {
        struct EventBuffer {
            GameEvent* events = new GameEvent[32];
            u32 size = 32;
            u32 capacity = 32;

            void push(GameEvent event);
            EventStream getStream();
            void clear();
        };
    }
}