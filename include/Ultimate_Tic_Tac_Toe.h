//
// Created by abdelhamid on 12/5/25.
//

#ifndef BOARDGAMEFRAMEWORK_ULTIMATE_TIC_TAC_TOE_H
#define BOARDGAMEFRAMEWORK_ULTIMATE_TIC_TAC_TOE_H

#include "BoardGame_Classes.h"

class mainBoard : public Board<char> {
private:
    char blank_symbol = '.';
public:
    /**
     *@brief Default constructor that initializes a 3*3 main Board;
     */
    mainBoard();

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

    virtual ~mainBoard(){};
};

class Ultimate_XO_Board : public Board<char> {
private:
    char blank_symbol = '.';
    Board<char> *mainBoardPtr = new mainBoard();
public:
    /**
     *@brief Default constructor that initializes a 9*9 Board;
     */
    Ultimate_XO_Board();

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

    virtual ~Ultimate_XO_Board() {
        delete mainBoardPtr;
    };
};

class Ultimate_XO_UI : public UI<char> {

public:
    /**
    * @brief Constructs an XO_UI object.
    *
    * Initializes the base `UI<char>` class with the welcome message "FCAI X-O".
    */
    Ultimate_XO_UI();

    /**
     * @brief Destructor for XO_UI.
     */
    virtual ~Ultimate_XO_UI() {};

    Player<char>** setup_players() ;
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

    // virtual void display_board_matrix(const vector<vector<char>> &matrix) const override;
};


#endif //BOARDGAMEFRAMEWORK_ULTIMATE_TIC_TAC_TOE_H