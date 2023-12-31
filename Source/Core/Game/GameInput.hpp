#pragma once

#include <defines.h>

namespace Core
{
    namespace Game
    {
        enum class GameInput : u8
        {
            NONE,

            // Sideways Movement
            MOVE_LEFT,
            MOVE_RIGHT,
            INSTANT_LEFT,
            INSTANT_RIGHT,

            // Rotation
            RCW,
            RCCW,
            FLIP,

            // Dropping
            SOFT_DROP,
            SONIC_DROP,
            HARD_DROP,

            // Misc
            Hold,
            CLIENT_CONTROLS,
            GRAVITY,
            REMOVE_ROW,
            ADD_ROW,
        };

        struct InputStream { 
            GameInput* events;
            u32 size;
        };
    }
}