#include <Game/Game.hpp>

#include <Assets/Assets.hpp>
#include <Game/ColorPallete.hpp>

namespace Game
{
    void setActiveTexture(Engine::Assets::Texture* texture);
    void setActivePallete(ColorPalette* pallete);

    void drawRow(glm::vec2 position, Core::Game::Row& row);
    void drawBoard(glm::vec2 position, Core::Game::Game& game);
    void drawPiece(glm::vec2 position, Core::Game::Tile type, i8 rotation);
    void drawGhostPiece(glm::vec2 position, Core::Game::Tile type, i8 rotation);

    void drawGame(glm::vec2 position, Core::Game::Game& testGame);

    void drawSwoosh(glm::vec2 position);
    void drawGap(glm::vec2 position);
}