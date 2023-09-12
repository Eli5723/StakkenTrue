#pragma once

#include <defines.h>

namespace Core {
namespace Game {

    using Tile = i8;

    enum GameTile : Tile {
        EMPTY = -1,
        T,
        O,
        L,
        J,
        Z,
        S,
        I,
        GARBAGE
    };

    enum DIRECTION {
        UP = BIT(0),
        RIGHT = BIT(1),
        DOWN = BIT(2),
        LEFT = BIT(3)
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

    // Return the grid of tiles corresponding to the piece / rotation
    Tile* getPieceDefinition(Tile type, i8 rotation);

    // Return the grid of connections corresponding to the piece / rotation
    i8* getPieceConnections(Tile type, i8 rotation);
}
}