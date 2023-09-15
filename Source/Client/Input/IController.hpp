#pragma once

#include <Game/GameInput.hpp>

namespace Client {
    namespace Input {
        class IController {
            public:
                virtual Core::Game::InputStream GetInputStream() = 0;
        };
    }
}