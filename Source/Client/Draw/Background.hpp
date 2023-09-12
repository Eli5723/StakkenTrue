#pragma once

#include <glm/glm.hpp>

namespace Engine::Draw::Background
{
    void Draw();
    void Update();
    void Init(glm::vec2 resolution);
    void SetResolution(const glm::vec2 &resolution);
}