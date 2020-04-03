#include "../include/randomize.hpp"

uint64_t  PRNG_SEED = 0x2cf915021e8312a0; // randomly chosen from random.org

uint32_t xorshiftStar32(){
    PRNG_SEED ^= PRNG_SEED >> 12;
    PRNG_SEED ^= PRNG_SEED << 25;
    PRNG_SEED ^= PRNG_SEED >> 27;
    return (PRNG_SEED >> 32) * (uint64_t)(0x2545f4914f6cdd1d);
}

void shuffleBytes(uint8_t* arr, uint8_t len){
    for(int i=0; i<len-1; ++i){
        uint32_t R = rand32();
        uint64_t N = (uint64_t(R) * uint64_t(len-i)) >> 32; // N is bounded [0,N-i)
        uint8_t  temp = arr[i];
        arr[i]   = arr[i+N];
        arr[i+N] = temp;
    }
}