#ifndef AI_H
#define AI_H

#include "BoardState.h"

const int DEPTH_NORMAL = 6; 
const int DEPTH_OPEN   = 5; 

class AI {
public:
    BoardState currentState;

    void reset();
    void registerOpponentMove(int row, int col);
    GameMove computeBestMove();

private:
    int minimax(const BoardState& state, int depth, int alpha, int beta, bool isMaximizing);
};

#endif // AI_H
