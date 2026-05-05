#ifndef AI_H
#define AI_H

#include "BoardState.h"

// --- RÉGLAGES DE L'IA ---
const int DEPTH_NORMAL = 6; // Profondeur quand on joue dans une petite grille
const int DEPTH_OPEN   = 5; // Profondeur quand on peut jouer n'importe où (plus de 9 choix)

class AI {
public:
    BoardState currentState;

    void reset();
    void registerOpponentMove(int row, int col);
    GameMove computeBestMove();

private:
    int minimax(const BoardState& state, int depth, int alpha, int beta, bool isMaximizing);
};

#endif // AI_H
