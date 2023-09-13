#pragma once

#include <defines.h>
#include <glm/glm.hpp>

namespace Engine::View {
    void Initialize(const glm::vec2& resolution);
    void Resize(const glm::vec2& resolution);
    void StartFrame();
    void EndFrame();
}