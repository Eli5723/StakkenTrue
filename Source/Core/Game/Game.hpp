#pragma once

#include <defines.h>

#include <Randomizers/Randomizers.hpp>
#include <Game/Pieces.hpp>
#include <Game/GameInput.hpp>

#include <utility>
#include <cstring>
#include <rocket.hpp>

namespace Core::Game
{
    union RotationTable
    {
        u8 pieces[7];
        struct
        {
            u8 T;
            u8 O;
            u8 L;
            u8 J;
            u8 Z;
            u8 S;
            u8 I;
        };
    };

    /*
    Binary Order of Connections,1 - 16
    UP
    RIGHT
    UP RIGHT
    DOWN
    UP DOWN
    RIGHT DOWN
    UP RIGHT DOWN
    LEFT
    UP LEFT
    RIGHT LEFT
    UP RIGHT LEFT
    DOWN LEFT
    UP DOWN LEFT
    RIGHT DOWN LEFT
    UP RIGHT DOWN LEFT
    */

    const int BOARD_ROWS = 20;
    const int BOARD_COLUMNS = 10;

    struct Row
    {
        Tile contents[BOARD_COLUMNS];
        u8 connections[BOARD_COLUMNS];
        u8 fill;

        void clear()
        {
            memset(contents, -1, sizeof(Tile) * BOARD_COLUMNS);
            memset(connections, 0, sizeof(u8) * BOARD_COLUMNS);
            fill = 0;
        }

        void setGarbage(u8 hole) {
            memset(contents, GameTile::GARBAGE, sizeof(Tile) * BOARD_COLUMNS);
            contents[hole] = GameTile::EMPTY;
            memset(connections, 0, sizeof(u8) * BOARD_COLUMNS);
            fill = BOARD_COLUMNS - 1;
        }

        inline bool filled()
        {
            return fill == BOARD_COLUMNS;
        }
    };

    class Piece
    {
    public:
        Piece(Tile type, u8 rotation)
        {
            defintion = getPieceDefinition(type, rotation);
            connections = getPieceConnections(type, rotation);
            this->type = type;
            this->rotation = rotation;
            x = 3;
            y = 0;
        }

        void Reset(Tile type, u8 rotation)
        {
            defintion = getPieceDefinition(type, rotation);
            connections = getPieceConnections(type, rotation);
            this->type = type;
            this->rotation = rotation;
            x = 3;
            y = 0;
        };

        void RCW()
        {
            rotation = (rotation + 1) % 4;
            defintion = getPieceDefinition(type, rotation);
            connections = getPieceConnections(type, rotation);
        }

        void RCCW()
        {
            rotation = (rotation + 3) % 4;
            defintion = getPieceDefinition(type, rotation);
            connections = getPieceConnections(type, rotation);
        }

        void Flip()
        {
            rotation = (rotation + 2) % 4;
            defintion = getPieceDefinition(type, rotation);
            connections = getPieceConnections(type, rotation);
        }

        inline Tile GetTile(int x, int y)
        {
            return defintion[x + y * 4];
        }

        void MoveLeft()
        {
            x--;
        }

        void MoveRight()
        {
            x++;
        }

        void MoveDown()
        {
            y++;
        }

        void MoveUp()
        {
            y--;
        }

        Tile *defintion;
        u8* connections;
        Tile type;
        i8 x;
        i8 y;
        i8 rotation;
    };

    class Game
    {
    public:
        // Player Configuration
        RotationTable initialRotationTable;

        // State
        Row rows[BOARD_ROWS];
        Piece currentPiece{0, 0};
        Piece nextPiece{0, 0};
        SevenBagRandomizer pieceRandomizer{};
        XoroshiroRandomizer holeRandomizer{};
        u64 seed;

        u32 gravityPeriod = 0;
        u32 gravityTimer = 0;

        u32 lockDelay = 300;
        u32 lockTimer = 0;

        i32 comboTimer = 0;
        u32 combo = 0;

        constexpr static i32 ComboBonusTimeTable[]{
            1200, 600, 300, 200, 100, 0, 0, 0, 0, 0, 0, 0, -100, -200, -300, -600, -1200, -1200
        };

        constexpr static i32 ComboGarbageTable[]{
            0, 1, 1, 1, 2, 2, 3, 5, 7, 9, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
        };

        constexpr static i32 ComboTimeTable[]{
            2400, 1100, 400, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        };

        // Graphical effects
        float verticalOffset = 0;
        float rowGaps[BOARD_ROWS]{0};

        rocket::signal<void()> OnGameStart;
        rocket::signal<void()> OnPieceLock;
        rocket::signal<void()> OnLineClear;
        rocket::signal<void()> OnSonicDrop;
        rocket::signal<void()> OnHardDrop;
        rocket::signal<void(u32 combo)> OnCombo;
        

        // Helpers
        inline bool BoundsY(u8 y)
        {
            return y >= 0 && y < BOARD_ROWS;
        }

        inline bool BoundsX(u8 x)
        {
            return x >= 0 && x < BOARD_COLUMNS;
        }

        inline void SetTile(u8 x, u8 y, Tile tile, u8 connections)
        {
            rows[y].contents[x] = tile;
            rows[y].connections[x] = connections;
            rows[y].fill++;
        }

        inline Tile GetTile(u8 x, u8 y, Tile tile)
        {
            return rows[y].contents[x];
        }

        void StartGame();
        void SetSeed(u64 seed);
        void RemoveRow(u8 y);
        void AddGarbage(u8 position);
        bool PieceFits(Piece &piece);

        void LockPiece(Piece &piece);
        bool OffsetTest();

        std::pair<i8,i8> GetGhostPiecePosition();

        // Gameplay
        void Tick(u8 dt);

        // Player Inputs
        void MoveLeft();
        void MoveRight();
        void InstantLeft();
        void InstantRight();
        void RCW();
        void RCCW();
        void Flip();
        void SoftDrop();
        void SonicDrop();
        void HardDrop();
        void ApplyInputStream(InputStream &stream);
    };
}