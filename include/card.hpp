#pragma once

#include <stdint.h>
#include <string>

class Card{
    /*
    The Card class tracks the data associated with a card: number, suit, and if
    it is known. The class offers simple functions for getting and setting
    those values, as well as for finding the possible parents and children of
    a Card. 
    */
    public:
        // initialized as blank card
        Card();

        // Rank: the number associated with the card
        // 0 indicates a blank card
        // 1-13 indicates ace-king
        uint8_t getRank();
        void setRank(uint8_t);

        // Suit: the group the card belongs to
        // 0-3 indicate Clubs, Spades, Hearts, Diamonds
        uint8_t getSuit();
        void setSuit(uint8_t);

        // Value: combines suit and rank
        // fully describes nonmeta card information
        uint8_t getValue();
        
        // Known: whether or not the card is truly known
        // distinguishes simulated guess from given reality
        bool getKnown();
        void setKnown(bool);

        // resets the state to be blank
        void makeBlank();

        // Child: any card of one fewer rank and opposite color
        // select chooses which of the two possible children to return 
        Card getChild(bool select);

        // Parent: any card of one higher rank and opposite color
        // select chooses which of the two possible parents to return
        Card getParent(bool);

        // String: a two character representation of the card
        // indicates both suit and rank in printable fashion
        std::string getString();

    private:
        // state: an 8 bit array containing all necessary information
        // bits 0-3: the rank of the card
        // bits 4-5: the suit of the card
        // bit    6: if the card is known
        uint8_t state;
};