#pragma once

#include <UI/Node.hpp>

#include <Assets/Assets.hpp>

namespace UI {
    void Init();
    void Click(glm::vec2 position);
    void Render(); 
    void AddToRoot(Node *node);

    Engine::Assets::Font& GetFont();
}