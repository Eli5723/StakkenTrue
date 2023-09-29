#include <Game/Game.hpp>

#include <cmath>
#include <memory.h>
#include <algorithm>

namespace Core::Game
{

    void Game::StartGame()
    {
        for (int y = 0; y < BOARD_ROWS; y++)
        {
            rows[y].clear();
        }
        SetSeed(rand());

        Tile firstType = pieceRandomizer.GetNextPiece();
        Tile nextType = pieceRandomizer.GetNextPiece();

        currentPiece = Piece(firstType, initialRotationTable.pieces[firstType]);
        nextPiece = Piece(nextType, initialRotationTable.pieces[nextType]);
    }

    void Game::SetSeed(u64 seed)
    {
        pieceRandomizer.Seed(seed);
        holeRandomizer.seed(seed);
        this->seed = seed;
    }

    void Game::RemoveRow(u8 y)
    {
        for (int i = y; i > 0; i--)
        {
            memcpy(&rows[i], &rows[i - 1], sizeof(Row));
        }

        if (y < BOARD_ROWS - 1) {
            for (int x = 0; x < BOARD_COLUMNS; x++)
            {
                rows[y + 1].connections[x] &= ~DIRECTION::UP;
            }
        }

        if (y > 0) {
            for (int x = 0; x < BOARD_COLUMNS; x++)
            {
                rows[y - 1].connections[x] &= ~DIRECTION::DOWN;
            }
        }

        rows[0].clear();
    }

    void Game::AddGarbage(u8 position)
    {
        memcpy(&rows[0], &rows[1], sizeof(Row) * (BOARD_ROWS - 1));
        rows[BOARD_ROWS - 1].setGarbage(position);

        for (int x = 0; x < BOARD_COLUMNS; x++) {
            // Connect garbage to the row above
            if (rows[BOARD_ROWS - 2].contents[x] == GameTile::GARBAGE) {
                rows[BOARD_ROWS - 1].connections[x] |= DIRECTION::UP;
            }
            if (rows[BOARD_ROWS - 1].contents[x] == GameTile::EMPTY) {
                rows[BOARD_ROWS - 2].connections[x] &= ~DIRECTION::DOWN;
            }
        }
    }

    void Game::QueueGarbage(u32 lines)
    {
        garbagePending += lines;
        garbageTimer = garbageRate;
    }

    bool Game::PieceFits(Piece &piece)
    {
        for (int tile_y = 0; tile_y < 4; tile_y++)
        {
            for (int tile_x = 0; tile_x < 4; tile_x++)
            {
                if (piece.GetTile(tile_x, tile_y) != GameTile::EMPTY)
                {
                    if (!BoundsX(piece.x + tile_x))
                    {
                        return false;
                    }
                    if (!BoundsY(piece.y + tile_y))
                    {
                        return false;
                    }
                    if (rows[piece.y + tile_y].contents[piece.x + tile_x] != GameTile::EMPTY)
                    {
                        return false;
                    }
                }
            }
        }

        return true;
    }

    void Game::Gravity()
    {
        currentPiece.MoveDown();
        if (!PieceFits(currentPiece))
        {
            currentPiece.MoveUp();
            LockPiece(currentPiece);
        }
    }

    void Game::LockPiece(Piece &piece)
    {
        int run = 0;
        int cleared = 0;

        // Lock piece
        for (int ty = 0; ty < 4; ty++)
        {
            for (int tx = 0; tx < 4; tx++)
            {
                if (piece.defintion[tx + ty * 4] != GameTile::EMPTY)
                {
                    SetTile(piece.x + tx, piece.y + ty, piece.defintion[tx + ty * 4], piece.connections[tx + ty * 4]);
                }
            }

            if (rows[piece.y + ty].filled())
            {
                run++;
                cleared++;
                RemoveRow(piece.y + ty);
            } else {
                rowGaps[piece.y + ty - 1] += run * 16.0f;
                run = 0;
            }
        }

        if (cleared > 0) {
            OnLineClear();
            combo++;

            comboTimer += ComboTimeTable[combo] + cleared * ComboBonusTimeTable[combo];

            OnCombo(combo);
        }

        if (run > 0) {
            rowGaps[piece.y + 3] += run * 16.0f;
        }

        // Spawn next piece
        Tile nextType = pieceRandomizer.GetNextPiece();
        currentPiece.Reset(nextPiece.type, nextPiece.rotation);
        nextPiece.Reset(nextType, initialRotationTable.pieces[nextType]);
        OnPieceLock();

        if (!PieceFits(currentPiece))
        {
            OnTopOut();
        }
    }

    std::pair<i8, i8> Game::GetGhostPiecePosition()
    {
        i8 intitialY = currentPiece.y;

        while (PieceFits(currentPiece))
        {
            currentPiece.MoveDown();
        }
        currentPiece.MoveUp();
        i8 ghostY = currentPiece.y;
        currentPiece.y = intitialY;

        return std::make_pair(currentPiece.x, ghostY);

    }

