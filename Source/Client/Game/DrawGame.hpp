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

    // Positioning information
    const float TILE_SIZE = 16.0f;
    const float boardWidth = TILE_SIZE * Core::Game::BOARD_COLUMNS;
    const float BOARD_HEIGHT = TILE_SIZE * Core::Game::BOARD_ROWS;
    const float BORDER_WIDTH = 1.0f;
    const glm::vec2 PREVIEW_SIZE = {TILE_SIZE * 4.0f, TILE_SIZE * 4.0f};

    const glm::vec2 BOARD_SIZE = {boardWidth, BOARD_HEIGHT};
    const glm::vec2 GAME_SIZE = {boardWidth + PREVIEW_SIZE.x + 2.0f * BORDER_WIDTH, BOARD_HEIGHT + 2.0f * BORDER_WIDTH};
    const float SCOREBOARD_GAP = 8.0f;
}