#include <UI/Node.hpp>

#include <Assets/Assets.hpp>

#include <Game/Game.hpp>

namespace UI
{
    class Image : public Node
    {
        public:
            Image(const std::string &path, glm::vec2 position);
            void render() override;

        private:
            Engine::Assets::Texture *texture;
    };

    class TextButton : public Node
    {
        public:
            TextButton(const std::string &text, glm::vec2 position);
            void render() override;

        private:
            std::string text;
    };

    class TetrisPiece : public Node
    {
        public:
            TetrisPiece(Core::Game::Tile tile, u8 rotation);
            void  render() override;
            void setType(Core::Game::Tile tile) { this->tile = tile; }
            void setRotation(u8 rotation) { this->rotation = rotation; }

        Core::Game::Tile tile;
        u8 rotation;
    };

}