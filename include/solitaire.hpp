#include "cards.hpp"
#include <vector>


class Move{
    public:
        uint8_t to;
        uint8_t from;
        uint8_t num;
        uint8_t type;num_
};


class MCSimulator{
    public:
        MCSimulator();
        void makeMove(Move);
        void revealCard(Card, uint8_t loc);
        void revealDraw(Card c1, Card c2=Card(), Card c3=Card());
        Move getBestMove();
    
    private:
        void shuffleDealUnknowns();
        void undoMove(Move);
        uint8_t rateMove(Move);
        std::vector<Move> getPossibleMoves();
        RootDeck root;
        Hand hand;
        LocTable loc_t;
        LocTable tab_lead_t;
        Tableau layout[7];
        Foundation reserve[4];
        uint8_t max_num_resets;
        uint8_t draw_size;
        uint8_t reserveVals[64] = {0};
        LoopTracker loop_t;
        uint8_t kings_set;
        uint8_t tabs_free;
        uint8_t num_kings_set;
        uint8_t num_tabs_free;
};


class ManualAssistant{
    public:
        ManualAssistant();
        void enterStartingCards();
        void showSimMovesScores();
        void selectMove();
    private:
        MCSimulator sim;
};