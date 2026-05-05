#include <iostream>
#include <cstdlib>
#include "main.h"
#include "MyAI.hpp"

// DEBUG: Set to true to close the program automatically after tests
const bool AUTO_EXIT = true;

int main()
{
    // Game initialization: 10 games against MEDIUM_2
    game.initialize(10, Level::MEDIUM_2, Mode::ARENA, false, "MasterAI");
    
    AI myAI;
    int myWins = 0;
    int oppWins = 0;
    int ties = 0;

    std::cout << "--- STARTING AUTO-TEST MISSION ---" << std::endl;

    while (!game.isAllGameFinish())
    {
        myAI.reset();

        while (!game.isFinish())
        {
            GameMove gameMove{-1, -1};
            game.getMove(gameMove);
            myAI.registerOpponentMove(gameMove.row, gameMove.col);

            GameMove myMove = myAI.computeBestMove();
            game.setMove(myMove);
        }

        Winner w = game.getWinner();
        if (w == Winner::PLAYER) {
            myWins++;
            std::cout << "Result: WIN" << std::endl;
        } else if (w == Winner::IA) {
            oppWins++;
            std::cout << "Result: LOSS" << std::endl;
        } else {
            ties++;
            std::cout << "Result: TIE" << std::endl;
        }
    }

    std::cout << "--- MISSION FINAL SCORE ---" << std::endl;
    std::cout << "Wins: " << myWins << " | Losses: " << oppWins << " | Ties: " << ties << std::endl;
    std::cout << "Win Rate: " << (myWins * 100.0 / (myWins + oppWins + ties)) << "%" << std::endl;

    if (AUTO_EXIT) {
        std::cout << "Auto-exiting as requested..." << std::endl;
        std::exit(0);
    }

    return 0;
}
