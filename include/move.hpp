#pragma once

#include <stdint.h>


class Move{
    /*
    The Move class describes all parts of a move necessary to both make and
    undo the move. There are several types of moves:
        MOVE_TYPE_DRAW:     flips cards over in hand
        MOVE_TYPE_RESET:    resets the hand
        MOVE_TYPE_H2T:      moves card from hand to tableau
        MOVE_TYPE_H2F:      moves card from hand to foundation
        MOVE_TYPE_T2T:      moves card from tableau to tableau
        MOVE_TYPE_T2F:      moves card from tableau to foundation
        MOVE_TYPE_F2T:      moves card from foundation to tableau
    Other data includes how many cards were moved, the state of PushPull before
    the move is made, etc.
    */
    public:
        // initialized as move with all zero values
        Move();

        // move type as defined in params
        uint8_t getType();
        void setType();

        //number of cards to move
        uint8_t getNumCards();
        void setNumCards();

        // PushPull state tells state before move is made
        uint32_t getPushPull();
        void setPushPull(uint32_t);

        // tells if a new card was revealed when these cards moved
        bool getRevealed();
        void setRevealed(bool);
    
    private:
        uint8_t type;
        uint8_t num;
        bool revealed;
        uint32_t pushpull;

}