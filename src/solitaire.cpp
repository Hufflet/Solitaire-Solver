#include "../include/solitaire.hpp"


MCSimulator::MCSimulator(){
    // RootDeck should start good to go
    // deal some cards to hand
    for(uint8_t i=1; i<HAND_SIZE; i++){
        hand.placeCard(i, root.getNextUnknown());
    }
    // loc_t should be set
    // tab_lead_t should be set
    // deal some cards to the layout
    for(uint8_t tab=0; tab<7; tab++){
        // loc that tableau
        layout[tab].setLoc(tab);
        // deal it some cards
        for(uint8_t i=0; i<tab+1; i++){
            layout[tab].addCard(root.getNextUnknown(), loc_t);
        }
    }
    // loc the foundations
    for(uint8_t i=0; i<4; i++){
        reserve[i].setLoc(i+7);
    }

    // lots of random things to assign
    max_num_resets = 3;
    draw_size = 3;

    // loop_t is good
    kings_set = 0;
    tabs_free = 0;
    num_kings_set = 0;
    num_tabs_free = 0;

    // take care of reserveVals
    for(uint8_t suit = 0; suit<4; suit++){
        for(uint8_t rank=1; rank<2; rank++){
            uint8_t value = (suit<<4) + rank;
            reserveVals[value] = 2;
        }
    }
}

void MCSimulator::makeMove(Move m){
    switch (m.type) {
        case MOVE_TYPE_RESET: // reset the hand
            hand.reset();
            break;
    
        case MOVE_TYPE_FLIP: // flip the hand
            hand.flip(m.num);
            break;
    
        case MOVE_TYPE_LAYOUT_TO_LAYOUT: // tableau to tableau
            layout[m.to].addFrom(layout[m.from], m.num, loc_t, tab_lead_t);
            break;
    
        case MOVE_TYPE_LAYOUT_TO_RESERVE: // from tableau to foundation
            reserve[m.to].addCard(layout[m.from].popTop(tab_lead_t), loc_t);
            break;
    
        case MOVE_TYPE_RESERVE_TO_LAYOUT: // from foundation to tableau
            layout[m.to].addCard(reserve[m.from].popTop(), loc_t, tab_lead_t);
            break;
    
        case MOVE_TYPE_HAND_TO_LAYOUT: // from hand to tableau
            layout[m.to].addCard(hand.takeTop(), loc_t, tab_lead_t);
            break;
    
        case MOVE_TYPE_HAND_TO_RESERVE: // from hand to foundation
            reserve[m.to].addCard(hand.takeTop(), loc_t);
    
        default:
            break;
    }
}

void MCSimulator::undoMove(Move* m){
    switch ((*m).type) {
        case 0: // reset the hand
            hand.unReset();
            break;
    
        case 1: // flip the hand
            hand.unFlip((*m).num);
            break;
    
        case 2: // tableau to tableau
            layout[(*m).from].addFrom(layout[(*m).to], (*m).num, loc_t);
            break;
    
        case 3: // from tableau to foundation
            layout[(*m).from].addCard(reserve[(*m).to].popTop(), loc_t);
            break;
    
        case 4: // from foundation to tableau
            reserve[(*m).from].addCard(layout[(*m).to].popTop(), loc_t);
            break;
    
        case 5: // from hand to tableau
            hand.unTakeTop(layout[(*m).from].popTop(), (*m).num);
            break;
    
        case 6: // from hand to foundation
            hand.unTakeTop(reserve[(*m).to].popTop(), (*m).num);
            break;
            
        default:
            break;
    }
}

void MCSimulator::revealCard(Card c, uint8_t loc){
    layout[loc].popTop();
    layout[loc].addCard(c, loc_t);
    root.makeKnown(c);
}

void MCSimulator::revealDraw(Card c1, Card c2=Card(), Card c3=Card()){
    hand.revealDraw(c1, c2, c3, loc_t);
    root.makeKnown(c1);
    if(c2.getValue()){
        root.makeKnown(c2);
        if(c3.getValue()){
            root.makeKnown(c3);
        }
    }
}

uint8_t MCSimulator::rateMove(Move m){
    // make the move
    // find all possible next moves
    // rate each of those
    // end by undoing move
}

