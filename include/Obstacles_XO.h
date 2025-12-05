/**
* @file Obstacles_XO.h
 * @brief Defines the Obstacles Tic-Tac-Toe game classes.
 */

#ifndef BOARDGAMEFRAMEWORK_OBSTACLES_XO_H
#define BOARDGAMEFRAMEWORK_OBSTACLES_XO_H

#include "BoardGame_Classes.h"

class Obstacles_XO_Board : public Board<char> {
private:
    char obstacle_symbol = '#'; /// Symbol for the obstacles

public:

    /**
 * @brief Constructor: Initializes the Diamond board in a diamond shape.
 */

    Obstacles_XO_Board();

    /**
     * @brief Updates the board with a player's move.
     * @param move Pointer to a Move<char> object containing move coordinates and symbol.
     * @return true if the move is valid and successfully applied, false otherwise.
     */
    bool update_board(Move<char>* move);

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
     * @param player Pointer to the playerended, to evaluate.
     * @return true if the game has  false otherwise.
     */

    bool game_is_over(Player<char>* player);

    virtual ~Obstacles_XO_Board() {};
};

/**
 * @class Obstacles_XO
 * @brief User Interface class for the Obstacles_XOe game.
 *
 * Inherits from the generic `UI<char>` base class and provides
 * X-O�specific functionality for player setup and move input.
 *
 * @see UI
 */

class Obstacles_XO_UI : public UI<char> {
public:
    /**
     * @brief Constructs an Obstacles_XO object.
     *
     * Initializes the base `UI<char>` class with the welcome message "FCAI X-O".
     */
    Obstacles_XO_UI();

    /**
    * @brief Destructor for Misere_UI.
    */

    virtual ~Obstacles_XO_UI() {};

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
};

#endif //BOARDGAMEFRAMEWORK_OBSTACLES_XO_H