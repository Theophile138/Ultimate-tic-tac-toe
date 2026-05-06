#include "BoardState.h"
#include <algorithm>

BoardState::BoardState() {
    for (int r = 0; r < 9; ++r)
        for (int c = 0; c < 9; ++c)
            board[r][c] = EMPTY;
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c)
            macroBoard[r][c] = EMPTY;
    nextMacroRow = -1;
    nextMacroCol = -1;
}

int BoardState::checkSmallBoardWinner(int macroRow, int macroCol) const {
    int startRow = macroRow * 3;
    int startCol = macroCol * 3;

    for (int i = 0; i < 3; ++i) {
        if (board[startRow + i][startCol] != EMPTY &&
            board[startRow + i][startCol] == board[startRow + i][startCol + 1] &&
            board[startRow + i][startCol] == board[startRow + i][startCol + 2]) {
            return board[startRow + i][startCol];
        }
        if (board[startRow][startCol + i] != EMPTY &&
            board[startRow][startCol + i] == board[startRow + 1][startCol + i] &&
            board[startRow][startCol + i] == board[startRow + 2][startCol + i]) {
            return board[startRow][startCol + i];
        }
    }
    if (board[startRow][startCol] != EMPTY &&
        board[startRow][startCol] == board[startRow + 1][startCol + 1] &&
        board[startRow][startCol] == board[startRow + 2][startCol + 2]) {
        return board[startRow][startCol];
    }
    if (board[startRow + 2][startCol] != EMPTY &&
        board[startRow + 2][startCol] == board[startRow + 1][startCol + 1] &&
        board[startRow + 2][startCol] == board[startRow][startCol + 2]) {
        return board[startRow + 2][startCol];
    }

    bool isFull = true;
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            if (board[startRow + r][startCol + c] == EMPTY) {
                isFull = false;
                break;
            }
        }
    }
    if (isFull) return TIE;

    return EMPTY;
}

void BoardState::updateMove(int row, int col, int player) {
    board[row][col] = player;
    int macroRow = row / 3;
    int macroCol = col / 3;
    
    if (macroBoard[macroRow][macroCol] == EMPTY) {
        macroBoard[macroRow][macroCol] = checkSmallBoardWinner(macroRow, macroCol);
    }

    int targetMacroRow = row % 3;
    int targetMacroCol = col % 3;

    if (macroBoard[targetMacroRow][targetMacroCol] != EMPTY) {
        nextMacroRow = -1;
        nextMacroCol = -1;
    } else {
        nextMacroRow = targetMacroRow;
        nextMacroCol = targetMacroCol;
    }
}

std::vector<GameMove> BoardState::getValidMoves() const {
    std::vector<GameMove> moves;
    
    if (nextMacroRow != -1 && nextMacroCol != -1) {
        int rStart = nextMacroRow * 3;
        int cStart = nextMacroCol * 3;
        for (int r = 0; r < 3; ++r) {
            for (int c = 0; c < 3; ++c) {
                if (board[rStart + r][cStart + c] == EMPTY) {
                    moves.push_back({rStart + r, cStart + c});
                }
            }
        }
    } else {
        for (int mr = 0; mr < 3; ++mr) {
            for (int mc = 0; mc < 3; ++mc) {
                if (macroBoard[mr][mc] == EMPTY) {
                    int rStart = mr * 3;
                    int cStart = mc * 3;
                    for (int r = 0; r < 3; ++r) {
                        for (int c = 0; c < 3; ++c) {
                            if (board[rStart + r][cStart + c] == EMPTY) {
                                moves.push_back({rStart + r, cStart + c});
                            }
                        }
                    }
                }
            }
        }
    }
    return moves;
}

bool BoardState::isTerminal() const {
    for (int i = 0; i < 3; ++i) {
        if (macroBoard[i][0] != EMPTY && macroBoard[i][0] != TIE &&
            macroBoard[i][0] == macroBoard[i][1] && macroBoard[i][0] == macroBoard[i][2])
            return true;
        if (macroBoard[0][i] != EMPTY && macroBoard[0][i] != TIE &&
            macroBoard[0][i] == macroBoard[1][i] && macroBoard[0][i] == macroBoard[2][i])
            return true;
    }
    if (macroBoard[0][0] != EMPTY && macroBoard[0][0] != TIE &&
        macroBoard[0][0] == macroBoard[1][1] && macroBoard[0][0] == macroBoard[2][2])
        return true;
    if (macroBoard[2][0] != EMPTY && macroBoard[2][0] != TIE &&
        macroBoard[2][0] == macroBoard[1][1] && macroBoard[2][0] == macroBoard[0][2])
        return true;
        
    if (getValidMoves().empty()) return true;

    return false;
}

BoardState BoardState::cloneAndApply(const GameMove& move, int player) const {
    BoardState nextState = *this;
    nextState.updateMove(move.row, move.col, player);
    return nextState;
}

