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
        push:   if tableau, set push/pull to false/false
                if foundation, set push/pull to true/false
        pull:   if tableau, set push/pull to false/true
                if foundation, set push/pull to false/true
    
    The game begins with all tableaus and foundations being able to pull. If a
    card is revealed in the tableau, set push/pull to true/true. If a card
    moves into the tableau, set push/pull to false/true. Drawing or resetting
    sets all push/pull to false/false.
    */
    public:
        // initialized with all push/pull to false/true
        PushPull();

        // Push: indicates if location can push its top card to other location
        bool getPush(uint8_t loc);
        void setPush(uint8_t loc, bool);
        void setAllPush(bool);
        bool anyPushTrue();

        // Pull: indicates if location can pull card(s) from other location
        bool getPull(uint8_t loc);
        void setPull(uint8_t loc, bool);
        void setAllPull(bool);
        bool anyPullTrue();

        // Update: given push/pull locations, modify according to above logic
        void update(uint8_t push_loc, uint8_t pull_loc);

        // State: 32-bit number storing encoded push/pull data
        uint32_t getState();
        void setState(uint32_t);
    
    private:
        bool push[NUM_LOCATIONS];
        bool pull[NUM_LOCATIONS];
};