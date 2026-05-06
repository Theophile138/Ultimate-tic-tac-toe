#include "AI.h"
#include <algorithm>
#include <iostream>

void AI::reset() {
    currentState = BoardState();
}

void AI::registerOpponentMove(int row, int col) {
    if (row != -1 && col != -1) {
        currentState.updateMove(row, col, PLAYER_OPP);
    }
}

GameMove AI::computeBestMove() {
    std::vector<GameMove> validMoves = currentState.getValidMoves();
    if (validMoves.empty()) {
        return {0, 0}; 
    }
    
    if (validMoves.size() == 81) {
        currentState.updateMove(4, 4, PLAYER_ME);
        return {4, 4};
    }

    GameMove bestMove = validMoves[0];
    int bestScore = -2000000;
    
    int depth = (validMoves.size() > 9) ? DEPTH_OPEN : DEPTH_NORMAL; 

    for (const auto& move : validMoves) {
        BoardState nextState = currentState.cloneAndApply(move, PLAYER_ME);
        int score = minimax(nextState, depth - 1, -2000000, 2000000, false);
        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }
    
    currentState.updateMove(bestMove.row, bestMove.col, PLAYER_ME);
    return bestMove;
}

int AI::minimax(const BoardState& state, int depth, int alpha, int beta, bool isMaximizing) {
    if (depth == 0 || state.isTerminal()) {
        return state.evaluate();
    }
    
    std::vector<GameMove> validMoves = state.getValidMoves();
    if (validMoves.empty()) return state.evaluate();

    if (isMaximizing) {
        int maxEval = -2000000;
        for (const auto& move : validMoves) {
            BoardState nextState = state.cloneAndApply(move, PLAYER_ME);
            int eval = minimax(nextState, depth - 1, alpha, beta, false);
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha) break; 
        }
        return maxEval;
    } else {
        int minEval = 2000000;
        for (const auto& move : validMoves) {
            BoardState nextState = state.cloneAndApply(move, PLAYER_OPP);
            int eval = minimax(nextState, depth - 1, alpha, beta, true);
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha) break; 
        }
        return minEval;
    }
}
