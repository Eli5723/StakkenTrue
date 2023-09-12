#include <Game/DrawGame.hpp>

#include <Draw/Draw.hpp>
#include <Assets/Assets.hpp>
#include <Game/Pieces.hpp>

#define rgbToFloat(r,g,b) {(float)r/255.0f,(float)g/255.0f,(float)b/255.0f,1.0f}

namespace Game
{

    using namespace Engine;

    ColorPalette defaultPallate = {
        {
            rgbToFloat(237, 41, 57),  // T
            rgbToFloat(255, 121, 0),  // O
            rgbToFloat(254, 203, 0),  // L
            rgbToFloat(105, 190, 40), // J
            rgbToFloat(0, 159, 218),  // Z
            rgbToFloat(0, 101, 189),  // S
            rgbToFloat(149, 45, 152), // I
            rgbToFloat(40, 40, 40)    // Garbage
        }
    };

    ColorPalette *activePallete = &defaultPallate;
    Assets::Texture *activeTexture;

    const float TILE_SIZE = 16.0f;
    const float boardWidth = TILE_SIZE * Core::Game::BOARD_COLUMNS;
    const float BOARD_HEIGHT = TILE_SIZE * Core::Game::BOARD_ROWS;

    void setActiveTexture(Assets::Texture *texture)
    {
        activeTexture = texture;
    }

    void setActivePallete(ColorPalette *pallete)
    {
        activePallete = pallete;
    }

    inline void drawTile(glm::vec2 position, Core::Game::Tile tile, u8 connections)
    {
        Draw::Quad::Textured(position, {16.0f, 16.0f}, activeTexture->id, activePallete->colors[tile]);
    }

    inline void drawGhostTile(glm::vec2 position, Core::Game::Tile tile, u8 connections)
    {
        Draw::Quad::Textured(position, {16.0f, 16.0f}, activeTexture->id, activePallete->colors[tile] * 0.6f);
    }

    void drawGame(glm::vec2 position, Core::Game::Game& game) {
            drawBoard(position, game);
            auto ghostPosition = game.GetGhostPiecePosition();
            // Sum offsets below ghost piece

            drawGhostPiece(position + glm::vec2{ghostPosition.first * 16.0f, (ghostPosition.second) * 16.0f}, game.currentPiece.type, game.currentPiece.rotation);
            drawPiece(position + glm::vec2{game.currentPiece.x * 16.0f, (game.currentPiece.y) * 16.0f}, game.currentPiece.type, game.currentPiece.rotation);

            // Draw piece preview
            Draw::Quad::BorderBox(position + glm::vec2{boardWidth + 8.0f, 0.0f}, {TILE_SIZE * 4.0f, TILE_SIZE * 4.0f}, 1.0f, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f});
            drawPiece(position + glm::vec2{boardWidth + 8.0f, 0.0f}, game.nextPiece.type, game.nextPiece.rotation);
    }

    void drawPiece(glm::vec2 position, Core::Game::Tile type, i8 rotation)
    {

        Core::Game::Tile* blockGrid = Core::Game::getPieceDefinition(type, rotation);

        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                if (blockGrid[x + y*4] != -1) {
                    drawTile(position + glm::vec2{TILE_SIZE*x, TILE_SIZE*y}, type, 0b00001111);
                }
            }
        }
    }

    void drawGhostPiece(glm::vec2 position, Core::Game::Tile type, i8 rotation)
    {

        Core::Game::Tile* blockGrid = Core::Game::getPieceDefinition(type, rotation);

        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                if (blockGrid[x + y*4] != -1) {
                    drawGhostTile(position + glm::vec2{TILE_SIZE*x, TILE_SIZE*y}, type, 0b00001111);
                }
            }
        }
    }

    void drawRow(glm::vec2 position, Core::Game::Row& row) {
        for (int x = 0; x < Core::Game::BOARD_COLUMNS; x++) {
            if (row.contents[x] != -1) {
                drawTile(position + glm::vec2{TILE_SIZE*x, 0}, row.contents[x], 0b00001111);
            }
        } 
    }

    void drawBoard(glm::vec2 position, Core::Game::Game& game) {
        // Draw Border and background
        Draw::Quad::BorderBox(position, {boardWidth, BOARD_HEIGHT}, 1.0f, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f});

        // Draw Rows, starting from the bottom
        float offset = BOARD_HEIGHT;
        for (int y = Core::Game::BOARD_ROWS - 1; y >= 0; y--) {
            // IF there is a gap, draw it and offset the next row
            offset-= game.rowGaps[y];
            Draw::Quad::Flat(position + glm::vec2{0.0f, offset}, {boardWidth, game.rowGaps[y]}, {1.0f, 1.0f, 1.0f, 1.0f});
            offset-= 16.0f;
            drawRow(position + glm::vec2{0.0f, offset}, game.rows[y]);
        }
    }

    void drawSwoosh(glm::vec2 position) {

    }

    void drawClearEffect(glm::vec2 position, float size) { 
        Engine::Draw::Quad::Flat(position, {TILE_SIZE * Core::Game::BOARD_COLUMNS, size}, {1.0f, 1.0f, 1.0f, 1.0f});
    }
}