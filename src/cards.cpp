#include "../include/cards.hpp"
#include "../include/params.hpp"
#include "../include/randomize.hpp"


Card::Card(){
    state = 0;  // indicates blank/no card
}

uint8_t Card::getSuit(){
    return (state & 0b110000)>>4;
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
    last_card = cur + HAND_SIZE;
    num_resets = 0;
}

bool Hand::canFlip(){
    return cur < last_card;
}

bool Hand::canReset(){
    return (num_resets < MAX_NUM_RESETS) && (!canFlip());
}

uint8_t Hand::flip(uint8_t num_to_flip){
    uint8_t counter = 0;
    while(num_to_flip){
        cur++;
        if((*cur).getValue()){
            num_to_flip--;
            counter++;
        }
        if(cur == last_card){
            break;
        }
    }
    return counter;
}

void Hand::reset(){
    cur = cards;
    num_resets++;
}

void Hand::placeCard(uint8_t pos, Card c, LocTable loc_t){
    cards[pos] = c;
    loc_t.move(c, 0);
}

void Hand::replaceUnknowns(RootDeck rd, LocTable loc_t){
    for(Card* p = cards+1; p < last_card; p++){
        if((*p).getValue() && !(*p).isKnown()){
            *p = rd.getNextUnknown();
            loc_t.move(*p, 0);
        }
    }
}

Card Hand::readTop(){
    return *cur;
}

void Hand::unFlip(uint8_t num_to_unflip){
    while(num_to_unflip){
        cur--;
        if((*cur).getValue()){
            num_to_unflip--;
        }
    }
}

void Hand::unReset(){
    num_resets--;
    cur = last_card;
}

Card Hand::takeTop(){
    Card c = *cur;
    (*cur).clear();

    if(cur < last_card){
        while(!(*cur).getValue()){
            cur++;
        }
    }

    else{
        while(!(*cur).getValue()){
            cur--;
            if(cur==cards){
                break;
            }
        }
        last_card = cur;
    }

    return c;
}

void Hand::unTakeTop(Card c, uint8_t num){
    cur = cards + num;
    *cur = c;
}

uint8_t Hand::getPos(){
    return cur-cards;
}

void Hand::revealDraw(Card c1, Card c2, Card c3, LocTable loc_t){
    loc_t.move(c1, 0);
    *cur = c1;
    if(c2.getValue()){
        loc_t.move(c2, 0);
        unFlip(1);
        *cur = c2;
        if(c3.getValue()){
            loc_t.move(c3, 0);
            unFlip(1);
            *cur = c3;
            flip(1);
        }
        flip(1);
    }
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


uint8_t LocTable::find(Card c){
    return locs[c.getValue()];
}

void LocTable::move(Card c, uint8_t loc){
    locs[c.getValue()] = loc;
}


Tableau::Tableau(){
    first_open = cards;
}

void Tableau::setLoc(uint8_t i){
    loc = i;
}

bool Tableau::hasCards(){
    return first_open > cards;
}

void Tableau::addFrom(Tableau base, uint8_t num, LocTable loc_t){
    for(uint8_t i=num; i; i--){
        *(first_open+i-1) = base.popTop();
    }
    first_open += num;
}

void Tableau::replaceUnknowns(RootDeck rd, LocTable loc_t){
    if(hasCards()){
        for(Card* p = first_open-1; p>=cards; p--){
            if(!(*p).isKnown()){
                *p = rd.getNextUnknown();
                loc_t.move(*p, loc);
            }
        }
    }
}

void Tableau::addCard(Card c, LocTable loc_t){
    *first_open = c;
    first_open++;
    loc_t.move(c, loc);
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

void Foundation::setLoc(uint8_t loc){
    this->loc = loc;
}

bool Foundation::isFull(){
    return (first_open-cards)==NUM_RANKS;
}

void Foundation::addCard(Card c, LocTable loc_t){
    *first_open = c;
    first_open++;
    loc_t.move(c, loc);
}

Card Foundation::popTop(){
    Card c = *(first_open-1);
    first_open--;
    (*first_open).clear();
    return c;
}