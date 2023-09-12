#include <Randomizers/SevenBag.hpp>

namespace Core::Game
{
    SevenBagRandomizer::SevenBagRandomizer()
    {
        index = 0;
        prng = XoroshiroRandomizer();
    }

    void SevenBagRandomizer::Seed(int seed)
    {
        prng.seed(seed);
        Shuffle();
    }

    void SevenBagRandomizer::Shuffle()
    {
        for (int i = 0; i < 7; i++)
        {
            bag[i] = Tile(i);
        }

        // Fisher-Yates shuffle
        for (int i = 6; i > 0; i--)
        {
            int j = prng.next() % (i + 1);
            Tile temp = bag[i];
            bag[i] = bag[j];
            bag[j] = temp;
        }
    }

    Tile SevenBagRandomizer::GetNextPiece()
    {
        if (index == 7)
        {
            Shuffle();
            index = 0;
        }

        return bag[index++];
    }
}