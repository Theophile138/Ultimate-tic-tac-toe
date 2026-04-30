#ifndef ULTIMATETICTACTOE_H_INCLUDED
#define ULTIMATETICTACTOE_H_INCLUDED

#include "TicTacToe.h"

class UltimateTicTacToe {
public:
    UltimateTicTacToe();
    TicTacToe& get(int x, int y);

private:
    TicTacToe m_subGames[3][3];
};

#endif
