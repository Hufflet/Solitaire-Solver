#include "../include/randomize.hpp"

uint64_t  PRNG_SEED = 0x2cf915021e8312a0; // randomly chosen from random.org

uint64_t xorshiftStar32(){
    PRNG_SEED ^= PRNG_SEED >> 12;
    PRNG_SEED ^= PRNG_SEED << 25;
    PRNG_SEED ^= PRNG_SEED >> 27;
    return (PRNG_SEED >> 32) * (uint64_t)(0x2545f4914f6cdd1d);
}

template <class T>
uint64_t randInRange(T range){
    uint64_t rand = xorshiftStar32();
    return (rand * uint64_t(range-1)) >> 32;
}

template<class T>
void swap(T *A, T *B){
    T temp = *A;
    *A = *B;
    *B = temp;
}

template<class T1, class T2>
void reverse(T1 *array, T2 len){
    for(T2 i=0; 2*i+1<len; ++i){
        swap(array+i, array+(len-1)-i);
    }
}

template <class T1, class T2>
void shuffleArray(T1 *array, T2 len){
    for(T2 i=0; i<len-1; ++i){
        uint64_t rand = i + randInRange(len - i);
        swap(array+i, array+rand);
    }
}

template <class T1, class T2>
void permuteArray(T1 *array, T2 len){
    // find first element followed by a larger one
    T2 pivot = 0;
    while(pivot < len-1){
        if(array[pivot] < array[pivot+1]){
            break;
        }
        ++pivot;
    }

    // if pivot is the last element, just reverse the list
    if(pivot == len-1){
        reverse(array, len);
        return;
    }
    
    // find largest element [left] of pivot that is <= array[pivot+1]+1
    // since these elements are in decreasing order, just start
    // at the pivot and go left
    T2 swap_index = pivot;
    for(T2 i=pivot; i>=0; --i){
        if(array[swap_index] > array[pivot+1]+1){
            swap_index = i;
        }
        else{
            break;
        }
    }

    // now swap that value with the one after pivot
    swap(array+swap_index, array+pivot+1);

    // finally, reverse the elements [left] of pivot
    reverse(array, pivot+1);
}