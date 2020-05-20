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

        // fills the Tableau with mock cards
        void fillMock();

        // returns if there are any cards in the Tableau
        bool hasCards();

        // places given card on top of the Tableau
        // updates the card location in LocationTable
        void addCard(Card, LocationTable*);

        // moves given number of cards from given Tableau to self
        // updates the card locations in LocationTable
        void addFrom(Tableau, uint8_t num, LocationTable*);

        // returns copy of top card
        // assumes that there is a top card (hasCards returns true)
        Card readTop();

        // returns copy of top card
        // removes top card from the Tableau
        // assumes that there is a top card (hasCards returns true)
        Card takeTop();

        // replaces unknown cards with ones from Deck
        void replaceUnknowns(Deck);

        // returns location
        uint8_t getLocation();

        // tells if removing num cards would reveal an unknown card
        bool wouldReveal(uint8_t num);

        // tells position of card from top (1)
        // assumes card is in tableau
        uint8_t getPosition(Card);

        // puts a given card in a specific position
        // overwrites whatever was there
        void placeCard(Card, uint8_t pos, LocationTable*);

    private:
        // array of cards in Tableau
        Card cards[TABLEAU_SIZE];

        // pointer to first open space in Tableau
        Card *first_open;

        // location of Tableau (1-7)
        uint8_t loc;
};