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
#include "../include/Ultimate_Tic_Tac_Toe.h"

#include "include/gamesMenu.h"

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

    Menu gamesMenu;
    gamesMenu.run();

    return 0; // Exit successfully
}

// =====================================================================
