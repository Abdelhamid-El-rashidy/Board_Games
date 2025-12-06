/**
 * @file Words_XO.h
 * @brief Defines the Word Tic-Tac-Toe specific classes that extend the generic board game framework.
 *
 * This file provides:
 * - `Word_XO_Board`: A specialized board class for the Word Tic-Tac-Toe game.
 * - `Word_XO_UI`: A user interface class tailored to Word X-O game setup and player interaction.
 * - `neMove`: A specialized move class that allows carrying a specific character value.
 */

#include "BoardGame_Classes.h"
using namespace std;

#ifndef WORDS_XO_H
#define WORDS_XO_H

/**
 * @class Word_XO_Board
 * @brief Represents the Word Tic-Tac-Toe game board.
 *
 * This class inherits from the generic `Board<char>` class and implements
 * the specific logic required for the Word Tic-Tac-Toe game, where players
 * place characters to form words.
 *
 * @see Board
 */
class Word_XO_Board : public Board<char> {
private:
    char blank_symbol = '.'; ///< Character used to represent an empty cell on the board.
    int counter;             ///< Counter to track the number of moves or game state progression.

public:
    /**
     * @brief Default constructor that initializes the Word X-O board.
     */
    Word_XO_Board();

    /**
     * @brief Updates the board with a player's move.
     * @param move Pointer to a Move<char> object containing move coordinates and the character value.
     * @return true if the move is valid and successfully applied, false otherwise.
     */
    bool update_board(Move<char>* move);

    /**
     * @brief Checks if the given player has won the game.
     * @param player Pointer to the player being checked.
     * @return true if the player has formed a winning word configuration, false otherwise.
     */
    bool is_win(Player<char>* player);

    /**
     * @brief Checks if the given player has lost the game.
     * @param player Pointer to the player being checked.
     * @return Always returns false (standard implementation for this game type).
     */
    bool is_lose(Player<char>*) { return false; };

    /**
     * @brief Checks if the game has ended in a draw.
     * @param player Pointer to the player being checked.
     * @return true if the board is full and no winning word is formed, false otherwise.
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
 * @class Word_XO_UI
 * @brief User Interface class for the Word Tic-Tac-Toe game.
 *
 * Inherits from the generic `UI<char>` base class and provides
 * Word X-O specific functionality for player setup and move input.
 *
 * @see UI
 */
class Word_XO_UI : public UI<char> {
public:
    /**
     * @brief Constructs a Word_XO_UI object.
     *
     * Initializes the UI specific to the Word Tic-Tac-Toe game.
     */
    Word_XO_UI();

    /**
     * @brief Destructor for Word_XO_UI.
     */
    ~Word_XO_UI() {};

    /**
     * @brief Creates a player of the specified type.
     * @param name Name of the player.
     * @param symbol Character symbol assigned to the player.
     * @param type The type of the player (Human or Computer).
     * @return Pointer to the newly created Player<char> instance.
     */
    Player<char>* create_player(string& name, char symbol, PlayerType type);

    /**
     * @brief Retrieves the next move from a player.
     * @param player Pointer to the player whose move is being requested.
     * @return A pointer to a new `Move<char>` object (specifically `neMove`) representing the player's action.
     */
    virtual Move<char>* get_move(Player<char>* player);
};

/**
 * @class neMove
 * @brief A specialized move class for Word Tic-Tac-Toe.
 *
 * This class extends `Move<char>` to include a specific character value
 * that the player wishes to place on the board.
 *
 * @see Move
 */
class neMove : public Move<char> {
private:
    char value; ///< The character value associated with this move.
public:
    /**
     * @brief Constructor to initialize Base Class Move and Derived Class neMove.
     * @param x x-coordinate in grid.
     * @param y y-coordinate in grid.
     * @param symbol The current player's symbol.
     * @param value The character value played by the move.
     */
    neMove(int x, int y, char symbol, char value) : Move<char>(x,y,symbol), value(value) {};

    /**
     * @brief Retrieves the character value of the move.
     * @return The character value.
     */
    char get_value() const { return value; }

    /**
     * @brief Destructor for neMove.
     */
    virtual ~neMove() {}

};

#endif //WORDS_XO_H