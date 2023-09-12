#include <UI/Components.hpp>

#include <Assets/Assets.hpp>
#include <Game/Game.hpp>
#include <UI/UI.hpp>
#include <Draw/Draw.hpp>
#include <Game/DrawGame.hpp>

namespace UI
{
    Image::Image(const std::string &path, glm::vec2 position) : Node(position, glm::vec2(0, 0))
    {
        texture = Engine::Assets::textureCache.get(path);
        this->position = position;
        this->size = texture->size;
    }

    void Image::render()
    {
        Engine::Draw::Quad::Textured(position, size, texture->id);
    }

    TextButton::TextButton(const std::string &text, glm::vec2 position)
    {
        this->text = text;
        this->position = position;
        this->size = glm::vec2(100, 100);
    }

    void TextButton::render()
    {
        Engine::Draw::Text::Immediate(position, text.c_str(), UI::GetFont());
    }

    TetrisPiece::TetrisPiece(Core::Game::Tile tile, u8 rotation) : tile(tile), rotation(rotation)
    {
        this->position = glm::vec2(0, 0);
        this->size = glm::vec2(16.0f * 4, 16.0f * 4);
        this->tile = tile;
        this->rotation = rotation;
    }

    void TetrisPiece::render()
    {
        Game::drawPiece(position, tile, rotation);
    }
}