
#include "Random.hpp"
#include <cstdio>
#include <typeinfo>

using namespace cs354;

#define ROR(value, amt) (((value) >> (amt)) | ((value) << (32 - (amt))))
#define RAND_A 1103515245
#define RAND_C 12345

Random::Random() { }
Random::~Random() { }

/**
 * Implements a Linear Contruential Generator, but with a twist: the initial
 * seed is permuted and mixed to get two seeds, and the random numbers are
 * obtained from a mix of bits from two random streams which change position
 * between each iteration.
 * The motivation for this is to discard the low order bits of the LCG, which
 * tend to have a much lower period than the higher order bits. This should
 * (but I haven't tested) improve the randomness of the values overall.
 */
LCG_Random::LCG_Random(int32_t seed) {
    reseed(seed);
}
LCG_Random::LCG_Random(int32_t s0, int32_t s1) {
    seed[0] = s0; seed[1] = s1;
}
LCG_Random::~LCG_Random() { }

int32_t LCG_Random::nextInt() {
    seed[0] = RAND_A * seed[0] + RAND_C;
    seed[1] = RAND_A * seed[1] + RAND_C;
    
    int32_t rval = ((((uint32_t)seed[0]) & 0xFFFF0000) >> 16) |
        (((uint32_t)seed[1]) & 0xFFFF0000);
    
    /* swap the seed values */
    seed[0] = seed[0] ^ seed[1];
    seed[1] = seed[0] ^ seed[1];
    seed[0] = seed[0] ^ seed[1];
    
    return ROR(rval, 9);
}

void LCG_Random::reseed(int32_t seed) {
   /* Get two starting values from the seed, using the LCG formula with bit
     * mixing.
     */
    seed = seed * RAND_A + RAND_C;
    seed = ROR(seed, 3) ^ ROR(seed, 7) ^ ROR(seed, 17);
    seed = seed * RAND_A + RAND_C;
    seed = ROR(seed, 3) ^ ROR(seed, 7) ^ ROR(seed, 17);
    this->seed[0] = seed;
    seed = seed * RAND_A + RAND_C;
    seed = ROR(seed, 3) ^ ROR(seed, 7) ^ ROR(seed, 17);
    seed = seed * RAND_A + RAND_C;
    seed = ROR(seed, 3) ^ ROR(seed, 7) ^ ROR(seed, 17);
    this->seed[1] = seed;
}

int32_t LCG_Random::nextInt(int32_t min, int32_t max) {
    return (nextInt() % (max - min)) + min;
}

double LCG_Random::nextRational() {
    int32_t numerator = nextInt();
    int32_t denominator = nextInt();
    if(denominator == 0) {
        return 0.0;
    }
    return ((double)numerator) / ((double)denominator);
}

Random * LCG_Random::copy() const {
    return new LCG_Random(seed[0], seed[1]);
}

bool LCG_Random::operator==(const Random &rand) const {
    if(typeid(*this) == typeid(rand)) {
        const LCG_Random *rhs = reinterpret_cast<const LCG_Random *>(&rand);
        return (rhs->seed[0] == seed[0] && rhs->seed[1] == seed[1]);
    }
    return false;
}