    bool Game::OffsetTest()
    {
        if (PieceFits(currentPiece)) // 0,0
            return true;
        currentPiece.x--;
        if (PieceFits(currentPiece)) //-1,0
            return true;
        currentPiece.x += 2;
        if (PieceFits(currentPiece)) // 1,0
            return true;
        currentPiece.x--;
        currentPiece.y++;
        if (PieceFits(currentPiece)) // 0,+1
            return true;
        currentPiece.x--; //-1,+1
        if (PieceFits(currentPiece))
            return true;
        currentPiece.x += 2; // 1,+1
        if (PieceFits(currentPiece))
            return true;
        currentPiece.y--;
        currentPiece.x -= 3; //-2,0
        if (PieceFits(currentPiece))
            return true;
        currentPiece.x += 4; // 2,0
        if (PieceFits(currentPiece))
            return true;
        currentPiece.x -= 2;
        currentPiece.MoveLeft();
        if (!PieceFits(currentPiece))
        {
            currentPiece.MoveRight();
        }

        return false;
    }

    void Game::Tick(u8 dt) {
        float fdelta = (float)dt / 1000.0f;

        for (int i=0; i < BOARD_ROWS; i++) {
            rowGaps[i] = std::max(rowGaps[i] - fdelta*256.0f, 0.0f);
        }

        printf("Gravity timer: %d\n", gravityTimer);

        // gravityTimer -= dt;
        // if (gravityTimer < 0) {
        //     gravityTimer += gravityPeriod;
        //     Gravity();
        // }
        if (comboTimer > 0) {
            comboTimer -= dt;
            if (comboTimer < 0) {
                combo = 0;
                comboTimer = 0;
            }
        }
 

        if (garbagePending > 0) {
            garbageTimer -= dt;
            if (garbageTimer < 0) {
                garbagePending--;
                garbageTimer += garbageRate;
                AddGarbage((u8)(holeRandomizer.next() % BOARD_COLUMNS));
            }
        } else {
            garbageTimer = std::max(garbageTimer - dt, 0);
        }
    }

    void Game::MoveLeft() {
        currentPiece.MoveLeft();
        if (!PieceFits(currentPiece)) {
            currentPiece.MoveRight();
        }
    }

    void Game::InstantLeft()
    {
        currentPiece.MoveLeft();
        while (PieceFits(currentPiece))
        {
            currentPiece.MoveLeft();
        }
        currentPiece.MoveRight();
    }

    void Game::MoveRight()
    {
        currentPiece.MoveRight();
        if (!PieceFits(currentPiece))
        {
            currentPiece.MoveLeft();
        }
    }

    void Game::InstantRight()
    {
        currentPiece.MoveRight();
        while (PieceFits(currentPiece))
        {
            currentPiece.MoveRight();
        }
        currentPiece.MoveLeft();
    }

    void Game::RCW()
    {
        currentPiece.RCW();
        if (!OffsetTest())
        {
            currentPiece.RCCW();
            return;
        }
    }
    void Game::RCCW()
    {
        currentPiece.RCCW();
        if (!OffsetTest())
        {
            currentPiece.RCW();
            return;
        }
    }
    void Game::Flip()
    {
        currentPiece.Flip();
        if (!OffsetTest())
        {
            currentPiece.Flip();
            return;
        }
    }

    void Game::SoftDrop()
    {
        currentPiece.MoveDown();

        if (!PieceFits(currentPiece))
        {
            currentPiece.MoveUp();
            LockPiece(currentPiece);
        }
    }

    void Game::SonicDrop()
    {
        currentPiece.MoveDown();
        while (PieceFits(currentPiece))
        {
            currentPiece.MoveDown();
        }
        currentPiece.MoveUp();
        OnSonicDrop();
    }

    void Game::HardDrop()
    {
        while (PieceFits(currentPiece))
        {
            currentPiece.MoveDown();
        }
        currentPiece.MoveUp();
        LockPiece(currentPiece);
        OnHardDrop();
    }

    void Game::ApplyInputStream(InputStream &stream)
    {
        while (stream.size > 0)
        {
            GameInput event = stream.events[--stream.size];
            switch (event)
            {
            case GameInput::MOVE_LEFT:
                MoveLeft();
                break;
            case GameInput::INSTANT_LEFT:
                InstantLeft();
                break;
            case GameInput::INSTANT_RIGHT:
                InstantRight();
                break;
            case GameInput::MOVE_RIGHT:
                MoveRight();
                break;
            case GameInput::RCW:
                RCW();
                break;
            case GameInput::RCCW:
                RCCW();
                break;
            case GameInput::FLIP:
                Flip();
                break;
            case GameInput::SOFT_DROP:
                SoftDrop();
                break;
            case GameInput::SONIC_DROP:
                SonicDrop();
                break;
            case GameInput::HARD_DROP:
                HardDrop();
                break;
            }
        }
    }

} // namespace Core
