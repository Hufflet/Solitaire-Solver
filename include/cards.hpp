#pragma once

#include <stdint.h>

class Card{
    public:
        Card();
        void setValue(uint8_t suit, uint8_t number);
        uint8_t getValue();
    private:
        uint8_t value;
};


class Hand{
    public:
        Hand();
        bool canFlip();
        bool canReset();
        void flip();
        void reset();
        void replace();
        void placeCard(uint8_t index, Card);
        Card get();
    private:
        Card cards[52-(7+6+5+4+3+2+1)];
};


class RootDeck{
    public:
        RootDeck();
        void makeKnown(uint8_t value);
        void shuffleUnknown();
        void getNextUnknown();
    private:
        Card cards[52];
        Card* first_unknown;
        Card* cur_unknown;
};


class LocTable{
    public:
        LocTable();
        uint8_t find(Card);
        void move(Card, uint8_t loc);
    private:
        uint8_t* locs[64];
};


class Tableau{
    public:
        Tableau();
        bool hasCards();
        void addFrom(Tableau base, LocTable loc_t);
        void replaceUnknowns(RootDeck rd);
        void addCard(Card);
        Card popTop();
    private:
        Card cards[20];
        Card* first_faceup;
        Card* first_open;
        uint8_t loc;
};


class Foundation{
    public:
        Foundation();
        bool isFull();
        void addCard(Card);
        Card popTop();
    private:
        Card cards[14];
        Card* first_open;
};