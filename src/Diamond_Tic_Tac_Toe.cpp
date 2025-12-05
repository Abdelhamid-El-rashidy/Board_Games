#include <bits/stdc++.h>
#include "../include/Diamond_Tic_Tac_Toe.h"

using namespace std;

//--------------------------------------- Board Implementation

Diamond_Tic_Tac_Toe_Board::Diamond_Tic_Tac_Toe_Board() : Board<char>(7, 7) {
    // Logic to draw the Diamond shape on a 7x7 grid.
    // The center is at (3, 3). The Manhattan distance radius is 3.
    // Equation: abs(row - 3) + abs(col - 3) <= 3

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (abs(i - 3) + abs(j - 3) <= 3) {
                board[i][j] = blank_symbol; // Inside the Diamond (Valid cell)
            } else {
                board[i][j] = '#'; // Outside the Diamond (Invalid/Decor)
            }
        }
    }
}

bool Diamond_Tic_Tac_Toe_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    // 1. Check boundaries.
    // 2. Check if the cell is valid (not '#').
    // 3. Allow move if cell is empty OR if mark is 0 (Undo operation).
    if (x >= 0 && x < rows && y >= 0 && y < columns &&
        board[x][y] != '#' &&
        (board[x][y] == blank_symbol || mark == 0)) {

        if (mark == 0) { // Undo move
            n_moves--;
            board[x][y] = blank_symbol;
        } else {         // Apply move
            n_moves++;
            board[x][y] = toupper(mark);
        }
        return true;
    }
    return false;
}

bool Diamond_Tic_Tac_Toe_Board::is_win(Player<char>* player) {
    char sym = player->get_symbol();

    // Variables to store the maximum consecutive sequence found in each direction
    int max_row = 0;   // Horizontal
    int max_col = 0;   // Vertical
    int max_diag1 = 0; // Main Diagonal (Top-Left to Bottom-Right)
    int max_diag2 = 0; // Anti-Diagonal (Top-Right to Bottom-Left)

    // Iterate over every cell in the board
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {

            // If the cell does not belong to the current player, skip it
            if (board[i][j] != sym) continue;

            // 1. Check Horizontal (Row)
            int current_row = 0;
            for (int k = 0; k < 7; k++) {
                if (j + k < columns && board[i][j + k] == sym) current_row++;
                else break;
            }
            if (current_row > max_row) max_row = current_row;

            // 2. Check Vertical (Column)
            int current_col = 0;
            for (int k = 0; k < 7; k++) {
                if (i + k < rows && board[i + k][j] == sym) current_col++;
                else break;
            }
            if (current_col > max_col) max_col = current_col;

            // 3. Check Main Diagonal
            int current_diag1 = 0;
            for (int k = 0; k < 7; k++) {
                if (i + k < rows && j + k < columns && board[i + k][j + k] == sym) current_diag1++;
                else break;
            }
            if (current_diag1 > max_diag1) max_diag1 = current_diag1;

            // 4. Check Anti-Diagonal
            int current_diag2 = 0;
            for (int k = 0; k < 7; k++) {
                if (i + k < rows && j - k >= 0 && board[i + k][j - k] == sym) current_diag2++;
                else break;
            }
            if (current_diag2 > max_diag2) max_diag2 = current_diag2;
        }
    }

    // Win Condition: One line of length >= 4 AND another line of length >= 3
    // The two lines must be in DIFFERENT directions.

    // Case 1: Primary line (4) is Horizontal. Secondary (3) must be Vertical or Diagonal.
    if (max_row >= 4 && (max_col >= 3 || max_diag1 >= 3 || max_diag2 >= 3)) return true;

    // Case 2: Primary line (4) is Vertical. Secondary (3) must be Horizontal or Diagonal.
    if (max_col >= 4 && (max_row >= 3 || max_diag1 >= 3 || max_diag2 >= 3)) return true;

    // Case 3: Primary line (4) is Main Diagonal. Secondary (3) must be Row, Col, or Anti-Diag.
    if (max_diag1 >= 4 && (max_row >= 3 || max_col >= 3 || max_diag2 >= 3)) return true;

    // Case 4: Primary line (4) is Anti-Diagonal. Secondary (3) must be Row, Col, or Main Diag.
    if (max_diag2 >= 4 && (max_row >= 3 || max_col >= 3 || max_diag1 >= 3)) return true;

    return false;
}

bool Diamond_Tic_Tac_Toe_Board::is_draw(Player<char>* player) {
    return (n_moves == 25 && !is_win(player));
}

bool Diamond_Tic_Tac_Toe_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}


//--------------------------------------- UI Implementation

Diamond_Tic_Tac_Toe_UI::Diamond_Tic_Tac_Toe_UI() : UI<char>("Welcome to 7x7 Diamond Tic-Tac-Toe Game", 3) {}

Player<char>* Diamond_Tic_Tac_Toe_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
         << " player: " << name << " (" << symbol << ")\n";
    return new Player<char>(name, symbol, type);
}

Move<char>* Diamond_Tic_Tac_Toe_UI::get_move(Player<char>* player) {
    int x, y;
    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nPlease enter your move x and y (0 to 6): ";
        cin >> x >> y;
    } else {
        // Random Computer Logic
        // Must choose a valid cell inside the Diamond
        do {
            x = rand() % 7;
            y = rand() % 7;
        } while (abs(x - 3) + abs(y - 3) > 3 ||
                 player->get_board_ptr()->get_board_matrix()[x][y] != '.');
    }
    return new Move<char>(x, y, player->get_symbol());
}