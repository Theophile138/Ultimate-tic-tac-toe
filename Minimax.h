#ifndef MINIMAX_H_INCLUDED
#define MINIMAX_H_INCLUDED

#include "TicTacToe.h"

struct Move {
    int x;
    int y;
};

class Minimax {
public:
    static Move findBestMove(TicTacToe& board, Symbol aiSymbol);

private:
    static int minimax(TicTacToe& board, int depth, bool isMax, Symbol aiSymbol);
    static int evaluate(const TicTacToe& board, Symbol aiSymbol);
};

#endif // MINIMAX_H_INCLUDED
