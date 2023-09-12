#include <Randomizers/XoroshiroRandomizer.hpp>

void XoroshiroRandomizer::seed(int seed){
    s[0] = seed;
    s[1] = seed - 2354234;
    s[2] = seed - 7423145;
    s[3] = seed - 10;
}

u64 XoroshiroRandomizer::rotl(const u64 x, int k) {
    return (x << k) | (x >> (64 - k));
}

u64 XoroshiroRandomizer::next(void) {
    const u64 result = rotl(s[0] + s[3], 23) + s[0];

    const u64 t = s[1] << 17;

    s[2] ^= s[0];
    s[3] ^= s[1];
    s[1] ^= s[2];
    s[0] ^= s[3];

    s[2] ^= t;

    s[3] = rotl(s[3], 45);

    return result;
}