#pragma once

#include "card.hpp"
#include "params.hpp"


class Deck{
    /*
    The Deck class is used to track which cards are known and which are not. It
    also allows the unknown cards to be mixed via shuffling or permuting.
    The approach is to keep an array of all cards. Known cards are at the start
    of the array, unknown at the end. As cards are discovered, they are moved 
    to the start of the array. Shuffling only occurs on end of array.
    */
    public:
        // default Deck contains the 52 standard cards, all unknown
        Deck();

        // finds the given card in the deck and marks it as known
        // assumes the card is in the deck, and is unknown
        void makeKnown(Card);

        // mixes the unknown cards using Fisher-Yates algorithm
        void shuffleUnknown();

        // mixes the unknown cards using an iterative algorithm.
        // can be called from any state, automatically loops
        // requires all unknown cards to be uniquely sortable
        void permuteUnknown();

        // returns the current unknown card
        // adjusts internal counter to prepare for the next call
        Card getNextUnknown();

        // returns number of unknown cards
        uint8_t numUnknown();
    
    private:
        // array of all cards in the deck
        Card cards[NUM_CARDS_IN_DECK];

        // pointer to first unknown card
        Card* first_unknown;

        // marker for current unknown, used by getNextUnknown
        Card* cur_unknown;
};