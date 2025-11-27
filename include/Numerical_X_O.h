#include "BoardGame_Classes.h"
using namespace std;

#ifndef NUMERICAL_X_O_H
#define NUMERICAL_X_O_H


class Numerical_X_O_Board : public Board<char> {
private:
    char blank_symbol = '0';
    int counter;

public:
    Numerical_X_O_Board();

    bool update_board(Move<char>* move);


    bool is_win(Player<char>* player);


    bool is_lose(Player<char>*) { return false; };


    bool is_draw(Player<char>* player);


    bool game_is_over(Player<char>* player);

    virtual ~Numerical_X_O_Board() {}
};

class Numerical_XO_UI : public UI<char> {
public:
    Numerical_XO_UI();

    Player<char>* create_player(string& name, char symbol, PlayerType type);


    virtual Move<char>* get_move(Player<char>* player);

    virtual ~Numerical_XO_UI() {}
};

class nMove : public Move<char> {
private:
    char value;
public:
    /**
     * Constructor to intialize Base Class Move and Derived Class dualMove
     * @param x x-coordinate in grid
     * @param y y-coordinate in grid
     * @param symbol the current player's move symbol
     * @param value the number played by the move
     */
    nMove(int x, int y, char symbol, char value) : Move<char>(x,y,symbol), value(value) {};

    char get_value() const { return value; }

    virtual ~nMove() {}

};

#endif //NUMERICAL_X_O_H
