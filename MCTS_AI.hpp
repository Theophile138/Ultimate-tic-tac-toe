#ifndef MCTS_AI_HPP
#define MCTS_AI_HPP

#include "main.h"
#include <vector>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <random>

// Reuse the BoardState logic for MCTS
struct MCTSBoardState {
    int board[9][9];
    int macroBoard[3][3];
    int nextMacroRow, nextMacroCol;

    MCTSBoardState() {
        for (int r = 0; r < 9; ++r) for (int c = 0; c < 9; ++c) board[r][c] = 0;
        for (int r = 0; r < 3; ++r) for (int c = 0; c < 3; ++c) macroBoard[r][c] = 0;
        nextMacroRow = -1; nextMacroCol = -1;
    }

    void updateMove(int row, int col, int player) {
        board[row][col] = player;
        int mr = row / 3, mc = col / 3;
        if (macroBoard[mr][mc] == 0) macroBoard[mr][mc] = checkSmallBoardWinner(mr, mc);
        int tr = row % 3, tc = col % 3;
        if (macroBoard[tr][tc] != 0) { nextMacroRow = -1; nextMacroCol = -1; }
        else { nextMacroRow = tr; nextMacroCol = tc; }
    }

    int checkSmallBoardWinner(int mr, int mc) const {
        int rS = mr * 3, cS = mc * 3;
        int lines[8][3][2] = {{{0,0},{0,1},{0,2}},{{1,0},{1,1},{1,2}},{{2,0},{2,1},{2,2}},
                              {{0,0},{1,0},{2,0}},{{0,1},{1,1},{2,1}},{{0,2},{1,2},{2,2}},
                              {{0,0},{1,1},{2,2}},{{0,2},{1,1},{2,0}}};
        for (auto &l : lines) {
            int p = board[rS+l[0][0]][cS+l[0][1]];
            if (p != 0 && p != 3 && p == board[rS+l[1][0]][cS+l[1][1]] && p == board[rS+l[2][0]][cS+l[2][1]]) return p;
        }
        for (int r = 0; r < 3; r++) for (int c = 0; c < 3; c++) if (board[rS+r][cS+c] == 0) return 0;
        return 3; // Tie
    }

    std::vector<GameMove> getValidMoves() const {
        std::vector<GameMove> moves;
        if (nextMacroRow != -1) {
            int rS = nextMacroRow * 3, cS = nextMacroCol * 3;
            for (int r = 0; r < 3; r++) for (int c = 0; c < 3; c++) if (board[rS+r][cS+c] == 0) moves.push_back({rS+r, cS+c});
        } else {
            for (int mr = 0; mr < 3; mr++) for (int mc = 0; mc < 3; mc++) {
                if (macroBoard[mr][mc] == 0) {
                    int rS = mr * 3, cS = mc * 3;
                    for (int r = 0; r < 3; r++) for (int c = 0; c < 3; c++) if (board[rS+r][cS+c] == 0) moves.push_back({rS+r, cS+c});
                }
            }
        }
        return moves;
    }

    int getWinner() const {
        int lines[8][3][2] = {{{0,0},{0,1},{0,2}},{{1,0},{1,1},{1,2}},{{2,0},{2,1},{2,2}},
                              {{0,0},{1,0},{2,0}},{{0,1},{1,1},{2,1}},{{0,2},{1,2},{2,2}},
                              {{0,0},{1,1},{2,2}},{{0,2},{1,1},{2,0}}};
        for (auto &l : lines) {
            int p = macroBoard[l[0][0]][l[0][1]];
            if (p != 0 && p != 3 && p == macroBoard[l[1][0]][l[1][1]] && p == macroBoard[l[2][0]][l[2][1]]) return p;
        }
        if (getValidMoves().empty()) {
            int p1 = 0, p2 = 0;
            for (int r = 0; r < 3; r++) for (int c = 0; c < 3; c++) {
                if (macroBoard[r][c] == 1) p1++; else if (macroBoard[r][c] == 2) p2++;
            }
            if (p1 > p2) return 1; if (p2 > p1) return 2; return 3;
        }
        return 0;
    }
};

class MCTSNode {
public:
    MCTSBoardState state;
    GameMove move;
    MCTSNode* parent;
    std::vector<MCTSNode*> children;
    double wins;
    int visits;
    std::vector<GameMove> untriedMoves;
    int playerWhoMoved;

    MCTSNode(MCTSBoardState s, GameMove m = {0,0}, MCTSNode* p = nullptr, int lastPlayer = 2) 
        : state(s), move(m), parent(p), wins(0), visits(0), playerWhoMoved(lastPlayer) {
        untriedMoves = state.getValidMoves();
    }

    ~MCTSNode() { for (auto child : children) delete child; }

    MCTSNode* selectChild() {
        MCTSNode* best = nullptr;
        double bestUCB = -1e9;
        for (auto child : children) {
            double ucb = (child->wins / child->visits) + 1.41 * sqrt(log(visits) / child->visits);
            if (ucb > bestUCB) { bestUCB = ucb; best = child; }
        }
        return best;
    }

    MCTSNode* expand() {
        GameMove m = untriedMoves.back();
        untriedMoves.pop_back();
        MCTSBoardState nextS = state;
        int nextPlayer = (playerWhoMoved == 1) ? 2 : 1;
        nextS.updateMove(m.row, m.col, nextPlayer);
        MCTSNode* child = new MCTSNode(nextS, m, this, nextPlayer);
        children.push_back(child);
        return child;
    }
};

class MCTS_AI {
    std::mt19937 rng;
public:
    MCTSBoardState currentS;
    MCTS_AI() : rng(std::random_device()()) {}

    void reset() { currentS = MCTSBoardState(); }
    void registerOpponentMove(int r, int c) { if (r != -1) currentS.updateMove(r, c, 2); }

    GameMove computeBestMove() {
        std::vector<GameMove> moves = currentS.getValidMoves();
        if (moves.empty()) return {0,0};
        if (moves.size() == 81) { currentS.updateMove(4, 4, 1); return {4, 4}; }

        MCTSNode* root = new MCTSNode(currentS);
        auto start = std::chrono::steady_clock::now();
        
        // Simulation loop for 450ms
        while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count() < 450) {
            MCTSNode* node = root;
            
            // 1. Selection
            while (node->untriedMoves.empty() && !node->children.empty()) {
                node = node->selectChild();
            }

            // 2. Expansion
            if (!node->untriedMoves.empty()) {
                node = node->expand();
            }

            // 3. Simulation (Playout)
            MCTSBoardState simS = node->state;
            int turn = (node->playerWhoMoved == 1) ? 2 : 1;
            while (simS.getWinner() == 0) {
                std::vector<GameMove> valid = simS.getValidMoves();
                std::uniform_int_distribution<> dist(0, valid.size() - 1);
                GameMove m = valid[dist(rng)];
                simS.updateMove(m.row, m.col, turn);
                turn = (turn == 1) ? 2 : 1;
            }

            // 4. Backpropagation
            int winner = simS.getWinner();
            while (node != nullptr) {
                node->visits++;
                if (winner == 1) node->wins += 1.0;
                else if (winner == 3) node->wins += 0.5;
                node = node->parent;
            }
        }

        MCTSNode* bestChild = nullptr;
        int maxVisits = -1;
        for (auto child : root->children) {
            if (child->visits > maxVisits) { maxVisits = child->visits; bestChild = child; }
        }

        GameMove bestM = bestChild->move;
        delete root;
        currentS.updateMove(bestM.row, bestM.col, 1);
        return bestM;
    }
};

#endif
