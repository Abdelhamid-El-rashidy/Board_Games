/**
 * @file main.cpp
 * @brief Entry point for the FCAI X-O (Tic-Tac-Toe) game.
 *
 * This file initializes the X-O game by creating the user interface, board, and players.
 * It then launches the game loop via the GameManager class.
 * All dynamically allocated objects are properly deleted at the end.
 */

#include <iostream> // Required for input/output operations (cout, cin)
#include <string>   // Required for string
#include <vector>   // Required for vector
#include <memory>   // Required for unique_ptr

#include "../include/BoardGame_Classes.h"
#include "../include/Four_Four_XO.h"
#include "../include/Infinity_XO_Classes.h"
#include "../include/Numerical_X_O.h"
#include "../include/Misere_Tic_Tac_Toe.h"
#include "../include/SUS_Classes.h"
#include "../include/X_O_5x5.h"
#include "../include/Pyramid_XO.h"
#include "../include/Four-in-a-row.h"
#include "../include/Diamond_Tic_Tac_Toe.h"
#include "../include/Obstacles_XO.h"
#include "../include/Words_XO.h"
#include "../include/Memory_Tic-Tac-Toe.h"

using namespace std;

/**
 * @brief Main function to run the X-O game.
 *
 * This function orchestrates the game by:
 * - Initializing the random number generator
 * - Creating the X-O specific UI and board
 * - Setting up players using the UI
 * - Running the main game loop through the GameManager
 * - Performing cleanup of dynamically allocated memory
 *
 * @return int Returns 0 on successful execution.
 */
