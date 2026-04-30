#include <iostream>
#include "main.h"
#include "TicTacToe.h"
#include "UltimateTicTacToe.h"
#include "Minimax.h"

int main()
{
    // // Test des nouvelles classes et de l'IA Minimax
    // UltimateTicTacToe uttt;
    // TicTacToe& centerGame = uttt.get(1, 1);

    // // On simule quelques coups
    // centerGame.play(0, 0, Symbol::CIRCLE);
    // centerGame.play(0, 1, Symbol::CIRCLE);

    // // L'IA Minimax doit trouver le meilleur coup pour CROSS (bloquer ou gagner)
    // Move bestMove = Minimax::findBestMove(centerGame, Symbol::CROSS);
    // centerGame.play(bestMove.x, bestMove.y, Symbol::CROSS);

    // std::cout << "L'IA Minimax a joue en : (" << bestMove.x << ", " << bestMove.y << ")" << std::endl;
    // std::cout << "Etat de la case bloquee : " 
    //           << (centerGame.getSymbol(0, 2) == Symbol::CROSS ? "Croix (Bloqué !)" : "Autre") 
    //           << std::endl;

    // Game initialization
    game.initialize(10, Level::EASY_1, Mode::DEBUG, false, "Pseudo");

    while (!game.isAllGameFinish())
    {
        // On réinitialise notre plateau interne pour chaque nouvelle partie
        UltimateTicTacToe myUTTT;

        while (!game.isFinish())
        {
            // 1. Récupérer le coup de l'IA adverse
            GameMove opponentMove;
            game.getMove(opponentMove);
            
            // Position globale de l'IA
            int iaGlobalRow = opponentMove.row;
            int iaGlobalCol = opponentMove.col;

            // Décomposition en sous-grille (IA) et position locale (IA)
            int iaSubRow = iaGlobalRow / 3;
            int iaSubCol = iaGlobalCol / 3;
            int iaLocalRow = iaGlobalRow % 3;
            int iaLocalCol = iaGlobalCol % 3;

            // Mise à jour de notre plateau avec le coup de l'IA (CERCLE)
            myUTTT.get(iaSubRow, iaSubCol).play(iaLocalRow, iaLocalCol, Symbol::CIRCLE);
            std::cerr << "IA a joue en (" << iaGlobalRow << "," << iaGlobalCol << ")" << std::endl;

            // RÈGLE ULTIMATE : Le coup local de l'IA détermine NOTRE sous-grille cible
            int targetSubRow = iaLocalRow;
            int targetSubCol = iaLocalCol;

            // 2. Choisir notre coup (CROIX)
            TicTacToe* targetGame = &myUTTT.get(targetSubRow, targetSubCol);
            
            // Si la sous-grille cible est pleine ou déjà gagnée, on peut jouer n'importe où
            if (targetGame->isFull() || targetGame->checkWinner() != Symbol::NONE) {
                for (int i = 0; i < 3; ++i) {
                    for (int j = 0; j < 3; ++j) {
                        if (!myUTTT.get(i, j).isFull() && myUTTT.get(i, j).checkWinner() == Symbol::NONE) {
                            targetSubRow = i;
                            targetSubCol = j;
                            targetGame = &myUTTT.get(targetSubRow, targetSubCol);
                            break;
                        }
                    }
                }
            }

            // Calcul du meilleur coup dans la sous-grille choisie
            Move bestLocalMove = Minimax::findBestMove(*targetGame, Symbol::CROSS);

            // Mise à jour de notre plateau interne
            targetGame->play(bestLocalMove.x, bestLocalMove.y, Symbol::CROSS);

            // Conversion en coordonnées globales pour envoyer au jeu
            GameMove myMove;
            myMove.row = targetSubRow * 3 + bestLocalMove.x;
            myMove.col = targetSubCol * 3 + bestLocalMove.y;

            std::cerr << "Minimax repond en (" << myMove.row << "," << myMove.col << ") dans la sous-grille (" << targetSubRow << "," << targetSubCol << ")" << std::endl;
            game.setMove(myMove);
        }
    }

    return 0;
}
