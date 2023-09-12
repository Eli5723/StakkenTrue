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
        rows[0].clear();
    }

    void Game::AddGarbage(u8 position)
    {
        memcpy(&rows[0], &rows[1], sizeof(Row) * (BOARD_ROWS - 1));
        rows[BOARD_ROWS - 1].clear();
        rows[BOARD_ROWS - 1].setGarbage(position);
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

    void Game::LockPiece(Piece &piece)
    {
        int run = 0;

        for (int ty = 0; ty < 4; ty++)
        {
            for (int tx = 0; tx < 4; tx++)
            {
                if (piece.defintion[tx + ty * 4] != GameTile::EMPTY)
                {
                    SetTile(piece.x + tx, piece.y + ty, piece.defintion[tx + ty * 4]);
                }
            }

            if (rows[piece.y + ty].filled())
            {
                run++;
                RemoveRow(piece.y + ty);
            } else {
                rowGaps[piece.y + ty - 1] += run * 16.0f;
                run = 0;
            }
        }

        if (run > 0) {
            rowGaps[piece.y + 3] += run * 16.0f;
        }

        Tile nextType = pieceRandomizer.GetNextPiece();
        currentPiece.Reset(nextPiece.type, nextPiece.rotation);
        nextPiece.Reset(nextType, initialRotationTable.pieces[nextType]);
    }

    std::pair<u8, u8> Game::GetGhostPiecePosition()
    {
        u8 intitialY = currentPiece.y;

        while (PieceFits(currentPiece))
        {
            currentPiece.MoveDown();
        }
        currentPiece.MoveUp();
        u8 ghostY = currentPiece.y;
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
            rowGaps[i] = std::max(rowGaps[i] - fdelta*196.0f, 0.0f);
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
    }

    void Game::HardDrop()
    {
        while (PieceFits(currentPiece))
        {
            currentPiece.MoveDown();
        }
        currentPiece.MoveUp();
        LockPiece(currentPiece);
    }

    void Game::ApplyEventStream(EventStream &stream)
    {
        while (stream.size > 0)
        {
            GameEvent event = stream.events[--stream.size];
            switch (event)
            {
            case GameEvent::MOVE_LEFT:
                MoveLeft();
                break;
            case GameEvent::INSTANT_LEFT:
                InstantLeft();
                break;
            case GameEvent::INSTANT_RIGHT:
                InstantRight();
                break;
            case GameEvent::MOVE_RIGHT:
                MoveRight();
                break;
            case GameEvent::RCW:
                RCW();
                break;
            case GameEvent::RCCW:
                RCCW();
                break;
            case GameEvent::FLIP:
                Flip();
                break;
            case GameEvent::SOFT_DROP:
                SoftDrop();
                break;
            case GameEvent::SONIC_DROP:
                SonicDrop();
                break;
            case GameEvent::HARD_DROP:
                HardDrop();
                break;
            }
        }
    }

} // namespace Core
