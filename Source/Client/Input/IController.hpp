#pragma once

#include <Game/GameEvent.hpp>

namespace Client {
    namespace Input {
        class IController {
            public:
                virtual Core::Game::EventStream GetInputStream() = 0;
        };
    }
}