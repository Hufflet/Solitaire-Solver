#pragma once

#include <stdint.h>

class Card{
    /*
    The Card class tracks the data associated with a card: number, suit, if it
    is known, etc. 
    */
    public:
        // initialized as blank card
        Card();

        // value gives combined rank and suit
        // must be set thru rank and suit
        uint8_t getValue();

        // rank is the number, 1-13
        uint8_t getRank();
        void setRank(uint8_t);

        // suit is 1-4, CHSD order (CHaSeD)
        uint8_t getSuit();
        void setSuit(uint8_t);
        
        // known indicates if card's position is actually known (not guessed)
        void makeKnown();
        bool isKnown();

        // becomes blank card
        void clear();

        // becomes generic nonblank card
        // isKnown = false; rank = 15; value = 4;
        void generic();

        // returns child card, takes in 0 or 1
        Card getChild(bool);

        // returns parent card, takes in 0 or 1
        Card getParent(bool);

        // returns other color (zeroth suit of other color)
        uint8_t getOtherColor();

    private:
        uint8_t state;
};
