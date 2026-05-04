#include <iostream>
#include "main.h"
#include "MyAI.hpp"

int main()
{
    // Game initialization
    game.initialize(10, Level::MEDIUM_2, Mode::ARENA, false, "Pseudo");
    
    AI myAI;

    while (!game.isAllGameFinish())
    {
        myAI.reset();

        while (!game.isFinish())
        {
            // Get IA move
            GameMove gameMove{-1, -1};
            game.getMove(gameMove);
            std::cerr << "IA move " << gameMove.row << " " << gameMove.col << std::endl;

            // Register IA move (handles the -1, -1 case if we play first)
            myAI.registerOpponentMove(gameMove.row, gameMove.col);

            // Compute our move
            GameMove myMove = myAI.computeBestMove();

            // Send your move
            std::cerr << "Send move " << myMove.row << " " << myMove.col << std::endl;
            game.setMove(myMove);
        }
    }

    return 0;
}
