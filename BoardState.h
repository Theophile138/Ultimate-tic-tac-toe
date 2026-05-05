#ifndef BOARD_STATE_H
#define BOARD_STATE_H

#include "main.h"
#include <vector>

const int PLAYER_ME = 1;
const int PLAYER_OPP = 2;
const int TIE = 3;
const int EMPTY = 0;

class BoardState {
public:
    int board[9][9];
    int macroBoard[3][3];
    int nextMacroRow;
    int nextMacroCol;

    BoardState();

    int checkSmallBoardWinner(int macroRow, int macroCol) const;
    void updateMove(int row, int col, int player);
    std::vector<GameMove> getValidMoves() const;
    bool isTerminal() const;
    BoardState cloneAndApply(const GameMove& move, int player) const;
    int evaluate() const;
};

#endif // BOARD_STATE_H
