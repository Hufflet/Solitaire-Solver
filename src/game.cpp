#include "../include/game.hpp"


// initialized as blank game, all cards are mock
Game::Game(){
    // deck initializes with all mock cards, good to go
    
    // foundations initialize empty, but need locations
    for(uint8_t i = 0; i<4; ++i){
        foundations[i].setLocation(i+8);
    }

    // hand initializes with all mock cards, good to go

    // loc_table initializes with all locations unknown, good to go

    // push_pull initializes with only tableau pulls true, good to go

    // tableaus need locations and mock cards
    for(uint8_t i = 0; i<7; ++i){
        tableaus[i].setLocation(i+1);
        tableaus[i].fillMock();
    }
}


// returns number of unknown cards
uint8_t Game::numUnknown(){
    return deck.numUnknown();
}


// sets parameters so that all legal moves from this state are found
void Game::resetRestrictions(){
    push_pull.reset();
}


// finds moves from this state
std::vector<Move> Game::getMoves(){
    // general procedure:
    // - look at all possible pushes and pulls
    // - find the children or parents
    // - if available, record the move
    // - add the move to a set of possible moves

    // template move
    Move move;
    move.setActive(false);
    move.setPushPull(push_pull.getState());
    
    // vector for storing all moves
    std::vector<Move> moves;

    // can the hand flip?
    uint8_t num_can_flip = hand.howManyCanFlip();
    if(num_can_flip){
        move.setNumCards(num_can_flip);
        move.setType(MOVE_TYPE_DRAW);
        moves.push_back(move);
    }
    
    // otherwise, can the hand reset?
    else if(hand.canReset()){
        move.setType(MOVE_TYPE_RESET);
        moves.push_back(move);
    }

    // can the top hand card be played?
    Card top = hand.readTop();
    if((top.getRank()) && (top.getRank()!=KING_RANK)){

        // loop through the parent cards
        for(uint8_t i=0; i<2; ++i){
            Card parent = top.getParent(i);
            uint8_t parent_loc = loc_table.find(parent);

            // skip if parent_loc unknown or in hand
            if(!parent_loc){
                continue;
            }

            // if parent_loc is tableau, add move if parent is in position 1
            if(parent_loc < 8){
                Tableau parent_tab = tableaus[parent_loc-1];
                uint8_t parent_pos = parent_tab.getPosition(parent);
                if(parent_pos == 1){
                    move.setPull(parent_tab.getLocation());
                    move.setType(MOVE_TYPE_H2T);
                    moves.push_back(move);
                }
            }
        }

        // check the appropriate foundation
        Foundation found = foundations[top.getSuit()];
        if(found.readTop().getRank()+1 == top.getRank()){
            move.setPull(found.getLocation());
            move.setType(MOVE_TYPE_H2F);
        }
    }
    else if(top.getRank()==KING_RANK){

        // could play king in rightmost empty tableau
        if(empty_tableau){
            move.setPull(empty_tableau);
            move.setType(MOVE_TYPE_H2T);
            moves.push_back(move);
        }

        // check the appropriate foundation
        Foundation found = foundations[top.getSuit()];
        if(found.readTop().getRank()+1 == top.getRank()){
            move.setPull(found.getLocation());
            move.setType(MOVE_TYPE_H2F);
        }
    }

    // if no cards can be pushed or pulled, then search is complete
    if(!push_pull.anyPushOrPull()){
        return moves;
    }

    // loop over tableau for push and pull
    for(uint8_t i=0; i<NUM_TABLEAUS; ++i){
        Tableau tab = tableaus[i];

        // check for pushing
        if(push_pull.canPush(tab.getLocation())){
            
            // pushing only happens for new cards
            // only need to consider top card
            top = tab.readTop();

            // if king, move to empty_tableau
            if(top.getRank() == KING_RANK){
                if(empty_tableau){
                    move.setNumCards(1);
                    move.setPull(empty_tableau);
                    move.setPush(tab.getLocation());
                    move.setRevealed(tab.wouldReveal(1));
                    move.setType(MOVE_TYPE_T2T);
                    moves.push_back(move);
                }
            }

            // othwise, loop over the parents
            else for(uint8_t j=0; j<2; ++j){
                Card parent = top.getParent(j);
                uint8_t parent_loc = loc_table.find(parent);

                // skip if parent is not in tableau
                if((parent_loc==0) || (parent_loc>7)){
                    continue;
                }

                // only do move if parent is in position 1
                Tableau parent_tab = tableaus[parent_loc-1];
                if(parent_tab.getPosition(parent) == 1){
                    move.setNumCards(1);
                    move.setPull(parent_tab.getLocation());
                    move.setPush(tab.getLocation());
                    move.setRevealed(tab.wouldReveal(1)));
                    move.setType(MOVE_TYPE_T2T);
                    moves.push_back(move);
                }
            }

            // check the appropriate foundation
            Foundation found = foundations[top.getSuit()];
            if(found.readTop().getRank()+1 == top.getRank()){
                move.setPull(found.getLocation());
                move.setPush(tab.getLocation());
                move.setRevealed(tab.wouldReveal(1));
                move.setType(MOVE_TYPE_T2F);
                moves.push_back(move);
            }
        }
    
        // check for pulling
        if(push_pull.canPull(tab.getLocation())){
            top = tab.readTop();

            // loop over children
            for(uint8_t j=0; j<2; ++j){
                Card child = top.getChild(j);
                uint8_t child_loc = loc_table.find(child);

                // skip if child in hand or unknown
                if(!child_loc){
                    continue;
                }

                // if child in tableau, is a possible move
                if(child_loc < 8){

                    // skip if child can push
                    if(push_pull.canPush(child_loc)){
                        continue;
                    }

                    Tableau child_tab = tableaus[child_loc-1];
                    uint8_t child_pos = child_tab.getPosition(child));
                    move.setNumCards(child_pos);
                    move.setPull(tab.getLocation());
                    move.setPush(child_loc);
                    move.setRevealed(child_tab.wouldReveal(child_pos));
                    move.setType(MOVE_TYPE_T2T);
                }

                // if child in foundation, must be in position 1
                else{
                    Foundation child_found = foundations[top.getSuit()];
                    uint8_t child_pos = child_found.getPosition(child);
                    if(child_pos == 1){
                        move.setPull(child_found.getLocation());
                        move.setPush(tab.getLocation());
                        move.setType(MOVE_TYPE_T2F);
                    }
                }
            }
        }
    }

    // loop over foundations for push and pull
    for(uint8_t i=0; i<NUM_SUITS; ++i){
        Foundation found = foundations[i];
        top = found.readTop();

        // check if push
        if(push_pull.canPush(found.getLocation())){

            // if king, play to empty_tableau
            if(top.getRank() == KING_RANK){
                if(empty_tableau){
                    // do not repeat if empty_tableau can pull
                    if(!push_pull.canPull(empty_tableau)){
                        move.setPull(empty_tableau);
                        move.setType(MOVE_TYPE_F2T);
                        moves.push_back(move);
                    }
                }
            }

            // otherwise, loop over parents
            else for(uint8_t j=0; j<2; ++j){
                Card parent = top.getParent(j);
                uint8_t parent_loc = loc_table.find(parent);

                // skip if not in tableau
                if((parent_loc==0) || (parent_loc > 7)){
                    continue;
                }

                // skip if parent could pull
                if(push_pull.canPull(parent_loc)){
                    continue;
                }

                // parent must be in position 1
                Tableau parent_tab = tableaus[parent_loc-1];
                uint8_t parent_pos = parent_tab.getPosition(parent);
                if(parent_pos == 1){
                    move.setPull(parent_loc);
                    move.setPush(found.getLocation());
                    move.setType(MOVE_TYPE_F2T);
                    moves.push_back(move);
                }
            }
        }
    
        // check if pull
        if(push_pull.canPull(found.getLocation())){

            // skip if king
            if(top.getRank() == KING_RANK){
                continue;
            }

            // find the only possible child
            Card child = top;
            child.setRank(top.getRank()+1);
            uint8_t child_loc = loc_table.find(child);

            // skip if child_loc is unknown or in hand
            if(!child_loc){
                continue;
            }

            // child must be in position 1
            Tableau child_tab = tableaus[child_loc-1];
            if(child_tab.getPosition(child) == 1){
                
                // do not repeat if child_tab could push
                if(!push_pull.canPush(child_loc)){
                    move.setPull(found.getLocation());
                    move.setPush(child_loc);
                    move.setRevealed(child_tab.wouldReveal(1));
                    move.setType(MOVE_TYPE_T2F);
                    moves.push_back(move);
                }
            }


        }
    }

    return moves;
}


