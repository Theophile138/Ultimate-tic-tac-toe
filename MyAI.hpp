#ifndef MY_AI_HPP
#define MY_AI_HPP

#include "main.h"
#include <vector>
#include <chrono>
#include <algorithm>
#include <unordered_map>

// =============================================================================
// --- CONFIGURATION DE L'HEURISTIQUE ---
// Modifiez ces valeurs pour ajuster le comportement de l'IA
// =============================================================================
const int COEF_VICTOIRE     = 1000000; // Score pour un alignement de 3 grilles
const int COEF_GRILLE_GAGNEE = 5000;    // Score de base pour une grille gagnée
const int COEF_MENACE_MACRO  = 10000;    // Bonus si on a 2 grilles alignées (menace macro)
const int COEF_MENACE_MICRO  = 100;     // Bonus pour 2 pions alignés dans une petite grille
const int COEF_PION_SIMPLE   = 10;      // Score pour chaque pion placé
const int COEF_COUP_LIBRE    = -2000;   // Pénalité si on donne un coup libre à l'adversaire

// Poids stratégiques des positions (Centre > Coins > Bords)
const int POIDS_POSITIONS[9] = {
    3, 2, 3, 
    2, 4, 2, 
    3, 2, 3
};

// Temps de calcul maximum (en millisecondes)
const int TEMPS_MAX_MS = 380;
// =============================================================================

// Masques binaires pour les alignements gagnants (3x3)
const uint16_t MASQUES_VICTOIRE[8] = {
    0x007, 0x038, 0x1C0, // Lignes
    0x049, 0x092, 0x124, // Colonnes
    0x111, 0x054         // Diagonales
};

struct BoardState {
    uint16_t me[9], opp[9];
    uint16_t mMe, mOpp, mFull;
    int nextG;

    BoardState() : mMe(0), mOpp(0), mFull(0), nextG(-1) {
        for (int i = 0; i < 9; i++) me[i] = opp[i] = 0;
    }

    inline void applyMove(int r, int c, bool isMe) {
        int g = (r / 3) * 3 + (c / 3), b = (r % 3) * 3 + (c % 3);
        if (isMe) me[g] |= (1 << b); else opp[g] |= (1 << b);
        
        if (!((mMe | mOpp | mFull) & (1 << g))) {
            uint16_t bb = isMe ? me[g] : opp[g];
            for (auto m : MASQUES_VICTOIRE) if ((bb & m) == m) { 
                if (isMe) mMe |= (1 << g); else mOpp |= (1 << g); 
                goto end_apply; 
            }
            if ((me[g] | opp[g]) == 0x1FF) mFull |= (1 << g);
        }
    end_apply:
        nextG = ((mMe | mOpp | mFull) & (1 << b)) ? -1 : b;
    }

    int evaluate(int depth) const {
        // Vérification victoire macro
        for (auto m : MASQUES_VICTOIRE) {
            if ((mMe & m) == m) return COEF_VICTOIRE - depth;
            if ((mOpp & m) == m) return depth - COEF_VICTOIRE;
        }

        int score = 0;
        for (int i = 0; i < 9; i++) {
            if (mMe & (1 << i)) score += COEF_GRILLE_GAGNEE * POIDS_POSITIONS[i];
            else if (mOpp & (1 << i)) score -= COEF_GRILLE_GAGNEE * POIDS_POSITIONS[i];
            else if (!(mFull & (1 << i))) {
                // Analyse interne de la petite grille
                score += __builtin_popcount(me[i]) * COEF_PION_SIMPLE;
                score -= __builtin_popcount(opp[i]) * COEF_PION_SIMPLE;
                for (auto m : MASQUES_VICTOIRE) {
                    int cMe = __builtin_popcount(me[i] & m), cOpp = __builtin_popcount(opp[i] & m);
                    if (cMe == 2 && cOpp == 0) score += COEF_MENACE_MICRO;
                    if (cOpp == 2 && cMe == 0) score -= COEF_MENACE_MICRO;
                }
            }
        }
        
        // Menaces macro
        for (auto m : MASQUES_VICTOIRE) {
            int cMe = __builtin_popcount(mMe & m), cOpp = __builtin_popcount(mOpp & m), f = __builtin_popcount(~(mMe|mOpp|mFull) & m);
            if (cMe == 2 && f == 1) score += COEF_MENACE_MACRO;
            if (cOpp == 2 && f == 1) score -= COEF_MENACE_MACRO;
        }

        if (nextG == -1) score += COEF_COUP_LIBRE; // Malus pour avoir donné un coup libre

        return score;
    }
};

struct TTEntry { int depth, score, flag; };

class AI {
    std::chrono::time_point<std::chrono::steady_clock> start_time;
    bool stop_search;
    long long node_count;
    std::unordered_map<uint64_t, TTEntry> transposition_table;

    uint64_t getHash(const BoardState& s) {
        uint64_t h = s.mMe | ((uint64_t)s.mOpp << 9) | ((uint64_t)s.mFull << 18);
        for(int i=0; i<9; i++) h ^= ((uint64_t)s.me[i] << (27 + i)) ^ ((uint64_t)s.opp[i] << (36 + i));
        return h ^ ((uint64_t)(s.nextG + 1) << 60);
    }

public:
    BoardState current;
    void reset() { current = BoardState(); transposition_table.clear(); }
    void registerOpponentMove(int r, int c) { if (r != -1) current.applyMove(r, c, false); }