int BoardState::evaluate() const {
    // 1. HELPERS DE PONDÉRATION
    auto getBoardWeight = [](int r, int c) -> double {
        if (r == 1 && c == 1) return 1.75;      // Centre global
        if (r % 2 == 0 && c % 2 == 0) return 1.4; // Coins globaux
        return 1.0;                             // Bords globaux
    };

    auto getCellWeight = [](int r, int c) -> double {
        if (r == 1 && c == 1) return 0.22;      // Centre local
        if (r != 1 && c != 1) return 0.20;      // Coins locaux
        return 0.17;                            // Bords locaux
    };

    // 2. VÉRIFICATION VICTOIRE GLOBALE (Priorité absolue)
    auto checkGlobalWinner = [&]() -> int {
        for (int i = 0; i < 3; ++i) {
            if (macroBoard[i][0] != EMPTY && macroBoard[i][0] != TIE && 
                macroBoard[i][0] == macroBoard[i][1] && macroBoard[i][0] == macroBoard[i][2])
                return macroBoard[i][0];
            if (macroBoard[0][i] != EMPTY && macroBoard[0][i] != TIE &&
                macroBoard[0][i] == macroBoard[1][i] && macroBoard[0][i] == macroBoard[2][i])
                return macroBoard[0][i];
        }
        if (macroBoard[0][0] != EMPTY && macroBoard[0][0] != TIE &&
            macroBoard[0][0] == macroBoard[1][1] && macroBoard[0][0] == macroBoard[2][2])
            return macroBoard[0][0];
        if (macroBoard[2][0] != EMPTY && macroBoard[2][0] != TIE &&
            macroBoard[2][0] == macroBoard[1][1] && macroBoard[2][0] == macroBoard[0][2])
            return macroBoard[2][0];
        return EMPTY;
    };

    int globalWinner = checkGlobalWinner();
    if (globalWinner == PLAYER_ME) return 1000000;
    if (globalWinner == PLAYER_OPP) return -1000000;

    double totalScore = 0.0;

    // 3. ÉVALUATION D'UNE PETITE GRILLE
    auto evaluateSmallGrid = [&](int mRow, int mCol) -> double {
        int startR = mRow * 3;
        int startC = mCol * 3;
        double gridScore = 0.0;

        if (macroBoard[mRow][mCol] != EMPTY) return 0.0;

        for (int r = 0; r < 3; ++r) {
            for (int c = 0; c < 3; ++c) {
                int val = board[startR + r][startC + c];
                double w = getCellWeight(r, c) * 45.0;
                if (val == PLAYER_ME) gridScore += w;
                else if (val == PLAYER_OPP) gridScore -= w;
            }
        }

        auto checkLocalLine = [&](int r1, int c1, int r2, int c2, int r3, int c3) {
            int cells[3] = {board[startR+r1][startC+c1], board[startR+r2][startC+c2], board[startR+r3][startC+c3]};
            int me = 0, opp = 0, empty = 0;
            for(int v : cells) { if(v == PLAYER_ME) me++; else if(v == PLAYER_OPP) opp++; else empty++; }
            
            if (me == 2 && empty == 1) gridScore += 45.0;
            if (opp == 2 && empty == 1) gridScore -= 90.0;
        };

        for (int i = 0; i < 3; ++i) {
            checkLocalLine(i, 0, i, 1, i, 2);
            checkLocalLine(0, i, 1, i, 2, i);
        }
        checkLocalLine(0, 0, 1, 1, 2, 2);
        checkLocalLine(0, 2, 1, 1, 2, 0);

        return gridScore;
    };

    // 4. ÉVALUATION DE LA MACROBOARD
    auto evaluateMacroBoard = [&]() -> double {
        double macroScore = 0.0;
        for (int r = 0; r < 3; ++r) {
            for (int c = 0; c < 3; ++c) {
                double w = getBoardWeight(r, c);
                if (macroBoard[r][c] == PLAYER_ME) macroScore += 225.0 * w;
                else if (macroBoard[r][c] == PLAYER_OPP) macroScore -= 225.0 * w;
            }
        }

        auto checkMacroLine = [&](int r1, int c1, int r2, int c2, int r3, int c3) {
            int cells[3] = {macroBoard[r1][c1], macroBoard[r2][c2], macroBoard[r3][c3]};
            int me = 0, opp = 0, empty = 0;
            for(int v : cells) { if(v == PLAYER_ME) me++; else if(v == PLAYER_OPP) opp++; else if(v == EMPTY) empty++; }
            
            if (me == 2 && empty == 1) macroScore += 150.0;
            if (opp == 2 && empty == 1) macroScore -= 150.0;
            if (me == 1 && empty == 2) macroScore += 30.0;
            if (opp == 1 && empty == 2) macroScore -= 30.0;
        };

        for (int i = 0; i < 3; ++i) {
            checkMacroLine(i, 0, i, 1, i, 2);
            checkMacroLine(0, i, 1, i, 2, i);
        }
        checkMacroLine(0, 0, 1, 1, 2, 2);
        checkMacroLine(0, 2, 1, 1, 2, 0);

        return macroScore;
    };

    totalScore += evaluateMacroBoard();

    for (int mr = 0; mr < 3; ++mr) {
        for (int mc = 0; mc < 3; ++mc) {
            if (macroBoard[mr][mc] == EMPTY) {
                totalScore += evaluateSmallGrid(mr, mc) * 1.5 * getBoardWeight(mr, mc);
            }
        }
    }

    if (nextMacroRow == -1 && nextMacroCol == -1) {
        totalScore -= 150.0;
    }

    return static_cast<int>(totalScore);
}
