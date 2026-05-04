#ifndef MY_AI_HPP
#define MY_AI_HPP

#include "main.h"
#include <vector>
#include <algorithm>
#include <iostream>

const int PLAYER_ME = 1;
const int PLAYER_OPP = 2;
const int TIE = 3;
const int EMPTY = 0;

// --- RÉGLAGES DE L'IA ---
const int DEPTH_NORMAL = 6; // Profondeur quand on joue dans une petite grille
const int DEPTH_OPEN   = 5; // Profondeur quand on peut jouer n'importe où (plus de 9 choix)

struct BoardState {
    int board[9][9];
    int macroBoard[3][3];
    int nextMacroRow;
    int nextMacroCol;

    BoardState() {
        for (int r = 0; r < 9; ++r)
            for (int c = 0; c < 9; ++c)
                board[r][c] = EMPTY;
        for (int r = 0; r < 3; ++r)
            for (int c = 0; c < 3; ++c)
                macroBoard[r][c] = EMPTY;
        nextMacroRow = -1;
        nextMacroCol = -1;
    }

    int checkSmallBoardWinner(int macroRow, int macroCol) const {
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

    void updateMove(int row, int col, int player) {
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

    std::vector<GameMove> getValidMoves() const {
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

    bool isTerminal() const {
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

    BoardState cloneAndApply(const GameMove& move, int player) const {
        BoardState nextState = *this;
        nextState.updateMove(move.row, move.col, player);
        return nextState;
    }

    int evaluate() const {
        int score = 0;
        for (int r = 0; r < 3; ++r) {
            for (int c = 0; c < 3; ++c) {
                if (macroBoard[r][c] == PLAYER_ME) score += 100;
                else if (macroBoard[r][c] == PLAYER_OPP) score -= 100;
            }
        }
        
        for (int r = 0; r < 9; ++r) {
            for (int c = 0; c < 9; ++c) {
                if (board[r][c] == PLAYER_ME) {
                    if (r%3 == 1 && c%3 == 1) score += 3; // Center of small grid
                    else if (r%3 != 1 && c%3 != 1) score += 1; // Corner of small grid
                } else if (board[r][c] == PLAYER_OPP) {
                    if (r%3 == 1 && c%3 == 1) score -= 3;
                    else if (r%3 != 1 && c%3 != 1) score -= 1;
                }
            }
        }
        return score;
    }
};

class AI {
public:
    BoardState currentState;

    void reset() {
        currentState = BoardState();
    }

    void registerOpponentMove(int row, int col) {
        if (row != -1 && col != -1) {
            currentState.updateMove(row, col, PLAYER_OPP);
        }
    }

    GameMove computeBestMove() {
        std::vector<GameMove> validMoves = currentState.getValidMoves();
        if (validMoves.empty()) {
            return {0, 0}; 
        }
        
        // Prevent huge branching on turn 1
        if (validMoves.size() == 81) {
            currentState.updateMove(4, 4, PLAYER_ME);
            return {4, 4};
        }

        GameMove bestMove = validMoves[0];
        int bestScore = -1000000;
        
        // If there are many open moves (e.g., after being sent to an open board), limit depth
        int depth = (validMoves.size() > 9) ? DEPTH_OPEN : DEPTH_NORMAL; 

        for (const auto& move : validMoves) {
            BoardState nextState = currentState.cloneAndApply(move, PLAYER_ME);
            int score = minimax(nextState, depth - 1, -1000000, 1000000, false);
            if (score > bestScore) {
                bestScore = score;
                bestMove = move;
            }
        }
        
        currentState.updateMove(bestMove.row, bestMove.col, PLAYER_ME);
        return bestMove;
    }

private:
    int minimax(const BoardState& state, int depth, int alpha, int beta, bool isMaximizing) {
        if (depth == 0 || state.isTerminal()) {
            return state.evaluate();
        }
        
        std::vector<GameMove> validMoves = state.getValidMoves();
        if (validMoves.empty()) return state.evaluate();

        if (isMaximizing) {
            int maxEval = -1000000;
            for (const auto& move : validMoves) {
                BoardState nextState = state.cloneAndApply(move, PLAYER_ME);
                int eval = minimax(nextState, depth - 1, alpha, beta, false);
                maxEval = std::max(maxEval, eval);
                alpha = std::max(alpha, eval);
                if (beta <= alpha) break; 
            }
            return maxEval;
        } else {
            int minEval = 1000000;
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
};

#endif