    GameMove computeBestMove() {
        std::vector<GameMove> moves;
        if (current.nextG != -1) {
            uint16_t free_bits = ~(current.me[current.nextG] | current.opp[current.nextG]) & 0x1FF;
            for(int i=0; i<9; i++) if(free_bits & (1<<i)) moves.push_back({(current.nextG/3)*3+i/3, (current.nextG%3)*3+i%3});
        } else {
            for(int g=0; g<9; g++) if(!((current.mMe|current.mOpp|current.mFull)&(1<<g))) {
                uint16_t free_bits = ~(current.me[g]|current.opp[g]) & 0x1FF;
                for(int i=0; i<9; i++) if(free_bits & (1<<i)) moves.push_back({(g/3)*3+i/3, (g%3)*3+i%3});
            }
        }
        if (moves.empty()) return {0,0};
        
        // Stratégie d'ouverture (Coin ou Centre)
        if (moves.size() == 81) { current.applyMove(0,0,true); return {0,0}; }

        start_time = std::chrono::steady_clock::now(); stop_search = false; node_count = 0;
        GameMove best_overall = moves[0];
        
        for (int depth = 1; depth <= 12; depth++) {
            int alpha = -2000000, beta = 2000000, best_score = -2000000;
            GameMove best_at_depth = best_overall;
            
            // Tri des coups simple (Centre d'abord)
            std::sort(moves.begin(), moves.end(), [&](const GameMove& a, const GameMove& b){
                if (a.row == best_overall.row && a.col == best_overall.col) return true;
                if (b.row == best_overall.row && b.col == best_overall.col) return false;
                return (a.row%3==1 && a.col%3==1) > (b.row%3==1 && b.col%3==1);
            });

            for (auto m : moves) {
                BoardState next = current; next.applyMove(m.row, m.col, true);
                int val = -negamax(next, depth-1, -beta, -alpha, 1, false);
                if (stop_search) break;
                if (val > best_score) { best_score = val; best_at_depth = m; }
                alpha = std::max(alpha, val);
            }
            if (stop_search) break;
            best_overall = best_at_depth;
            if (best_score > 900000) break; 
        }

        current.applyMove(best_overall.row, best_overall.col, true);
        return best_overall;
    }

    int negamax(BoardState& s, int depth, int alpha, int beta, int d_from_root, bool isMe) {
        if (++node_count % 1024 == 0) {
            if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()-start_time).count() > TEMPS_MAX_MS) {
                stop_search = true; return 0;
            }
        }

        uint64_t h = getHash(s);
        if (transposition_table.count(h) && transposition_table[h].depth >= depth) {
            if (transposition_table[h].flag == 0) return transposition_table[h].score;
            if (transposition_table[h].flag == 1) alpha = std::max(alpha, transposition_table[h].score);
            else beta = std::min(beta, transposition_table[h].score);
            if (alpha >= beta) return transposition_table[h].score;
        }

        // Vérification de fin de partie (victoire/défaite immédiate)
        bool wMe = false, wOpp = false;
        for (auto m : MASQUES_VICTOIRE) { if ((s.mMe & m) == m) wMe = true; if ((s.mOpp & m) == m) wOpp = true; }
        if (wMe) return isMe ? (COEF_VICTOIRE - d_from_root) : -(COEF_VICTOIRE - d_from_root);
        if (wOpp) return isMe ? -(COEF_VICTOIRE - d_from_root) : (COEF_VICTOIRE - d_from_root);
        
        if (depth == 0) return isMe ? s.evaluate(d_from_root) : -s.evaluate(d_from_root);

        // Hot loop pour la génération de coups
        int gStart = s.nextG, gEnd = s.nextG + 1;
        if(s.nextG == -1) { gStart = 0; gEnd = 9; }

        int best = -2000000, old_alpha = alpha;
        bool has_moves = false;

        for (int g = gStart; g < gEnd; g++) {
            if (s.nextG == -1 && ((s.mMe | s.mOpp | s.mFull) & (1 << g))) continue;
            uint16_t free_bits = ~(s.me[g] | s.opp[g]) & 0x1FF;
            for (int i = 0; i < 9; i++) {
                if (free_bits & (1 << i)) {
                    has_moves = true;
                    BoardState next = s;
                    next.applyMove((g/3)*3 + i/3, (g%3)*3 + i%3, isMe);
                    int val = -negamax(next, depth-1, -beta, -alpha, d_from_root + 1, !isMe);
                    if (stop_search) return 0;
                    if (val > best) best = val;
                    alpha = std::max(alpha, val);
                    if (alpha >= beta) goto store_and_return;
                }
            }
        }

        if (!has_moves) return 0;

    store_and_return:
        int flag = (best <= old_alpha) ? 2 : (best >= beta ? 1 : 0);
        transposition_table[h] = {depth, best, flag};
        return best;
    }
};

#endif
