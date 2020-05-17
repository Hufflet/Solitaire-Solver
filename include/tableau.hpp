#pragma once

#include <stdint.h>
#include "card.hpp"
#include "deck.hpp"
#include "locationtable.hpp"
#include "params.hpp"

class Tableau{
    /*
    The Tableau class is used to control each column of cards. Each Tableau has
    a unique location (1-7).
    The approach is to use a Card array that is the max length that any Tableau
    could be. A pointer to the first blank space is kept. 
    */
    public:
        // creates an empty Tableau
        Tableau();

        // given position should be between 1 and 7
        void setLocation(uint8_t loc);

        // fills the Tableau with default unknown cards
        // does not put the top card visible at the beginning of the game
        void fillUnknown(Deck);

        // returns if there are any cards in the Tableau
        bool hasCards();

        // places given card on top of the Tableau
        // updates the card location in LocationTable
        void addCard(Card, LocationTable);

        // moves given number of cards from given Tableau to self
        // updates the card locations in LocationTable
        void addFrom(Tableau, uint8_t num, LocationTable);

        // returns copy of top card
        // assumes that there is a top card (hasCards returns true)
        Card readTop();

        // returns copy of top card
        // removes top card from the Tableau
        // assumes that there is a top card (hasCards returns true)
        Card takeTop();

        // replaces unknown cards with ones from Deck
        void replaceUnknowns(Deck);

    private:
        // array of cards in Tableau
        Card cards[TABLEAU_SIZE];

        // pointer to first open space in Tableau
        Card *first_open;

        // location of Tableau (1-7)
        uint8_t loc;
};