std::vector<Move> MCSimulator::getPossibleMoves(){
    std::vector<Move> move_list;        // list of general moves do explore
    std::vector<Move> must_make_move;   // will be one definite best move
    
    Move m;             // generic template move to modify
    Card to_card;       // card that move goes to
    Card from_card;     // card that move comes from
    uint8_t to_loc;     // 1-indexed tab that to card is in
    uint8_t from_loc;   // 1-indexed tab that from card is in

    // Prioritize searching for best moves first since they
    // shorten the time spent searching

// ----- Check for hand -> reserve ----- //
    // set all relevant parts of m
    // do not need m.num since always moving 1
    m.type = MOVE_TYPE_HAND_TO_RESERVE;
    m.to   = hand.readTop().getSuit();
    m.from = hand.getPos();

    // look at the card in the hand
    from_card = hand.readTop();

    // look at the card in the corresponding reserve
    to_card = reserve[from_card.getSuit()].readTop();
    
    // see if the top reserve card is what we need
    if(from_card.getValue()-to_card.getValue() == 1){

        // this move is ideal if both child cards are in reserve
        if(reserveVals[to_card.getValue()] == 2){
            must_make_move.push_back(m);
            return must_make_move;
        }

        // not ideal, just possible
        move_list.push_back(m);        
    }
// ----- End hand -> reserve ----- //

// ----- Check layout -> reserve ----- //
    // set parts of m that we know already
    // will need to update m.to and m.from
    // m.num does not matter since only moving 1
    m.type = MOVE_TYPE_LAYOUT_TO_RESERVE;

    // iterate through the tableaus
    for(uint8_t tab=0; tab<7; tab++){
        
        // get the top card of the tableau
        from_card = layout[tab].readTop();

        // get the top card of the relevant reserve
        to_card = reserve[to_card.getSuit()].readTop();
        
        // see if it is the one we are looking for
        if(from_card.getValue()-to_card.getValue() == 1){

            // we have a move! update the m parameters
            m.to = to_card.getSuit();
            m.from = tab;
            
            // this move is ideal if both child cards are in reserve
            if(reserveVals[from_card.getValue()] == 2){

                // this is ideal! do it
                must_make_move.push_back(m);
                return must_make_move;
            }

            // not ideal but still an option
            move_list.push_back(m);
        }
    }
// ----- End layout -> reserve ----- //

// ----- Check layout -> layout ----- //
    // we know m.type
    // need to figure out m.to, m.from, m.num
    m.type = MOVE_TYPE_LAYOUT_TO_LAYOUT;
    
    // first, are there any kings that have not been set?
    if(num_kings_set < 4){

        // are there open places for these kings?
        if(num_tabs_free){

            // are any of these kings in the layout?
            for(uint8_t i=0; i<4; i++){

                // create the next king
                from_card.setState(0, i, 13);

                // get its 1-indexed location
                from_loc = loc_t.find(from_card);

                // see if the king is available
                if(from_loc){

                    // there is a move here! define m params
                    m.from = from_loc - 1;
                    for(uint8_t tab=0; tab<4; tab++){
                        if((1<<tab) & tabs_free){
                            m.to = tab;
                            break;
                        }
                    }
                    m.num = 13 - layout[m.from].readTop().getRank() + 1;

                    // this is ideal if enough tabs are open
                    if(num_tabs_free >= (4-num_kings_set)){

                        // ideal move found: do it
                        must_make_move.push_back(m);
                        return;
                    }

                    // not ideal but could do it
                    move_list.push_back(m);
                }
            }
        }
    }

    // done with kings, what about the rest?
    for(uint8_t tab=0l tab<7; tab++){

        // what card do we have here?
        to_card = layout[tab].readTop();

        // skip it if it is empty
        if(!to_card.getValue()){
            continue;
        }

        // figure out what the child cards could be
        for(uint8_t i=0; i<2; i++){

            // need to adjust the suit
            uint8_t flip_bits = ~(2*i + 1);

            // get the child
            from_card.setState(0, to_card.getSuit()&flip_bits, to_card.getRank()-1);

            // skip if we are not clear to put from_card on to_card
            if(!loop_t.isClearAonB(from_card, to_card)){
                continue;
            }

            // figure out where child is
            from_loc = loc_t.find(from_card);

            // if available, we have a move!
            if((from_loc) && (from_loc <= 7)){
                
                // fill out the move parameters
                m.to = loc_t.find(to_card) - 1;
                m.from = from_loc - 1;
                m.n = to_card.getRank() - layout[m.from].readTop().getRank();

                // no chance of ideal here
                move_list.push_back(m);
            }
        }
    }
// ----- End layout -> layout ----- //

// ----- Check reserve -> layout ----- //
    // we know m.type
    // need to get m.to and m.from
    // do not need m.num since it will always be 1
    m.type = MOVE_TYPE_RESERVE_TO_LAYOUT;

    // iterate through the reserve
    for(uint8_t res=0; res<4; res++){

        // see what card is there
        from_card = reserve[res].readTop();

        // skip if no point in moving it down
        if(reserveVals[from_card.getValue()] == 2){
            continue;
        }

        // is it a king
        if(from_card.getRank() == 13){
        
            // can it go anywhere?
            if(num_tabs_free){

                // possible move! but def not ideal
                m.from = res;
                for(uint8_t i=0; i<7; i++){
                    if((1<<i) & tabs_free){
                        m.to = i;
                        break;
                    }
                }
                move_list.push_back(m);
            }

            // no need to keep going. Skip!
            continue;
        }

        // not a king? where to go...
        for(uint8_t i=0; i<2; i++){

            // need to adjust the suit each time
            uint8_t flip_bits = ~(2*i - 1);

            // need to get the parent
            to_card.setState(0, from_card.getSuit()&flip_bits, from_card.getRank()+1);

            // skip if we are not clear to put from_card on to_card
            if(!loop_t.isClearAonB(from_card, to_card)){
                continue;
            }

            // locate the parent
            to_loc = loc_t.find(to_card);

            // see if parent is in layout
            if((to_loc) && (to_loc <= 7)){

                // see if the parent is the first card
                if(to_card.getValue() == layout[to_loc-1].readTop().getValue()){

                    // great! got a move. not ideal but a move
                    m.to = to_loc-1;
                    m.from = res;
                    move_list.push_back(m);
                }
            }
        }
    }
// ----- End reserve -> layout ----- //

// ----- Check hand -> layout ----- //
    // we know m.from and m.type
    // still need to get m.to
    // m.num will not be used
    m.from = hand.getPos();
    m.type = MOVE_TYPE_HAND_TO_LAYOUT;

    // what's in the hand?
    from_card = hand.readTop();

    // make sure hand is not empty...
    if(from_card.getValue()){

        // ah yes, kings again...
        if(from_card.getValue == 13){

            // anywhere to stick it?
            if(num_tabs_free){

                // great. where?
                for(uint8_t i=0; i<7; i++){
                    if((1<<i) & tabs_free){
                        m.to = i;
                    }
                }

                // ideal if enough open for all kings
                if(num_tabs_free >= (4-num_kings_set)){

                    // ideal if doesn't screw up deck order
                    if((draw_size==1) || (!hand.canReset())){

                        // great! ideal!
                        must_make_move.push_back(m);
                        return must_make_move;
                    }
                }

                // not ideal but workable
                move_list.push_back(m);
            }
        }

        // finally, no more kings
        else{

            // so, who are your parents?
            for(uint8_t i=0; i<2; i++){

                // suit flipper bits
                uint8_t flip_bits = ~(2*i -1);

                // get parent
                to_card.setState(0, from_card.getSuit()&flip_bits, from_card.getRank()+1);

                // see if parent leads a tab
                to_loc = lead_t.find(to_card);

                // if so, great! got a move. not ideal but works
                m.to = to_loc - 1;
                move_list.push_back(m);
            }

        }
    }
// ----- End hand -> layout ----- //

// ----- Check reset ----- //
    // only need the type for this one
    m.type = MOVE_TYPE_RESET;

    // see if we can do it
    if(hand.canReset()){
        move_list.push_back(m);
    }
// ----- End reset ----- //

// ----- Check flip ----- //
    // know the type
    // need the num
    m.type = MOVE_TYPE_FLIP;

    // see if it can be done
    if(hand.canFlip()){
        m.num = draw_size;
        if(draw_size > hand.getNumLeft()){
            m.num = hand.getNumLeft();
        }
        move_list.push_back(m);
    }
// ----- End flip ----- //

    // all done searching, here are the moves found
    return move_list;
}