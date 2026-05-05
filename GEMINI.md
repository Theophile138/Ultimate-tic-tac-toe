# Ultimate Tic-Tac-Toe AI Project

This project is a C++ implementation of an AI for the "Ultimate Tic-Tac-Toe" game. It uses the Allegro 5 library for graphics and a custom static library (`libUTTTLib.a`) for the game engine.

## Project Overview

- **Purpose:** Develop a competitive AI capable of defeating built-in AI levels (up to `VERY_HARD`).
- **Game Rules:** Ultimate Tic-Tac-Toe consists of a 3x3 grid of small 3x3 boards. Winning a small board places your mark on the large board. The objective is to win three small boards in a row on the large board.
- **AI Strategy:** The current implementation in `MyAI.hpp` uses the **Minimax algorithm** with **Alpha-Beta pruning**.
- **Key Technologies:**
    - **Language:** C++
    - **UI/Graphics:** Allegro 5
    - **Game Engine:** Provided via `libUTTTLib.a` and `main.h`.

## Key Files

- `main.h`: Defines the core game interfaces (`IGame`), move structures (`GameMove`), and enumerations for difficulty levels, game modes, and winners.
- `MyAI.hpp`: Contains the `AI` class and `BoardState` logic. This is where the search algorithm and heuristic evaluation are implemented.
- `main.cpp`: The entry point that orchestrates the game loop between the engine and the custom AI.
- `libUTTTLib.a`: Static library containing the compiled game engine.
- `ia_plan/`: Contains planning documentation (`Plan_IA_Ultimate_TicTacToe.md`) and project requirements.
- `images/`: Asset files for rendering crosses and circles.

## Building and Running

### Prerequisites
- **Compiler:** `g++` (MinGW-w64/MSYS2 recommended).
- **Libraries:** Allegro 5 (DLLs are included in the root directory).

### Build Command
The project can be built using the following command (as defined in `.vscode/tasks.json`):

```bash
g++ -g -fno-lto *.cpp -o main.exe -I. -L. -lUTTTLib -lallegro_font-5.2 -lallegro_ttf-5.2 -lallegro_image-5.2 -lallegro_primitives-5.2 -lallegro-5.2
```

### Running
Execute `main.exe`. Ensure all `.dll` files provided in the root directory remain in the same folder as the executable.

## Development Conventions

- **AI Integration:** The `AI` class must implement `registerOpponentMove(row, col)` and `computeBestMove()`.
- **Heuristics:** Evaluation logic is located in `BoardState::evaluate()`.
- **Search Depth:** Controlled by `DEPTH_NORMAL` and `DEPTH_OPEN` constants in `MyAI.hpp`.
- **Game Engine Interaction:** Use the `game` global object (type `IGame&`) to initialize the session, get opponent moves, and submit AI moves.

## Planned Optimizations (from `ia_plan/`)
- Implementation of **Bitboards** for faster state management.
- **Transposition Tables** to cache evaluated positions.
- Improved heuristics prioritizing strategic control of the macro-board.
