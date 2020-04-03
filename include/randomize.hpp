#pragma once

#include <cstdint>

extern uint64_t PRNG_SEED;
extern uint32_t* PRNG_SEED_UPPER_HALF;

uint32_t xorshiftStar32();
void     shuffleBytes(uint8_t* start, uint8_t len);

#define rand32() xorshiftStar32()