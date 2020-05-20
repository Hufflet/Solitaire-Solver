#pragma once

//#define FLIP_3
#define HAND_SIZE 52-(7+6+5+4+3+2+1)

#define NUM_RANKS 13
#define NUM_SUITS 4
#define NUM_CARDS_IN_DECK NUM_RANKS*NUM_SUITS

#define LOC_TABLE_SIZE 64
#define FOUNDATION_SIZE 14
#define TABLEAU_SIZE 6+13+1

#define NUM_CARDS_IN_HAND 52-(7+6+5+4+3+2+1)
#define NUM_TABLEAUS 7
#define NUM_LOCATIONS 12

#define MOVE_TYPE_DRAW  0
#define MOVE_TYPE_RESET 1
#define MOVE_TYPE_H2T   2
#define MOVE_TYPE_H2F   3
#define MOVE_TYPE_T2T   4
#define MOVE_TYPE_T2F   5
#define MOVE_TYPE_F2T   6

#define KING_RANK 13