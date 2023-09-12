#pragma once

#include <Randomizers/Randomizer.hpp>

struct XoroshiroRandomizer : Randomizer {
    u64 s[4];
    
    void seed(int s1);
    inline u64 rotl(const u64 x, int k);
    u64 next(void);
};