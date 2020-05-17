#include "../include/simulator.hpp"
#include <math.h>


// initializes with default game
Simulator::Simulator(){
    // reserve reasonable space for vectors
    moves.reserve(8);
    scores.reserve(8);
    stds.reserve(8);
}

// makes the given move - this cannot be undone
void Simulator::makeMove(Move m){
    // just pass this through to the game
    game.makeMove(m);
}

// finds possible moves and score distributions
// shuffles deck iter times for each move
void Simulator::simulate(uint32_t iter){
    // set the game to a clean state
    game.resetRestrictions();

    // find what moves are possible from where the game is now
    moves = game.getMoves();

    // set up vector for tracking scores
    std::vector< std::vector<uint32_t> > all_scores;

    // figure out how many times to repeat the algorithm
    bool permute = true;
    uint32_t repeats = 1;
    uint8_t num_unknowns = game.numUnknownCards();
    // factorial could be VERY large, so only do as much as needed
    if(num_unknowns > 1){
        uint32_t num_permutes = 1;
        for(uint32_t i = uint32_t(num_unknowns); i>1; --i){
            num_permutes *= i;
            if(num_permutes >= iter){
                repeats = iter;
                permute = false;
                break;
            }
            else{
                repeats = num_permutes;
            }
        }
    }

    // loop over the algorithm
    for(uint32_t i = repeats; i>0; --i){
        // shuffle the cards
        if(permute){
            game.permuteUnknowns();
        }
        else{
            game.shuffleUnknowns();
        }

        // loop through the possible moves
        for(uint32_t j = moves.size(); j>0; --j){
            // make the move
            game.makeMove(moves[j-1]);

            // find moves that can then occur
            std::vector<Move> move_list = game.getMoves();

            // track score
            uint8_t max_score = NUM_CARDS_IN_DECK;
            uint8_t best_score = 0;

            // perform tree search
            while(move_list.size()){
                if(move_list.back().getActive()){
                    game.undoMove(move_list.back());
                    move_list.pop_back();
                }
                else{
                    if(best_score == max_score){
                        move_list.pop_back();
                    }
                    else{
                        game.makeMove(move_list.back());
                        move_list.back().setActive(true);
                        best_score = game.getScore();
                        std::vector<Move> m = game.getMoves();
                        move_list.reserve(move_list.size() + m.size());
                        move_list.insert(move_list.end(), m.begin(), m.end());
                    }
                }
            }

            // undo the move
            game.undoMove(moves[j-1]);

            // store the best_score with others
            all_scores[i-1].push_back(best_score);
        }
    }

    // find the mean best score for each move
    scores = std::vector<float> (moves.size(), 0);
    for(uint32_t i = moves.size(); i>0; --i){
        for(uint32_t j = repeats; j>0; --j){
            scores[i] += float(all_scores[i][j]);
        }
        scores[i] /= float(repeats);
    }

    // find the variance for each move
    vars = std::vector<float> (moves.size(), 0);
    for(uint32_t i = moves.size(); i>0; --i){
        for(uint32_t j = repeats; j>0; --j){
            vars[i] += pow(float(all_scores[i][j]) - scores[i], 2);
        }
        vars[i] /= float(repeats);
    }
}

// get the vector of moves after calling simulate
std::vector <Move> Simulator::getMoves(){
    // just return copy of stored moves
    return moves;
}

// get the vector of average scores after calling simulate
std::vector<float> Simulator::getMeanScores(){
    return scores;
}

// get the vector of variances after calling simulate
std::vector<float> Simulator::getVariances(){
    return vars;
}

// place a known card at the given location/position
void Simulator::placeKnown(Card c, uint8_t loc, uint8_t pos){
    game.placeKnown(c, loc, pos);
}




