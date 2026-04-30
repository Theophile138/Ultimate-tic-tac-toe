#include "TicTacToe.h"

TicTacToe::TicTacToe() {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            m_grid[i][j] = Symbol::NONE;
        }
    }
}

bool TicTacToe::play(int x, int y, Symbol s) {
    if (x < 0 || x >= 3 || y < 0 || y >= 3) {
        return false;
    }
    if (m_grid[x][y] != Symbol::NONE) {
        return false;
    }
    m_grid[x][y] = s;
    return true;
}

void TicTacToe::undo(int x, int y) {
    if (x >= 0 && x < 3 && y >= 0 && y < 3) {
        m_grid[x][y] = Symbol::NONE;
    }
}

Symbol TicTacToe::getSymbol(int x, int y) const {
    if (x < 0 || x >= 3 || y < 0 || y >= 3) {
        return Symbol::NONE;
    }
    return m_grid[x][y];
}

Symbol TicTacToe::checkWinner() const {
    // Check rows and columns
    for (int i = 0; i < 3; ++i) {
        if (m_grid[i][0] != Symbol::NONE && m_grid[i][0] == m_grid[i][1] && m_grid[i][0] == m_grid[i][2])
            return m_grid[i][0];
        if (m_grid[0][i] != Symbol::NONE && m_grid[0][i] == m_grid[1][i] && m_grid[0][i] == m_grid[2][i])
            return m_grid[0][i];
    }
    // Check diagonals
    if (m_grid[0][0] != Symbol::NONE && m_grid[0][0] == m_grid[1][1] && m_grid[0][0] == m_grid[2][2])
        return m_grid[0][0];
    if (m_grid[0][2] != Symbol::NONE && m_grid[0][2] == m_grid[1][1] && m_grid[0][2] == m_grid[2][0])
        return m_grid[0][2];

    return Symbol::NONE;
}

bool TicTacToe::isFull() const {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (m_grid[i][j] == Symbol::NONE) return false;
        }
    }
    return true;
}