// makes the move, updates state and restrictions accordingly
// assumes the move is possible from this state
void Game::makeMove(Move move){
    uint8_t move_type = move.getType();
    switch(move_type){

        case MOVE_TYPE_DRAW:
            // flip over the given number of cards
            hand.flip(move.getNumCards());

            // set all pushes/pulls to false
            push_pull.setAllPush(false);
            push_pull.setAllPull(false);

            break;
        
        case MOVE_TYPE_RESET:
            // reset the hand
            hand.reset();

            // set all pushes/pulls to false
            push_pull.setAllPush(false);
            push_pull.setAllPull(false);

            break;

        case MOVE_TYPE_H2T:
            // take card out of hand
            Card card = hand.takeTop();

            // put the card on the the given tableau
            Tableau tab = tableaus[move.getPull()-1];
            tab.addCard(card, loc_table);

            // set tableau push/pull to false/true
            push_pull.setPush(tab.getLocation(), false);
            push_pull.setPull(tab.getLocation(), true);

            break;
        
        case MOVE_TYPE_H2F:
            // take card out of hand
            Card card = hand.takeTop();

            // put card on the given foundation
            Foundation found = foundations[move.getPull()-8];
            found.addCard(card, &loc_table);

            // set foundation push/pull to false/true
            push_pull.setPush(found.getLocation(), false);
            push_pull.setPull(found.getLocation(), true);

            break;
        
        case MOVE_TYPE_T2T:
            // find the tableaus
            Tableau push_tab = tableaus[move.getPush()-1];
            Tableau pull_tab = tableaus[move.getPull()-1];

            // move the given number of cards
            pull_tab.addFrom(push_tab, move.getNumCards(), &loc_table);

            // set push_tab push/pull to false/false
            push_pull.setPush(move.getPush(), false);
            push_pull.setPull(move.getPush(), false);

            // set pull_tab push/pull to false/true
            push_pull.setPush(move.getPull(), false);
            push_pull.setPull(move.getPull(), true);

            // if card revealed, update loc_table
            if(move.getRevealed()){
                Card rev_card = push_tab.readTop();
                loc_table.setLocation(rev_card, push_tab.getLocation());
            }

            break;
        
        case MOVE_TYPE_T2F:
            // take card from tableau
            Tableau tab = tableaus[move.getPush()-1];
            Card card = tab.takeTop();

            // put card on foundation
            Foundation found = foundations[move.getPull()-8];
            found.addCard(card, &loc_table);

            // set tableau push/pull to false/false
            push_pull.setPush(tab.getLocation(), false);
            push_pull.setPull(tab.getLocation(), false);

            // set foundation push/pull to false/true
            push_pull.setPush(found.getLocation(), false);
            push_pull.setPull(found.getLocation(), true);
            
            // if card revealed, update loc_table
            if(move.getRevealed()){
                Card rev_card = tab.readTop();
                loc_table.setLocation(rev_card, tab.getLocation());
            }

            break;

        case MOVE_TYPE_F2T:
            // take card from foundation
            Foundation found = foundations[move.getPush()-8];
            Card card = found.takeTop();

            // add card to tableau
            Tableau tab = tableaus[move.getPull()-1];
            tab.addCard(card, &loc_table);

            // set foundation push/pull to true/false
            push_pull.setPush(found.getLocation(), true);
            push_pull.setPull(found.getLocation(), false);

            // set tableau push/pull to false/true
            push_pull.setPush(tab.getLocation(), false);
            push_pull.setPull(tab.getLocation(), true);

            break;
        
        default:
            break;
    }
}


