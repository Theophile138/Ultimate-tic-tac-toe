#include "BoardState.h"

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

    // Rows and cols
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
    // Diagonals
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
    // 1. Victoire immédiate (Fin de partie)
    for (int i = 0; i < 3; ++i) {
        if (macroBoard[i][0] != EMPTY && macroBoard[i][0] != TIE && 
            macroBoard[i][0] == macroBoard[i][1] && macroBoard[i][0] == macroBoard[i][2])
            return (macroBoard[i][0] == PLAYER_ME) ? 1000000 : -1000000;
        if (macroBoard[0][i] != EMPTY && macroBoard[0][i] != TIE &&
            macroBoard[0][i] == macroBoard[1][i] && macroBoard[0][i] == macroBoard[2][i])
            return (macroBoard[0][i] == PLAYER_ME) ? 1000000 : -1000000;
    }
    if (macroBoard[0][0] != EMPTY && macroBoard[0][0] != TIE &&
        macroBoard[0][0] == macroBoard[1][1] && macroBoard[0][0] == macroBoard[2][2])
        return (macroBoard[0][0] == PLAYER_ME) ? 1000000 : -1000000;
    if (macroBoard[2][0] != EMPTY && macroBoard[2][0] != TIE &&
        macroBoard[2][0] == macroBoard[1][1] && macroBoard[2][0] == macroBoard[0][2])
        return (macroBoard[2][0] == PLAYER_ME) ? 1000000 : -1000000;

    int score = 0;

    // 2. Valeur stratégique des petites grilles gagnées (MacroBoard)
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            int weight = 0;
            if (r == 1 && c == 1) weight = 250;      // Centre : Très stratégique
            else if (r % 2 == 0 && c % 2 == 0) weight = 150; // Coins : Importants
            else weight = 100;                       // Bords : Standard

            if (macroBoard[r][c] == PLAYER_ME) score += weight;
            else if (macroBoard[r][c] == PLAYER_OPP) score -= weight;
        }
    }

    // 3. Bonus pour les alignements sur la MacroBoard (2 sur 3)
    // Cela encourage l'IA à créer des menaces et à bloquer celles de l'adversaire
    auto checkLineBonus = [&](int aR, int aC, int bR, int bC, int cR, int cC) {
        int me = 0, opp = 0, empty = 0;
        int cells[3] = {macroBoard[aR][aC], macroBoard[bR][bC], macroBoard[cR][cC]};
        for(int val : cells) {
            if (val == PLAYER_ME) me++;
            else if (val == PLAYER_OPP) opp++;
            else if (val == EMPTY) empty++;
        }
        if (me == 2 && empty == 1) score += 50;   // Menace de gagner la partie
        if (opp == 2 && empty == 1) score -= 50;  // Menace de perdre la partie
    };

    for (int i = 0; i < 3; ++i) {
        checkLineBonus(i, 0, i, 1, i, 2); // Lignes
        checkLineBonus(0, i, 1, i, 2, i); // Colonnes
    }
    checkLineBonus(0, 0, 1, 1, 2, 2); // Diagonale 1
    checkLineBonus(0, 2, 1, 1, 2, 0); // Diagonale 2

    // 4. Bonus/Malus pour le "Free Move" (Open Board)
    // Envoyer l'adversaire sur une grille déjà finie lui permet de jouer n'importe où.
    // C'est souvent très dangereux stratégiquement.
    if (nextMacroRow == -1 && nextMacroCol == -1) {
        // Si c'est au tour de l'adversaire (PLAYER_OPP) de jouer et qu'il a un Free Move
        // Cela signifie que le dernier coup (PLAYER_ME) a créé cette situation.
        score -= 150; 
    }

    // 5. Valeur des pièces locales (Positionnement tactique dans les petites grilles)
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            if (board[r][c] == PLAYER_ME) {
                if (r%3 == 1 && c%3 == 1) score += 3;
                else if (r%3 != 1 && c%3 != 1) score += 1;
            } else if (board[r][c] == PLAYER_OPP) {
                if (r%3 == 1 && c%3 == 1) score -= 3;
                else if (r%3 != 1 && c%3 != 1) score -= 1;
            }
        }
    }

    return score;
}