int main() {

    srand(static_cast<unsigned int>(time(0)));  // Seed the random number generator
    
    // The Menu
    int choice = 1;
    while(choice != 0) {
        cout << "--Welcome to the FCAI Board Game Collection:\n";
        cout << "------------------------------------------\n";
        cout << "0. Exit\n";
        cout << "1. Infinity X_O\n";
        cout << "2. Numerical X_O\n";
        cout << "3. 5 x 5 X_O\n";
        cout << "4. Misere X_O\n";
        cout << "5. Four_Four X_O\n";
        cout << "6. SUS Game\n";
        cout << "7. Pyramid XO\n";
        cout << "8. Four_in_a_row\n";
        cout << "9. Diamond_Tic_Tac_Toe\n";
        cout << "10. Obstacles_XO\n";
        cout << "11. Word_XO\n";
        cout << "12. Memory tic-tac-toe\n";
        cout << "Select a game (0-12): ";
        cin >> choice;

        switch (choice) {
            case 0: {
                cout << "Exiting the program. Goodbye!\\n";
                return 0;
            }
            case 1: {
                UI<char>* game_ui = new Infinit_XO_UI();

                // Create the game board. For X-O, this is an X_O_Board.
                Board<char>* xo_board = new Infinity_X_O_Board();

                // Use the UI to set up the players for the game.
                // The UI returns a dynamically allocated array of Player pointers.
                Player<char>** players = game_ui->setup_players();

                // Create the game manager with the board and the array of players.
                GameManager<char> x_o_game(xo_board, players, game_ui);

                // Run the game loop.
                x_o_game.run();

                // --- Cleanup ---
                // Delete the dynamically allocated board object.
                delete xo_board;

                // Delete the individual player objects.
                for (int i = 0; i < 2; ++i) {
                    delete players[i];
                }
                // Delete the dynamically allocated array of player pointers itself.
                delete[] players;
            }
            case 2: {
                UI<char>* game_ui = new Numerical_XO_UI();

                // Create the game board. For X-O, this is an X_O_Board.
                Board<char>* xo_board = new Numerical_X_O_Board();

                // Use the UI to set up the players for the game.
                // The UI returns a dynamically allocated array of Player pointers.
                Player<char>** players = game_ui->setup_players();

                // Create the game manager with the board and the array of players.
                GameManager<char> x_o_game(xo_board, players, game_ui);

                // Run the game loop.
                x_o_game.run();

                // --- Cleanup ---
                // Delete the dynamically allocated board object.
                delete xo_board;

                // Delete the individual player objects.
                for (int i = 0; i < 2; ++i) {
                    delete players[i];
                }
                // Delete the dynamically allocated array of player pointers itself.
                delete[] players;
            }
            case 3: {
                UI<char>* game_ui = new X_O_5x5_UI();

                // Create the game board. For X-O, this is an X_O_Board.
                Board<char>* xo_board = new X_O_5x5_Board();

                // Use the UI to set up the players for the game.
                // The UI returns a dynamically allocated array of Player pointers.
                Player<char>** players = game_ui->setup_players();

                // Create the game manager with the board and the array of players.
                GameManager<char> x_o_game(xo_board, players, game_ui);

                // Run the game loop.
                x_o_game.run();

                // --- Cleanup ---
                // Delete the dynamically allocated board object.
                delete xo_board;

                // Delete the individual player objects.
                for (int i = 0; i < 2; ++i) {
                    delete players[i];
                }
                // Delete the dynamically allocated array of player pointers itself.
                delete[] players;
            }
            case 4: {
                UI<char>* game_ui = new Misere_Tic_Tac_Toe_UI();

                // Create the game board. For X-O, this is an X_O_Board.
                Board<char>* xo_board = new Misere_Tic_Tac_Toe_board();

                // Use the UI to set up the players for the game.
                // The UI returns a dynamically allocated array of Player pointers.
                Player<char>** players = game_ui->setup_players();

                // Create the game manager with the board and the array of players.
                GameManager<char> x_o_game(xo_board, players, game_ui);

                // Run the game loop.
                x_o_game.run();

                // --- Cleanup ---
                // Delete the dynamically allocated board object.
                delete xo_board;

                // Delete the individual player objects.
                for (int i = 0; i < 2; ++i) {
                    delete players[i];
                }
                // Delete the dynamically allocated array of player pointers itself.
                delete[] players;
            }
            case 5: {
                UI<char>* game_ui = new Four_Four_XO_UI();

                // Create the game board. For X-O, this is an X_O_Board.
                Board<char>* xo_board = new Four_Four_XO_Board();

                // Use the UI to set up the players for the game.
                // The UI returns a dynamically allocated array of Player pointers.
                Player<char>** players = game_ui->setup_players();

                // Create the game manager with the board and the array of players.
                GameManager<char> x_o_game(xo_board, players, game_ui);

                // Run the game loop.
                x_o_game.run();

                // --- Cleanup ---
                // Delete the dynamically allocated board object.
                delete xo_board;

                // Delete the individual player objects.
                for (int i = 0; i < 2; ++i) {
                    delete players[i];
                }
                // Delete the dynamically allocated array of player pointers itself.
                delete[] players;
            }
            case 6: {
                UI<char>* game_ui = new SUS_UI();

                // Create the game board. For X-O, this is an X_O_Board.
                Board<char>* xo_board = new SUS_Board();

                // Use the UI to set up the players for the game.
                // The UI returns a dynamically allocated array of Player pointers.
                Player<char>** players = game_ui->setup_players();

                // Create the game manager with the board and the array of players.
                GameManager<char> x_o_game(xo_board, players, game_ui);

                // Run the game loop.
                x_o_game.run();

                // --- Cleanup ---
                // Delete the dynamically allocated board object.
                delete xo_board;

                // Delete the individual player objects.
                for (int i = 0; i < 2; ++i) {
                    delete players[i];
                }
                // Delete the dynamically allocated array of player pointers itself.
                delete[] players;
            }
            case 7: {
                UI<char>* game_ui = new Pyramid_XO_UI();

                // Create the game board. For X-O, this is an X_O_Board.
                Board<char>* xo_board = new Pyramid_XO_Board();

                // Use the UI to set up the players for the game.
                // The UI returns a dynamically allocated array of Player pointers.
                Player<char>** players = game_ui->setup_players();

                // Create the game manager with the board and the array of players.
                GameManager<char> x_o_game(xo_board, players, game_ui);

                // Run the game loop.
                x_o_game.run();

                // --- Cleanup ---
                // Delete the dynamically allocated board object.
                delete xo_board;

                // Delete the individual player objects.
                for (int i = 0; i < 2; ++i) {
                    delete players[i];
                }
                // Delete the dynamically allocated array of player pointers itself.
                delete[] players;
            }
            case 8: {
                UI<char>* game_ui = new Four_in_a_row_UI();

                // Create the game board. For X-O, this is an X_O_Board.
                Board<char>* xo_board = new Four_in_a_row_Board();

                // Use the UI to set up the players for the game.
                // The UI returns a dynamically allocated array of Player pointers.
                Player<char>** players = game_ui->setup_players();

                // Create the game manager with the board and the array of players.
                GameManager<char> x_o_game(xo_board, players, game_ui);

                // Run the game loop.
                x_o_game.run();

                // --- Cleanup ---
                // Delete the dynamically allocated board object.
                delete xo_board;

                // Delete the individual player objects.
                for (int i = 0; i < 2; ++i) {
                    delete players[i];
                }
                // Delete the dynamically allocated array of player pointers itself.
                delete[] players;
            }
            case 9: {
                UI<char>* game_ui = new Diamond_Tic_Tac_Toe_UI();

                // Create the game board. For X-O, this is an X_O_Board.
                Board<char>* xo_board = new Diamond_Tic_Tac_Toe_Board();

                // Use the UI to set up the players for the game.
                // The UI returns a dynamically allocated array of Player pointers.
                Player<char>** players = game_ui->setup_players();

                // Create the game manager with the board and the array of players.
                GameManager<char> x_o_game(xo_board, players, game_ui);

                // Run the game loop.
                x_o_game.run();

                // --- Cleanup ---
                // Delete the dynamically allocated board object.
                delete xo_board;

                // Delete the individual player objects.
                for (int i = 0; i < 2; ++i) {
                    delete players[i];
                }
                // Delete the dynamically allocated array of player pointers itself.
                delete[] players;
            }
            case 10: {
                UI<char>* game_ui = new Obstacles_XO_UI();

                // Create the game board. For X-O, this is an X_O_Board.
                Board<char>* xo_board = new Obstacles_XO_Board();

                // Use the UI to set up the players for the game.
                // The UI returns a dynamically allocated array of Player pointers.
                Player<char>** players = game_ui->setup_players();

                // Create the game manager with the board and the array of players.
                GameManager<char> x_o_game(xo_board, players, game_ui);

                // Run the game loop.
                x_o_game.run();

                // --- Cleanup ---
                // Delete the dynamically allocated board object.
                delete xo_board;

                // Delete the individual player objects.
                for (int i = 0; i < 2; ++i) {
                    delete players[i];
                }
                // Delete the dynamically allocated array of player pointers itself.
                delete[] players;
            }
            case 11: {
                UI<char>* game_ui = new Word_XO_UI();

                // Create the game board. For X-O, this is an X_O_Board.
                Board<char>* xo_board = new Word_XO_Board();

                // Use the UI to set up the players for the game.
                // The UI returns a dynamically allocated array of Player pointers.
                Player<char>** players = game_ui->setup_players();

                // Create the game manager with the board and the array of players.
                GameManager<char> x_o_game(xo_board, players, game_ui);

                // Run the game loop.
                x_o_game.run();

                // --- Cleanup ---
                // Delete the dynamically allocated board object.
                delete xo_board;

                // Delete the individual player objects.
                for (int i = 0; i < 2; ++i) {
                    delete players[i];
                }
                // Delete the dynamically allocated array of player pointers itself.
                delete[] players;
            }

            case 12 : {
                UI<char>* game_ui = new MEMORY_TIC_TAC_TOE_UI();

                // Create the game board. For X-O, this is an X_O_Board.
                Board<char>* xo_board = new MEMORY_TIC_TAC_TOE_Board();

                // Use the UI to set up the players for the game.
                // The UI returns a dynamically allocated array of Player pointers.
                Player<char>** players = game_ui->setup_players();

                // Create the game manager with the board and the array of players.
                GameManager<char> x_o_game(xo_board, players, game_ui);

                // Run the game loop.
                x_o_game.run();

                // --- Cleanup ---
                // Delete the dynamically allocated board object.
                delete xo_board;

                // Delete the individual player objects.
                for (int i = 0; i < 2; ++i) {
                    delete players[i];
                }
                // Delete the dynamically allocated array of player pointers itself.
                delete[] players;
            }
            default:
                cout << "Invalid choice. Please try again.\\n";
                choice = 1;
                break;
        }
    }


    // // Run the game loop.
    // x_o_game.run();

    // // --- Cleanup ---
    // // Delete the dynamically allocated board object.
    // delete xo_board;

    // // Delete the individual player objects.
    // for (int i = 0; i < 2; ++i) {
    //     delete players[i];
    // }
    // // Delete the dynamically allocated array of player pointers itself.
    // delete[] players;

    return 0; // Exit successfully
}

// =====================================================================
