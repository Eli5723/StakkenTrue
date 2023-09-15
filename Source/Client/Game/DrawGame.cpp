#include <Game/DrawGame.hpp>

#include <Draw/Draw.hpp>
#include <Assets/Assets.hpp>
#include <Game/Pieces.hpp>

constexpr glm::vec4 rgbToFloat(int r, int g, int b) { return glm::vec4{(float)r/255.0f,(float)g/255.0f,(float)b/255.0f,1.0f};};

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

    // Colors
    const float GHOST_ALPHA = 0.6f;
    const glm::vec4 WHITE{1.0f, 1.0f, 1.0f, 1.0f};
    const glm::vec4 BLACK{0.0f, 0.0f, 0.0f, 1.0f};

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
        Draw::Quad::Textured(position, {TILE_SIZE, TILE_SIZE}, activeTexture->id, activePallete->colors[tile]);

        // // Draw connections
        if (!(connections & Core::Game::UP))
        {
            Draw::Line::Line(position, {position.x + TILE_SIZE, position.y}, WHITE);
        }
        if (!(connections & Core::Game::RIGHT))
        {
            Draw::Line::Line({position.x + TILE_SIZE, position.y}, {position.x + TILE_SIZE, position.y + TILE_SIZE}, WHITE);
        }
        if (!(connections & Core::Game::DOWN))
        {
            Draw::Line::Line({position.x, position.y + TILE_SIZE}, {position.x + TILE_SIZE, position.y + TILE_SIZE}, WHITE);
        }
        if (!(connections & Core::Game::LEFT))
        {
            Draw::Line::Line(position, {position.x, position.y + TILE_SIZE}, WHITE);
        }
    }

    inline void drawGhostTile(glm::vec2 position, Core::Game::Tile tile, u8 connections)
    {
        Draw::Quad::Textured(position, {TILE_SIZE, TILE_SIZE}, activeTexture->id, activePallete->colors[tile] * GHOST_ALPHA);

        // Draw connections
        if (!(connections & Core::Game::UP))
        {
            Draw::Line::Line(position, {position.x + TILE_SIZE, position.y}, WHITE * GHOST_ALPHA);
        }
        if (!(connections & Core::Game::RIGHT))
        {
            Draw::Line::Line({position.x + TILE_SIZE, position.y}, {position.x + TILE_SIZE, position.y + TILE_SIZE}, WHITE * GHOST_ALPHA);
        }
        if (!(connections & Core::Game::DOWN))
        {
            Draw::Line::Line({position.x, position.y + TILE_SIZE}, {position.x + TILE_SIZE, position.y + TILE_SIZE}, WHITE * GHOST_ALPHA);
        }
        if (!(connections & Core::Game::LEFT))
        {
            Draw::Line::Line(position, {position.x, position.y + TILE_SIZE}, WHITE * GHOST_ALPHA);
        }
    }

    void drawGame(glm::vec2 position, Core::Game::Game& game) {
            drawBoard(position, game);
            auto ghostPosition = game.GetGhostPiecePosition();
            // Sum offsets below ghost piece

            drawGhostPiece(position + glm::vec2{ghostPosition.first * TILE_SIZE, (ghostPosition.second) * TILE_SIZE}, game.currentPiece.type, game.currentPiece.rotation);
            drawPiece(position + glm::vec2{game.currentPiece.x * TILE_SIZE, (game.currentPiece.y) * TILE_SIZE}, game.currentPiece.type, game.currentPiece.rotation);

            // Draw piece preview
            Draw::Quad::BorderBox(position + glm::vec2{boardWidth + SCOREBOARD_GAP, 0.0f}, PREVIEW_SIZE, BORDER_WIDTH, WHITE, BLACK);
            drawPiece(position + glm::vec2{boardWidth + SCOREBOARD_GAP, 0.0f}, game.nextPiece.type, game.nextPiece.rotation);
    }

    void drawPiece(glm::vec2 position, Core::Game::Tile type, i8 rotation)
    {

        Core::Game::Tile* blockGrid = Core::Game::getPieceDefinition(type, rotation);
        u8 *connections = Core::Game::getPieceConnections(type, rotation);

        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                if (blockGrid[x + y*4] != -1) {
                    drawTile(position + glm::vec2{TILE_SIZE*x, TILE_SIZE*y}, type, connections[x + y*4]);
                }
            }
        }
    }

    void drawGhostPiece(glm::vec2 position, Core::Game::Tile type, i8 rotation)
    {

        Core::Game::Tile* blockGrid = Core::Game::getPieceDefinition(type, rotation);
        u8 *connections = Core::Game::getPieceConnections(type, rotation);

        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                if (blockGrid[x + y*4] != -1) {
                    drawGhostTile(position + glm::vec2{TILE_SIZE*x, TILE_SIZE*y}, type, connections[x + y*4]);
                }
            }
        }
    }

    void drawRow(glm::vec2 position, Core::Game::Row& row) {
        for (int x = 0; x < Core::Game::BOARD_COLUMNS; x++) {
            if (row.contents[x] != -1) {
                drawTile(position + glm::vec2{TILE_SIZE*x, 0}, row.contents[x],  row.connections[x]);
            }
        } 
    }

    void drawBoard(glm::vec2 position, Core::Game::Game& game) {
        // Draw Border and background
        Draw::Quad::BorderBox(position, {boardWidth, BOARD_HEIGHT}, BORDER_WIDTH, WHITE, BLACK);

        // Draw Rows, starting from the bottom
        float offset = BOARD_HEIGHT;
        for (int y = Core::Game::BOARD_ROWS - 1; y >= 0; y--) {
            // IF there is a gap, draw it and offset the next row
            offset-= game.rowGaps[y];
            Draw::Quad::Flat(position + glm::vec2{0.0f, offset}, {boardWidth, game.rowGaps[y]}, WHITE);
            offset-= TILE_SIZE;
            drawRow(position + glm::vec2{0.0f, offset}, game.rows[y]);
        }
    }

    void drawSwoosh(glm::vec2 position) {

    }

    void drawClearEffect(glm::vec2 position, float size) { 
        Engine::Draw::Quad::Flat(position, {TILE_SIZE * Core::Game::BOARD_COLUMNS, size}, WHITE);
    }
}