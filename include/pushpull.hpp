#pragma once

#include <stdint.h>
#include "params.hpp"

class PushPull{
    /*
    The PushPull class keeps track of which tableaus can push or pull to find
    possible moves. The intent is to prevent infinite loops and maximize
    pruning of the move tree. Except for drawing/resetting, each moves takes
    a card(s) from one location (push) to another location (pull).
    
    Logic:
        push:   if tableau, set push to true, pull to false
                if foundation, set push to true, pull to false
        pull:   if tableau, set pull to false, keep push as is
                if foundation, set pull to true, push to false
    
    The game begins with all tableaus being able to pull. If a card is revealed
    in the tableau, set push and pull to true. If a card moves into the tableau
    from hand or foundation, set pull to true and push to false.  
    */
    public:
        // initialized with all tableau pulls true, rest false
        PushPull();

        // modifies push and pull arrays according to given logic
        void update(uint8_t push_loc, uint8_t pull_loc);

        // returns if given location can push
        bool canPush(uint8_t loc);

        // returns if given location can pull
        bool canPull(uint8_t loc);
    
    private:
        // array indicating if locations can push
        bool push[NUM_LOCATIONS];

        // array indicating if locations can pull
        bool pull[NUM_LOCATIONS];
};