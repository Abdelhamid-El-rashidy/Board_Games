//
// Created by abdelhamid on 11/26/25.
//

#ifndef BOARDGAMEFRAMEWORK_FOUR_FOUR_XO_H
#define BOARDGAMEFRAMEWORK_FOUR_FOUR_XO_H
#include "BoardGame_Classes.h"

/**
 * @class Four_Four_XO_Board
 * @brief Represent the 4*4 XO Tic Tac Toe Game.
 */
class Four_Four_XO_Board : public Board<char> {
    char blank_symbol = '.';

public:
    /** Default constructor of 4*4 Board Size */
    Four_Four_XO_Board();
    /**
     * @brief Update the board with a new move.
     * @param move The move object containing position and symbol.
     * @return true if the move is valid and applied, false otherwise.
     */
    virtual bool update_board(Move<char>* move);

    /** @brief Check if a player has won. */
    virtual bool is_win(Player<char>*);

    /** @brief Check if a player has lost. */
    virtual bool is_lose(Player<char>*);

    /** @brief Check if the game ended in a draw. */
    virtual bool is_draw(Player<char>*);

    /** @brief Check if the game is over. */
    virtual bool game_is_over(Player<char>*);

    /** Destructor */
    virtual ~Four_Four_XO_Board();

};


/**
 * @class Four_Four_XO_UI
 * @brief User Interface class for 4*4 XO Tic Tac Toe Game.
 *
 * Inherits from the generic `UI<char>` base class and provides
 * X-O�specific functionality for player setup and move input.
 *
 * @see UI
 */
class Four_Four_XO_UI : public UI<char> {

public:
    Four_Four_XO_UI();
    /**
     * @brief Retrieves the next move from a player.
     * @param player Pointer to the player whose move is being requested.
     * @return A pointer to a new `Move<char>` object representing the player's action.
     */
    virtual Move<char>* get_move(Player<char>* player);

    virtual ~Four_Four_XO_UI();

};


/**
 * @class dualMove
 * @brief Customized move that changed symbol position in board
 *
 * Inherits from the generic `Move<char>` base class and provides
 * specific functionality for move.
 *
 * @see UI
 */
class dualMove : public Move<char> {
private:
    int horizontal;
    int vertical;
public:
    /**
     * Constructor to intialize Base Class Move and Derived Class dualMove
     * @param x x-coordinate in grid
     * @param y y-coordinate in grid
     * @param symbol the current player's move symbol
     * @param hort horizontal change (right,left) 1 or -1
     * @param vert vertical change (up,down) 1 or -1
     */
    dualMove(int x, int y, char symbol, int hort, int vert) : Move<char>(x,y,symbol), horizontal(hort), vertical(vert) {};

    /**
     * @return The horizontal change
     */
    int get_horizontal() const;
    /**
     * @return The vertical change
     */
    int get_vertical() const;

    virtual ~dualMove();

};

#endif //BOARDGAMEFRAMEWORK_FOUR_FOUR_XO_H