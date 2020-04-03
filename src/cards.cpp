#include "../include/cards.hpp"


Card::Card(){
    value = 0;
}

void Card::setValue(uint8_t suit, uint8_t number){
    value = (suit<<4) + number;
}

uint8_t Card::getValue(){
    return value & 0b111111;
}

