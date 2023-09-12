#pragma once

#include <glm/glm.hpp>

namespace Game
{
    void HSV2RGB(float h, float s, float v, float &r, float &g, float &b);
    void HSV2RGB(float h, float s, float v, glm::vec3 &vector);
    void HSV2RGB(float h, float s, float v, glm::vec4 &vector);
    void RGB2HSV(float &h, float &s, float &v, const glm::vec4 &color);
}