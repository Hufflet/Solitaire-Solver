#include "../include/cards.hpp"
#include "../include/params.hpp"
#include "../include/randomize.hpp"


Card::Card(){
    state = 0;  // indicates no card
}

uint8_t Card::getValue(){
    return state & 0b00111111; // last 6 bits give value
}

uint8_t Card::getRank(){
    return state & 0b00001111; // last 4 bits give rank
}

uint8_t Card::getSuit(){
    return (state & 0b110000)>>4; // 5th and 6th bit give suit
}

void Card::setState(bool known, uint8_t suit, uint8_t rank){
    state = (known<<6) + (suit<<4) + rank;
}

void Card::makeKnown(){
    state |= 0b01000000; // 7th bit indicates if it is known
}

bool Card::isKnown(){
    return state & 0b01000000;
}

void Card::clear(){
    state = 0; // reset the state
}



RootDeck::RootDeck(){
    Card c;
    for(uint8_t suit = 0; suit<NUM_SUITS; ++suit){
        for(uint8_t rank = 1; rank < NUM_RANKS+1; ++rank){
            c.setState(false, suit, rank);
            cards[suit*NUM_RANKS+rank-1] = c;
        }
    }
    first_unknown = cards;
    cur_unknown = first_unknown;
}

void RootDeck::makeKnown(Card c){
    Card *p = first_unknown;
    while((*p).getValue() != c.getValue()){
        ++p;
    }
    (*p).makeKnown();
    // swap this with first unknown
    swap(p, first_unknown);
    // move up first_unknown
    ++first_unknown;
}

void RootDeck::shuffleUnknown(){
    uint8_t num_unknown = NUM_CARDS_IN_DECK - (first_unknown-cards);
    if(num_unknown){
        shuffleArray(first_unknown, num_unknown);
    }
}

Card RootDeck::getNextUnknown(){
    Card c = *cur_unknown;
    ++cur_unknown;
    if(cur_unknown-cards >= NUM_CARDS_IN_DECK){
        cur_unknown = first_unknown;
    }
    return c;
}



Hand::Hand(uint8_t num_resets, uint8_t num_flip){
    // set all cards in hand to unknown
    for(uint8_t i=1; i<=HAND_SIZE+1; ++i){
        cards[i].setState(false, uint8_t(0), uint8_t(1111));
    }
    // start out pointing at blank first card
    cur = cards;
    last_card = cur + HAND_SIZE;
    resets_remaining = num_resets;
    flip_size = num_flip;
}

uint8_t Hand::howManyCanFlip(){
    // if you are at the end of the hand, you cannot flip any
    if(cur == last_card){
        return 0;
    }

    // count how many cards from cur to last_card
    uint8_t count = 0;
    for(Card *i = cur; i<=last_card; ++i){
        
        // increase count when card is not blank
        if((*i).getValue()){
            ++count;
            
            // if there are more than we need to flip, call it quits early
            if(count >= flip_size){
                return flip_size;
            }
        }
    }

    // if here, count is neither 0 nor flip_size, but something inbetween
    return count;
}

void Hand::flip(uint8_t num_to_flip){
    uint8_t count = num_to_flip;
    while(count){
        ++cur;
        if((*cur).getValue()){
            --count;
        }
    }
}

void Hand::unFlip(uint8_t num_to_unflip){
    uint8_t count = num_to_unflip;
    while(count){
        --cur;
        if((*cur).getValue()){
            --count;
        }
    }
}

bool Hand::canReset(){
    return resets_remaining && (cur == last_card);
}

void Hand::reset(){
    cur = cards;
    --resets_remaining;
}

void Hand::unReset(){
    ++resets_remaining;
    cur = last_card;
}

Card Hand::readTop(){
    return *cur;
}

Card Hand::takeTop(){
    // copy the value of the top card
    Card c = *cur;

    // set the top card to blank
    (*cur).clear();

    // now move cur to previous card
    for(Card *i=cur; i>=cards; --i){
        --cur;
        if((*cur).getValue()){
            break;
        }
    }

    // return the card originally at cur
    return c;
}

void Hand::unTakeTop(Card c, uint8_t pos){
    // set cur to right spot
    cur = cards + pos;

    // put the card there
    *cur = c;

    // update if this is the last card
    if(cur > last_card){
        last_card = cur;
    }
}

uint8_t Hand::getPos(){
    return cur-cards;
}

void Hand::placeCard(uint8_t pos, Card c, LocTable loc_t){
    cards[pos] = c;
    loc_t.move(c, 0);
}

void Hand::replaceUnknowns(RootDeck rd, LocTable loc_t){
    // go through all cards in the hand
    for(Card* p = cards+1; p <= last_card; ++p){
        if((*p).getValue() && !(*p).isKnown()){
            *p = rd.getNextUnknown();
            loc_t.move(*p, 0);
        }
    }
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

void Tableau::addCard(Card c, LocTable loc_t){
    *first_open = c;
    ++first_open;
    loc_t.move(c, loc);
}

void Tableau::addFrom(Tableau base, uint8_t num, LocTable loc_t){
    for(uint8_t i=num; i>0; --i){
        *(first_open+i-1) = base.takeTop();
    }
    first_open += num;
}

Card Tableau::takeTop(){
    Card c = *(first_open-1);
    --first_open;
    (*first_open).clear();
    return c;
}

void Tableau::replaceUnknowns(RootDeck rd, LocTable loc_t){
    for(Card *i = first_open-1; i>=cards; --i){
        if(!(*i).isKnown()){
            *i = rd.getNextUnknown();
            loc_t.move(*i, loc);
        }
    }
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
    ++first_open;
    loc_t.move(c, loc);
}

Card Foundation::readTop(){
    return *(first_open-1);
}

Card Foundation::takeTop(){
    Card c = *(first_open-1);
    --first_open;
    (*first_open).clear();
    return c;
}



void LoopTracker::markAonB(Card a, Card b){
    
}