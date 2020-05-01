#pragma once

//#define FLIP_3
#define HAND_SIZE 52-(7+6+5+4+3+2+1)

#define RANK_BITS  0b00001111
#define VALUE_BITS 0b00111111
#define KNOWN_BITS 0b01000000
#define NUM_RANKS 13
#define NUM_SUITS 4
#define NUM_CARDS_IN_DECK NUM_RANKS*NUM_SUITS

#define LOC_TABLE_SIZE VALUE_BITS+1
#define FOUNDATION_SIZE 14
#define TABLEAU_SIZE 20