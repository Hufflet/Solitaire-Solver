#pragma once

#include "game.hpp"


class Simulator{
    /*
    The Simulator class searches the game tree to find the best move. The game
    tree is maximally pruned to speed up the search. The simulation process
    shuffles the unknown cards, finds the best possible outcome for each move,
    and then repeats a given number of times. This builds a statistical guess
    at which move is the best from the current position.
    The Simulator uses an iterative approach rather than recursive one to save
    on time and space.
    */
    public:
        // initializes with default game
        Simulator();

        // makes the given move - this cannot be undone
        void makeMove(Move);

        // finds possible moves and score distributions
        // shuffles deck iter times for each move
        void simulate(uint32_t iter);

        // get the vector of moves after calling simulate
        std::vector <Move> getMoves();

        // get the vector of average scores after calling simulate
        std::vector <float> getMeanScores();

        // get the vector of standard deviations after calling simulate
        std::vector <float> getStandardDeviations();

        // place a known card at the given location/position
        void placeKnown(Card, uint8_t loc, uint8_t pos);

    private:
        Game game;
        std::vector <Move> moves;
        std::vector <float> scores;
        std::vector <float> stds;
};