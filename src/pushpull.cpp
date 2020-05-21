#include "../include/pushpull.hpp"
#include <assert.h>


// initialized with all push/pull to false/true
PushPull::PushPull(){
    for(uint8_t i=0; i<NUM_LOCATIONS; ++i){
        push[i] = false;
        pull[i] = true;
    }
}

// Push: indicates if location can push its top card to other location
bool PushPull::getPush(uint8_t loc){
    return push[loc-1];
}
void PushPull::setPush(uint8_t loc, bool b){
    push[loc-1] = b;
}
void PushPull::setAllPush(bool b){
    for(uint8_t i=0; i<NUM_LOCATIONS; ++i){
        push[i] = b;
    }
}
bool PushPull::anyPushTrue(){
    for(uint8_t i=0; i<NUM_LOCATIONS; ++i){
        if(push[i]){
            return true;
        }
    }
    return false;
}

// Pull: indicates if location can pull card(s) from other location
bool PushPull::getPull(uint8_t loc){
    return pull[loc-1];
}
void PushPull::setPull(uint8_t loc, bool b){
    pull[loc-1] = b;
}
void PushPull::setAllPull(bool b){
    for(uint8_t i=0; i<NUM_LOCATIONS; ++i){
        pull[i] = b;
    }
}
bool PushPull::anyPullTrue(){
    for(uint8_t i=0; i<NUM_LOCATIONS; ++i){
        if(pull[i]){
            return true;
        }
    }
    return false;
}

// Update: given push/pull locations, modify according to above logic
void PushPull::update(uint8_t push_loc, uint8_t pull_loc){
    push[push_loc-1] = push_loc > NUM_TABLEAUS;
    pull[push_loc-1] = false;
    push[pull_loc-1] = false;
    pull[pull_loc-1] = true;
}

// State: 32-bit number storing encoded push/pull data
uint32_t PushPull::getState(){
    uint32_t state = 0;
    for(uint8_t i=0; i<NUM_LOCATIONS; ++i){
        state = (state | push[i]) << 1;
    }
    for(uint8_t i=0; i<NUM_LOCATIONS; ++i){
        state = (state | pull[i]) << 1;
    }
    return state;
}
void PushPull::setState(uint32_t state){
    for(uint8_t i=NUM_LOCATIONS-1; i>=0; --i){
        pull[i] = (state >> 1) & 1;
    }
    for(uint8_t i=NUM_LOCATIONS-1; i>=0; --i){
        push[i] = (state >> 1) & 1;
    }
}