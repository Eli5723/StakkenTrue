#pragma once

#include <Draw/Quad.hpp>

#include <Assets/Font.hpp>

namespace Engine {
namespace Draw {
namespace Text {

void Immediate(const glm::vec2& position, const char* text, const Assets::Font& font);


}
}
}