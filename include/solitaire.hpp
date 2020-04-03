#include "cards.hpp"
#include <vector>


class Move{
    public:
        uint8_t from;
        uint8_t to;
        bool caused_flip;
        bool is_active;
};


class MCSimulator{
    public:
        MCSimulator();
        void resetSimulation();
        void makeMove(Move);
        void revealCard(Card, uint8_t loc);
        void revealDraw(Card);
        void revealDraw(Card, Card);
        void revealDraw(Card, Card, Card);
        Move getBestMove();
    private:
        Hand hand;
        RootDeck root;
        LocTable loc_t;
        Tableau* layout[7];
        Foundation* reserve[4];
        void shuffleDealUnknowns();
        std::vector<Move> root_moves;
        std::vector<uint32_t> root_scores;
        std::vector<Move> move_tree;
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