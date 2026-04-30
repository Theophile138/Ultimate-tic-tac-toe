#include "UltimateTicTacToe.h"

UltimateTicTacToe::UltimateTicTacToe() {
}

TicTacToe& UltimateTicTacToe::get(int x, int y) {
    if (x < 0) x = 0; if (x > 2) x = 2;
    if (y < 0) y = 0; if (y > 2) y = 2;
    return m_subGames[x][y];
}
