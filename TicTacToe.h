#ifndef TICTACTOE_H_INCLUDED
#define TICTACTOE_H_INCLUDED

enum class Symbol {
    NONE,
    CROSS,
    CIRCLE
};

class TicTacToe {
    public:
        TicTacToe();
        bool play(int x, int y, Symbol s);
        void undo(int x, int y);
        Symbol getSymbol(int x, int y) const;
        Symbol checkWinner() const;
        bool isFull() const;
    private:
        Symbol m_grid[3][3];
};

#endif 