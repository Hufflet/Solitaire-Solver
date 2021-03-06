#pragma once

#include <stdint.h>
#include "card.hpp"
#include "locationtable.hpp"
#include "params.hpp"


class Foundation{
    /*
    The Foundation class keeps track of cards moved to the foundation. The
    cards must be of the same suit in ascending order.
    The approach is to use an array of cards. The location is given in range
    [8-11], in order of suit.
    */
    public:
        // Initialized to be empty
        Foundation();

        // Sets the location [8-11]
        void setLocation(uint8_t);
        uint8_t getLocation();

        // returns if the foundation is empty
        void isEmpty();

        // returns if the foundation is full
        void isFull();

        // adds card to foundation
        // updates LocationTable for card
        void addCard(Card, LocationTable*);

        // returns copy of top card
        // assumes there is a top card
        Card readTop();

        // returns copy of top card
        // assumes there is a top card
        // removes top card from foundation
        Card takeTop();

        // returns distance of card from top (1)
        uint8_t getPosition(Card);

        // puts a given card in a specific position
        // overwrites whatever was there
        void placeCard(Card, uint8_t pos, LocationTable*);

        // returns number of cards currently in foundation
        uint8_t numCards();

    private:
        // array of cards
        Card cards[FOUNDATION_SIZE];

        // pointer to first open spot
        Card *first_open;

        // location
        uint8_t loc;
};