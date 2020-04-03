#pragma once

#include "params.hpp"
#include <stdint.h>

class Card{
    public:
        Card();
        void setState(bool known, uint8_t suit, uint8_t rank);
        uint8_t getValue();
        uint8_t getRank();
        void clear();
        void makeKnown();
    private:
        uint8_t state;
};


class RootDeck{
    public:
        RootDeck();
        void makeKnown(Card);
        void shuffleUnknown();
        Card getNextUnknown();
    private:
        Card cards[NUM_CARDS_IN_DECK];
        Card* first_unknown;
        Card* cur_unknown;
};


class Hand{
    public:
        Hand();
        bool canFlip();
        void flip();
        #ifdef CAN_RESET
            bool canReset();
            void reset();
        #endif
        void placeCard(uint8_t index, Card);
        void replaceUnknowns(RootDeck rd);
        Card get();
    private:
        Card cards[HAND_SIZE+1];
        Card* cur;
        uint8_t num_discard;
        uint8_t num_remain;
        uint8_t num_resets;
};


class LocTable{
    public:
        LocTable();
        uint8_t find(Card);
        void move(Card, uint8_t loc);
    private:
        uint8_t locs[LOC_TABLE_SIZE];
};


class Tableau{
    public:
        Tableau();
        void setLoc(uint8_t loc);
        bool hasCards();
        void addFrom(Tableau base, LocTable loc_t);
        void replaceUnknowns(RootDeck rd);
        void addCard(Card, LocTable loc_t);
        Card popTop();
    private:
        Card cards[TABLEAU_SIZE];
        Card* first_faceup;
        Card* first_open;
        uint8_t loc;
};


class Foundation{
    public:
        Foundation();
        void setLoc(uint8_t);
        bool isFull();
        void addCard(Card, LocTable loc_t);
        Card popTop();
    private:
        Card cards[FOUNDATION_SIZE];
        Card* first_open;
        uint8_t loc;
};