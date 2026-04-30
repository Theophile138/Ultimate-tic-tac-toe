#include "Minimax.h"
#include <algorithm>
#include <climits>

Move Minimax::findBestMove(TicTacToe& board, Symbol aiSymbol) {
    int bestVal = INT_MIN;
    Move bestMove = {-1, -1};

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board.getSymbol(i, j) == Symbol::NONE) {
                board.play(i, j, aiSymbol);
                int moveVal = minimax(board, 0, false, aiSymbol);
                board.undo(i, j);

                if (moveVal > bestVal) {
                    bestMove = {i, j};
                    bestVal = moveVal;
                }
            }
        }
    }
    return bestMove;
}

int Minimax::minimax(TicTacToe& board, int depth, bool isMax, Symbol aiSymbol) {
    int score = evaluate(board, aiSymbol);

    if (score == 10) return score - depth;
    if (score == -10) return score + depth;
    if (board.isFull()) return 0;

    Symbol opponent = (aiSymbol == Symbol::CROSS) ? Symbol::CIRCLE : Symbol::CROSS;

    if (isMax) {
        int best = INT_MIN;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (board.getSymbol(i, j) == Symbol::NONE) {
                    board.play(i, j, aiSymbol);
                    best = std::max(best, minimax(board, depth + 1, !isMax, aiSymbol));
                    board.undo(i, j);
                }
            }
        }
        return best;
    } else {
        int best = INT_MAX;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (board.getSymbol(i, j) == Symbol::NONE) {
                    board.play(i, j, opponent);
                    best = std::min(best, minimax(board, depth + 1, !isMax, aiSymbol));
                    board.undo(i, j);
                }
            }
        }
        return best;
    }
}

int Minimax::evaluate(const TicTacToe& board, Symbol aiSymbol) {
    Symbol winner = board.checkWinner();
    if (winner == aiSymbol) return 10;
    if (winner != Symbol::NONE) return -10; // L'autre a gagné
    return 0;
}
