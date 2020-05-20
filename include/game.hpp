#pragma once

#include <stdint.h>
#include <vector>
#include "move.hpp"
#include "card.hpp"
#include "deck.hpp"
#include "hand.hpp"
#include "foundation.hpp"
#include "locationtable.hpp"
#include "params.hpp"
#include "tableau.hpp"
#include "pushpull.hpp"


class Game{
    /*
    The Game class keeps track of the game state. The class can make and undo
    moves, and can return the possible moves from the current state. The method
    for finding moves includes restrictions to prune the tree, so to find all
    possible moves, first call resetRestrictions. The Game can also be told 
    where known cards are.
    */
    public:
        // initialized as blank game, all cards are mock
        Game();

        // returns number of unknown cards
        uint8_t numUnknown();

        // sets parameters so that all legal moves from this state are found
        void resetRestrictions();

        // finds moves from this state
        std::vector<Move> getMoves();

        // makes the move, updates state and restrictions accordingly
        // assumes the move is possible from this state
        void makeMove(Move);

        // undoes move, updates states and restrictions accordingly
        // assumes undoing the move is possible from this state
        void undoMove(Move);

        // places Card at given location/position
        // updates state to account for known card
        void placeKnown(Card, uint8_t loc, uint8_t pos);

        // shuffles the unknown cards
        // applies to all unknown cards in hand and tableau
        void shuffleUnknowns();

        // permutes the unknown cards
        // applies to all unknown cards in hand and tableau
        void permuteUnknowns();

        // returns number of cards in all foundations
        uint8_t getScore();
        
    private:
        Deck deck;
        Foundation foundations[NUM_SUITS];
        Hand hand;
        LocationTable loc_table;
        PushPull push_pull;
        Tableau tableaus[NUM_TABLEAUS];
        uint8_t empty_tableau;
};