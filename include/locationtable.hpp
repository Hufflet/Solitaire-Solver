#pragma once

#include <stdint.h>
#include "card.hpp"

class LocationTable{
    /*
    The LocationTable class keeps track of where each card is. The class is
    used to quickly find possible moves.
    The approach is to create a table that stores the position of each card.
    Locations follow this standard:
           0:   Either in the hand or unknown
         1-7:   In one of the tableau
        8-11:   In one of the foundations
    The value of the card is used as the index into the table
    */
    public:
        // initializes all cards to be in the hand/unknown
        LocationTable();

        // returns the location of the given card
        uint8_t find(Card);

        // marks the given card's position as the given location
        void setLocation(Card, uint8_t loc);
    
    private:
        // array of all card locations
        uint8_t locs[LOC_TABLE_SIZE];
};