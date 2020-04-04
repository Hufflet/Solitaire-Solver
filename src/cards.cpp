#include "../include/cards.hpp"
#include "../include/params.hpp"
#include "../include/randomize.hpp"


Card::Card(){
    state = 0;  // indicates blank/no card
}

void Card::setState(bool known, uint8_t suit, uint8_t rank){
    state = (known<<6) + (suit<<4) + rank;
}

uint8_t Card::getValue(){
    return state & VALUE_BITS;
}

uint8_t Card::getRank(){
    return state & RANK_BITS;
}

void Card::clear(){
    state = 0;
}

void Card::makeKnown(){
    state |= KNOWN_BITS;
}

bool Card::isKnown(){
    return state & KNOWN_BITS;
}


Hand::Hand(){
    cur = cards;
    num_discard = 0;
    num_remain = 0;
    num_resets = 0;
}

bool Hand::canFlip(){
    return num_remain;
}

#ifdef CAN_RESET
    bool Hand::canReset(){
        return (num_resets < MAX_NUM_RESETS) && (!canFlip());
    }
#endif

void Hand::flip(){
    uint8_t counter = 0;
    while(DRAW_SIZE-counter){
        cur++;
        if((*cur).getValue()){
            counter++;
            num_remain--;
            num_discard++;
        }
        if(!num_remain){
            break;
        }
    }
}

void Hand::reset(){
    cur = cards;
    num_remain += num_discard;
    num_discard = 0;
    num_resets += 1;
}

void Hand::placeCard(uint8_t pos, Card c){
    cards[pos] = c;
    if(pos>(cur-cards)){
        num_remain++;
    }
    else{
        num_discard++;
    }
}

void Hand::replaceUnknowns(RootDeck rd){
    for(Card* p = cards; p < cards+HAND_SIZE; p++){
        *p = rd.getNextUnknown();
    }
}

Card Hand::get(){
    return *cur;
}


RootDeck::RootDeck(){
    Card c;
    for(uint8_t suit = 0; suit<NUM_SUITS; suit++){
        for(uint8_t rank = 1; rank < NUM_RANKS+1; rank++){
            c.setState(false, suit, rank);
            cards[suit*NUM_RANKS+rank-1] = c;
        }
    }
    first_unknown = cards;
    cur_unknown = first_unknown;
}

void RootDeck::makeKnown(Card c){
    Card* p = first_unknown;
    while((*p).getValue() != c.getValue()){
        p++;
    }
    (*p).makeKnown();
    // swap this with first unknown
    Card temp = *first_unknown;
    *first_unknown = *p;
    *p = temp;
    first_unknown++;
}

void RootDeck::shuffleUnknown(){
    uint8_t num_unknown = NUM_CARDS_IN_DECK - (first_unknown-cards);
    for(uint8_t i = 0; i < num_unknown-1; i++){
        uint32_t R = rand32();
        uint64_t N = (uint64_t(R) * uint64_t(num_unknown-i)) >> 32;
        Card temp = *(first_unknown+i);
        *(first_unknown+i) = *(first_unknown+i+N);
        *(first_unknown+i+N) = temp;
    }
}

Card RootDeck::getNextUnknown(){
    Card c = *cur_unknown;
    cur_unknown++;
    if(cur_unknown-cards >= NUM_CARDS_IN_DECK){
        cur_unknown = first_unknown;
    }
    return c;
}


LocTable::LocTable(){
    for(uint8_t* i=locs; i<locs+LOC_TABLE_SIZE; i++){
        *i = 0;
    }
}

uint8_t LocTable::find(Card c){
    return locs[c.getValue()];
}

void LocTable::move(Card c, uint8_t loc){
    locs[c.getValue()] = loc;
}


Tableau::Tableau(){
    first_faceup = cards;
    first_open = cards;
}

void Tableau::setLoc(uint8_t i){
    loc = i;
}

bool Tableau::hasCards(){
    return first_open > cards;
}

void Tableau::addFrom(Tableau base, LocTable loc_t){
    Card c = base.popTop();
    // if there is already a card here...
    uint8_t num_to_move = (*(first_open-1)).getRank()-c.getRank();
    // if no card here...
    if(!hasCards()) num_to_move = 14 - c.getRank();
    
    Card* i = first_faceup + num_to_move - 1;
    *i = c;
    for(uint8_t N=1; N<num_to_move; N++){
        c = base.popTop();
        *(i-N) = c;
    }
    
    first_open = i+1;
}

void Tableau::replaceUnknowns(RootDeck rd){
    for(Card* p = cards; p<cards+20; p++){
        *p = rd.getNextUnknown();
    }
}

void Tableau::addCard(Card c, LocTable loc_t){
    loc_t.move(c, loc);
    *first_open = c;
    first_open++;
}

Card Tableau::popTop(){
    Card c = *(first_open-1);
    first_open--;
    (*first_open).clear();
    return c;
}


Foundation::Foundation(){
    first_open = cards;
}

bool Foundation::isFull(){
    return (first_open-cards)==NUM_RANKS;
}

void Foundation::addCard(Card c, LocTable loc_t){
    loc_t.move(c, loc);
    *first_open = c;
    first_open++;
}

Card Foundation::popTop(){
    Card c = *(first_open-1);
    first_open--;
    (*first_open).clear();
    return c;
}