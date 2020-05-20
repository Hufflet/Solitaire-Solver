#include "../include/card.hpp"
#include <assert.h>
#include "../include/params.hpp"


// Initialization
Card::Card(){
    state = 0;
}

// Rank
uint8_t Card::getRank(){
    return state & 0b00001111;
}
void Card::setRank(uint8_t r){
    assert(validateRank(r));
    state &= 0b11110000;
    state |= r;
}
bool validateRank(uint8_t r){
    return r < NUM_RANKS;
}

// Suit
uint8_t Card::getSuit(){
    return (state & 0b00110000) >> 4;
}
void Card::setSuit(uint8_t s){
    assert(validateSuit(s));
    state &= 0b11001111;
    state |= s<<4;
}
bool validateSuit(uint8_t s){
    return s < NUM_SUITS;
}

// Value
uint8_t Card::getValue(){
    return state & 0b00111111;
}

// Known
bool Card::getKnown(){
    return state & 0b01000000;
}
void Card::setKnown(bool k){
    state &= 0b10111111;
    state |= k << 7;
}

// Blank
void Card::makeBlank(){
    state = 0;
}

// Child
Card Card::getChild(bool b){
    assert(validateHasChild(this));
    Card child;
    uint8_t suit = this->getSuit();
    suit ^= 0b10;
    suit &= b;
    child.setSuit(suit);
    child.setRank(this->getRank()-1);
}
bool validateHasChild(Card *card){
    return (*card).getRank();
}

// Parent
Card Card::getParent(bool b){
    assert(validateHasParent(this));
    Card parent;
    uint8_t suit = this->getSuit();
    suit ^= 0b10;
    suit &= b;
    parent.setSuit(suit);
    parent.setRank(this->getRank()+1);
}
bool validateHasParent(Card *card){
    // card only has parent if it is not a king
    return (*card).getRank() < NUM_RANKS;
}

// String
std::string RANK_CHARS ("0A123456789TJQK");
std::string SUIT_CHARS ("CSHD");
std::string Card::getString(){
    std::string str ("__");
    str[0] = RANK_CHARS[this->getRank()];
    str[1] = SUIT_CHARS[this->getSuit()];
    return str;
}