// undoes move, updates states and restrictions accordingly
// assumes undoing the move is possible from this state
void Game::undoMove(Move move){
    switch(move.getType()){
        case MOVE_TYPE_DRAW:
            hand.unflip(move.getNumCards());
            break;
        
        case MOVE_TYPE_RESET:
            hand.unreset();
            break;
        
        case MOVE_TYPE_H2T:
            Tableau tab = tableaus[move.getPull()-1];
            Card card = tab.takeTop();
            hand.untakeTopCard(card, move.getNumCards(), &loc_table);
            break;
        
        case MOVE_TYPE_H2F:
            Foundation found = foundations[move.getPull()-8];
            Card card = found.takeTop();
            hand.untakeTopCard(card, move.getNumCards(), &loc_table);
            break;
        
        case MOVE_TYPE_T2T:
            Tableau push_tab = tableaus[move.getPush()-1];
            Tableau pull_tab = tableaus[move.getPull()-1];
            
            // if card revealed, unupdate loc_table
            if(move.getRevealed()){
                Card rev_card = push_tab.readTop();
                loc_table.setLocation(rev_card, 0);
            }
            
            push_tab.addFrom(pull_tab, move.getNumCards(), &loc_table);
            break;

        case MOVE_TYPE_T2F:
            Foundation found = foundations[move.getPull()-8];
            Card card = found.takeTop();
            Tableau tab = tableaus[move.getPush()-1];
            
            // if card revealed, unupdate loc_table
            if(move.getRevealed()){
                Card rev_card = tab.readTop();
                loc_table.setLocation(rev_card, 0);
            }
            
            tab.addCard(card, &loc_table);
            break;
        
        case MOVE_TYPE_F2T:
            Tableau tab = tableaus[move.getPull()-1];
            Card card = tab.takeTop();
            Foundation found = foundations[move.getPush()-8];
            found.addCard(card, &loc_table);
            break;

        default:
            break;
    }

    push_pull.setState(move.getPushPull());
}


