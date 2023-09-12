#include <UI/UI.hpp>

#include <spdlog/spdlog.h>

namespace UI {

    // Settings
    struct Settings {
        Engine::Assets::Font *font;
    } settings;

    Engine::Assets::Font& GetFont() {
        return *settings.font;
    }


    // State
    struct State {
        Node *root;
    } state;

    // UI
    void Init() {
        spdlog::info("UI Init");
        
        settings.font = Engine::Assets::fontCache.get("Resources/Fonts/Roboto-Regular.ttf");
        state.root = new Node(glm::vec2(0, 0), glm::vec2(0, 0));
    }

    void Click(glm::vec2 position) {

    }

    void Render() {
        state.root->renderTree();
    }

    void AddToRoot(Node *node) {
        state.root->addChild(node);
    }
}