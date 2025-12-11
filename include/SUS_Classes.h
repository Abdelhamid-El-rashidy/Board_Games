/**
* @file XO_Classes.h
 * @brief Defines the X-O (Tic-Tac-Toe) specific classes that extend the generic board game framework.
 *
 * This file provides:
 * - `X_O_Board`: A specialized board class for the Tic-Tac-Toe game.
 * - `XO_UI`: A user interface class tailored to X-O game setup and player interaction.
 */
#ifndef SUS_CLASSES_H
#define SUS_CLASSES_H

#include "BoardGame_Classes.h"
#include <string>

using namespace std ;

/**
 * @class X_O_Board
 * @brief Represents the Tic-Tac-Toe game board.
 *
 * This class inherits from the generic `Board<char>` class and implements
 * the specific logic required for the Tic-Tac-Toe (X-O) game, including
 * move updates, win/draw detection, and display functions.
 *
 * @see Board
 */

class SUS_Board : public Board<char> {
private:
    char blank_symbol = '.';

public:

    /**
 * @brief Default constructor that initializes a 3x3 X-O board.
 */
    SUS_Board();

    /**
     * @brief Updates the board with a player's move.
     * @param move Pointer to a Move<char> object containing move coordinates and symbol.
     * @return true if the move is valid and successfully applied, false otherwise.
     */

    bool update_board(Move<char>* move) ;

    /**
    * @brief Checks if the given player has won the game.
    * @param player Pointer to the player being checked.
    * @return true if the player has a winning line, false otherwise.
    */
    bool is_win(Player<char>* player);
    /**
     * @brief Checks if the given player has lost the game.
     * @param player Pointer to the player being checked.
     * @return Always returns false (not used in X-O logic).
     */
    bool is_lose(Player<char>*) { return false; };

    /**
     * @brief Checks if the game has ended in a draw.
     * @param player Pointer to the player being checked.
     * @return true if all cells are filled and no player has won, false otherwise.
     */
    bool is_draw(Player<char>* player);

    /**
     * @brief Determines if the game is over (win or draw).
     * @param player Pointer to the player to evaluate.
     * @return true if the game has ended, false otherwise.
     */
    bool game_is_over(Player<char>* player);

};

/**
 * @class XO_UI
 * @brief User Interface class for the X-O (Tic-Tac-Toe) game.
 *
 * Inherits from the generic `UI<char>` base class and provides
 * X-O�specific functionality for player setup and move input.
 *
 * @see UI
 */

class SUS_UI : public UI<char> {
public:

    /**
   * @brief Constructs an XO_UI object.
   *
   * Initializes the base `UI<char>` class with the welcome message "FCAI X-O".
   */
    SUS_UI();

    /**
     * @brief Destructor for XO_UI.
     */

    // Player<char>** setup_players() ;
    /**
     * @brief Creates a player of the specified type.
     * @param name Name of the player.
     * @param symbol Character symbol ('X' or 'O') assigned to the player.
     * @param type The type of the player (Human or Computer).
     * @return Pointer to the newly created Player<char> instance.
     */
    Player<char>* create_player(string& name, char symbol, PlayerType type);

    /**
     * @brief Retrieves the next move from a player.
     * @param player Pointer to the player whose move is being requested.
     * @return A pointer to a new `Move<char>` object representing the player's action.
     */
    virtual Move<char>* get_move(Player<char>* player);
    ~SUS_UI() {};
};

class ne_Move : public Move<char> {
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
    ne_Move(int x, int y, char symbol, char value) : Move<char>(x,y,symbol), value(value) {};

    char get_value() const { return value; }

    virtual ~ne_Move() {}

};

#endif
