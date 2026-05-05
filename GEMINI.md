# Ultimate Tic-Tac-Toe AI Project

## Project Overview
This project is a C++ implementation of an autonomous agent designed to play **Ultimate Tic-Tac-Toe**. The goal is to develop a high-performance AI capable of defeating various difficulty levels in a specialized arena.

### Main Technologies
- **Language:** C++ (Standard STL)
- **Graphics & Arena:** Allegro 5
- **Build System:** MSYS2/MinGW (g++)
- **Architecture:** Interface-based design using `IGame` for arena integration.

### Core Algorithms
- **Bitboards:** Used for ultra-fast board state manipulation and victory checking.
- **Negamax with Alpha-Beta Pruning:** The primary search algorithm for decision-making.
- **Iterative Deepening:** Allows the AI to search deeper as time permits (targeted 400ms limit).
- **Heuristics:** Strategic weighting of macro-board positions, threats, and free-move penalties.

## Project Structure
- `main.cpp`: Entry point, manages the game loop against the arena's `Level` difficulty.
- `main.h`: Core definitions, enums (`Level`, `Mode`), and the `IGame` interface.
- `MyAI.hpp`: Main AI implementation using bitboards and Negamax.
- `MCTS_AI.hpp`: Alternative AI implementation using Monte Carlo Tree Search.
- `ia_plan/`: Contains strategic analysis (`ANALYSIS.md`) and development plans (`Plan_IA_Ultimate_TicTacToe.md`).
- `images/`: Visual assets for the Allegro-based UI.
- `libUTTTLib.a`: Precompiled game engine and arena library.

## Building and Running

### Build Instructions
The project is built using `g++`. Ensure Allegro 5 and the `libUTTTLib.a` are in the library path.

```powershell
g++ -fdiagnostics-color=always -g -fno-lto main.cpp -o main.exe -I. -L. -lUTTTLib -lallegro_font-5.2 -lallegro_ttf-5.2 -lallegro_image-5.2 -lallegro_primitives-5.2 -lallegro-5.2
```

*(Note: VS Code users can use the provided build task in `.vscode/tasks.json`)*

### Running the Project
```powershell
./main.exe
```

## Development Conventions
- **Performance First:** Use bitwise operations and avoid dynamic memory allocation in the search hot loops.
- **Strategic Evolution:** Refer to `ia_plan/ANALYSIS.md` for current heuristic weightings and identified improvements.
- **Time Management:** The AI should respect a search time limit (currently set to ~370ms in `MyAI.hpp`) to avoid arena timeouts.
- **Testing:** The game is configured to run batches of games (e.g., 10 games) in `DEBUG` mode to calculate win rates against specific levels.
