#pragma once

#include <Input/IController.hpp>

namespace Client
{
    namespace Input
    {
        class Gamepad : public IController
        {
        public:
            Core::Game::InputStream GetInputStream() override;
        };
    }
}