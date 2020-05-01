#pragma once

#include "params.hpp"
#include <stdint.h>

class Card{
    public:
        Card();
        uint8_t getValue();
        uint8_t getRank();
        void    setState(bool known, uint8_t suit, uint8_t rank);
        void    clear();
        void    makeKnown();
        bool    isKnown();
        uint8_t getSuit();
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
        uint8_t flip(uint8_t num_to_flip);
        void unFlip(uint8_t num_to_unflip);
        bool canReset();
        void reset();
        void unReset();
        void placeCard(uint8_t pos, Card, LocTable);
        void replaceUnknowns(RootDeck rd, LocTable);
        Card readTop();
        Card takeTop();
        void unTakeTop(Card, uint8_t num);
        uint8_t getPos();
        void revealDraw(Card, Card, Card, LocTable);
    private:
        Card cards[HAND_SIZE+1];
        Card* cur;
        Card* last_card;
        uint8_t num_resets;
};


class LocTable{
    public:
        uint8_t find(Card);
        void move(Card, uint8_t loc);
    private:
        uint8_t locs[LOC_TABLE_SIZE] = {0};
};


class Tableau{
    public:
        Tableau();
        void setLoc(uint8_t loc);
        bool hasCards();
        void addFrom(Tableau base, uint8_t num, LocTable loc_t);
        void replaceUnknowns(RootDeck rd, LocTable);
        void addCard(Card, LocTable loc_t);
        Card popTop();
    private:
        Card cards[TABLEAU_SIZE];
        Card* first_open;
        uint8_t loc;
};


class Foundation{
    public:
        Foundation();
        void setLoc(uint8_t);
        bool isFull();
        void addCard(Card, LocTable);
        Card popTop();
        Card readTop();
    private:
        Card cards[FOUNDATION_SIZE];
        Card* first_open;
        uint8_t loc;
};


class LoopTracker{
    public:
        void markAonB(Card, Card);
        void clearParents(Card);
        bool isClearAonB(Card, Card);
    private:
        uint8_t type0[64] = {0};
        uint8_t type1[64] = {0};
};