#pragma once 

#include <Game/Pieces.hpp>

#include <Randomizers/XoroshiroRandomizer.hpp>

namespace Core {
    namespace Game {
        class SevenBagRandomizer {
            public:
                SevenBagRandomizer();
                void Seed(int seed);
                Tile GetNextPiece();

            private:
                void Shuffle();
                int index;
                Tile bag[7]{};
                XoroshiroRandomizer prng;
        };
    }
}