// places Card at given location/position
// updates state to account for known card
void Game::placeKnown(Card card, uint8_t loc, uint8_t pos){
    // case 0: hand
    if(loc == 0){
        hand.placeCard(card, pos, &loc_table);
    }

    // case 1: tableau
    else if(loc < 8){
        Tableau tab = tableaus[loc - 1];
        tab.placeCard(card, pos, &loc_table);
    }

    // case 2: foundation
    else{
        Foundation found = foundations[loc - 8];
        found.placeCard(card, pos, &loc_table);
    }
}


// shuffles the unknown cards
// applies to all unknown cards in hand and tableau
void Game::shuffleUnknowns(){
    // first shuffle the deck
    deck.shuffleUnknown();

    // now go through the hand and tableaus
    hand.replaceUnknowns(deck, &loc_table);
    for(uint8_t i=0; i<NUM_TABLEAUS; ++i){
        tableaus[i].replaceUnknowns(deck);
    }
}


// permutes the unknown cards
// applies to all unknown cards in hand and tableau
void Game::permuteUnknowns(){
    // first permute the deck
    deck.permuteUnknown();

    // now go through the hand and tableaus
    hand.replaceUnknowns(deck, &loc_table);
    for(uint8_t i=0; i<NUM_TABLEAUS; ++i){
        tableaus[i].replaceUnknowns(deck);
    }
}


// returns number of cards in all foundations
uint8_t Game::getScore(){
    uint8_t score = 0;
    for(uint8_t i=0; i<NUM_SUITS; ++i){
        score += foundations[i].numCards();
    }
    return score;
}