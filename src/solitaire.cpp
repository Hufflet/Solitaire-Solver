#include "../include/solitaire.hpp"


MCSimulator::MCSimulator(){
    // RootDeck should start good to go
    // deal some cards to hand
    for(uint8_t i=1; i<HAND_SIZE; i++){
        hand.placeCard(i, root.getNextUnknown());
    }
    // loc_t should be set
    // deal some cards to the layout
    for(uint8_t tab=0; tab<7; tab++){
        // loc that tableau
        layout[tab].setLoc(tab+1);
        // deal it some cards
        for(uint8_t i=0; i<tab+1; i++){
            layout[tab].addCard(root.getNextUnknown(), loc_t);
        }
    }
}

void MCSimulator::makeMove(Move m){
    switch (m.type) {
        case 0: // reset the hand
            hand.reset();
            break;
    
        case 1: // flip the hand
            hand.flip();
            break;
    
        case 2: // tableau to tableau
            layout[m.to].addFrom(layout[m.from],loc_t);
            break;
    
        case 3: // from tableau to foundation
            reserve[m.to].addCard(layout[m.from].popTop(), loc_t);
            break;
    
        case 4: // from foundation to tableau
            layout[m.to].addCard(reserve[m.from].popTop(), loc_t);
            break;
    
        case 5: // from hand to tableau
            layout[m.to].addCard(hand.popTop(), loc_t);
            break;
    
        case 6: // from hand to foundation
            reserve[m.to].addCard(hand.popTop(), loc_t);
    
        default:
            break;
    }
}

void MCSimulator::undoMove(Move m){
    switch (m.type) {
        case 0: // reset the hand
            hand.undoReset(m.hand_pos);
            break;
    
        case 1: // flip the hand
            hand.unFlip();
            break;
    
        case 2: // tableau to tableau
            layout[m.to].addFrom(layout[m.from],loc_t);
            break;
    
        case 3: // from tableau to foundation
            reserve[m.to].addCard(layout[m.from].popTop(), loc_t);
            break;
    
        case 4: // from foundation to tableau
            layout[m.to].addCard(reserve[m.from].popTop(), loc_t);
            break;
    
        case 5: // from hand to tableau
            layout[m.to].addCard(hand.popTop(), loc_t);
            break;
    
        case 6: // from hand to foundation
            reserve[m.to].addCard(hand.popTop(), loc_t);
    
        default:
            break;
    }
}
}
