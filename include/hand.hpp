#pragma once

#include <stdint.h>
#include "card.hpp"
#include "deck.hpp"
#include "locationtable.hpp"

class Hand{
    /*
    The Hand class tracks card positions within the hand. This includes both
    the cards that have been drawn (discard) and the ones that are still
    facedown.
    The approach used is to just have an array of all cards assigned to the
    hand, buffered with a blank space at the beginning. Removed cards are
    replaced with a blank space to make returning them simple. The current
    position in the Hand is tracked, indicating what card is available for
    play.
    */
    public:
        // num_resets tells how many times the hand can be reset
        // num_flip tells how many cards are typically flipped per draw
        Hand(uint8_t num_resets, uint8_t num_flip);

        // returns how many cards can currently flip
        // given in range [0, num_flip]
        uint8_t howManyCanFlip();

        // flips the stated number of cards
        // assumes that the given number of cards can be flipped
        void flip(uint8_t num_to_flip);

        // unflips the stated number of cards
        // assumes that the given number of cards can be unflipped
        void unflip(uint8_t num_to_unflip);

        // tells if the hand could currently be reset
        bool canReset();

        // resets the hand
        // assumes the hand can currently be reset
        void reset();

        // undoes a reset
        // assumes the hand was just reset
        void unreset();

        // returns copy of card currently available
        Card readTop();

        // returns copy of card currently available
        // removes the card from the Hand
        // adjusts internal counter to point to the new top card
        Card takeTop();

        // places given card at given position
        // moves current position to given position
        // updates card location in LocationTable
        void untakeTopCard(Card, uint8_t pos, LocationTable*);

        // returns current position in the hand
        uint8_t getPos();

        // places given card at given position
        // updates card location in LocationTable
        void placeCard(Card, uint8_t pos, LocationTable*);

        // replaces each unknown card with an unknown card from the Deck
        // updates each new card's location in LocationTable
        void replaceUnknowns(Deck, LocationTable*);
    
    private:
        // array of all cards in hand, plus a buffer
        Card cards[NUM_CARDS_IN_HAND + 1];

        // pointer to currently available card
        Card *cur;

        // pointer to last nonblank card
        Card *last_card;

        // counter for how many more times the hand can be reset
        uint8_t resets_remaining;

        // number of cards to typically flip over at a time during draw
        uint8_t flip_size;
};