#include <bits/stdc++.h>
#include "../include/Obstacles_XO.h"

using namespace std;

//--------------------------------------- Board Implementation

Obstacles_XO_Board::Obstacles_XO_Board() : Board<char>(6, 6) {

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            board[i][j] = '.';
        }
    }
}

bool Obstacles_XO_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    // 1. Validate the move
    if (x >= 0 && x < rows && y >= 0 && y < columns && 
        board[x][y] == '.' && board[x][y] != obstacle_symbol) {
        

        if (mark == 0) { // Undo move
            n_moves--;
            board[x][y] = '.';
        }
        else {
            // Apply move
            n_moves++;
            board[x][y] = toupper(mark);
        }

        
        if (n_moves % 2 == 0) {
            int obstacles_added = 0;
            // Try to add 2 obstacles (unless board is full)
            // We loop until we place 2 obstacles or verify board is full
            int safety_counter = 0; 
            while (obstacles_added < 2 && n_moves < 36 && safety_counter < 1000) {
                int r = rand() % 6;
                int c = rand() % 6;
                
                if (board[r][c] == '.') {
                    board[r][c] = obstacle_symbol;
                    n_moves++; // Count obstacle as a filled spot
                    obstacles_added++;
                }
                safety_counter++;
            }
        }


        return true;
    }
    return false;
}

bool Obstacles_XO_Board::is_win(Player<char>* player) {
    char sym = player->get_symbol();

    // Check for 4 consecutive symbols in any direction
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            
            // Only verify if the current cell belongs to the player
            if (board[i][j] != sym) continue;

            // 1. Check Horizontal (Right)
            // Ensure we have room for 3 more cells to the right
            if (j + 3 < columns && 
                board[i][j+1] == sym && 
                board[i][j+2] == sym && 
                board[i][j+3] == sym) {
                return true;
            }

            // 2. Check Vertical (Down)
            // Ensure we have room for 3 more cells downwards
            if (i + 3 < rows && 
                board[i+1][j] == sym && 
                board[i+2][j] == sym && 
                board[i+3][j] == sym) {
                return true;
            }

            // 3. Check Diagonal (Down-Right)
            if (i + 3 < rows && j + 3 < columns && 
                board[i+1][j+1] == sym && 
                board[i+2][j+2] == sym && 
                board[i+3][j+3] == sym) {
                return true;
            }

            // 4. Check Anti-Diagonal (Down-Left)
            if (i + 3 < rows && j - 3 >= 0 && 
                board[i+1][j-1] == sym && 
                board[i+2][j-2] == sym && 
                board[i+3][j-3] == sym) {
                return true;
            }
        }
    }
    return false;
}

bool Obstacles_XO_Board::is_draw(Player<char>* player) {
    // Draw if board is full (36 moves including obstacles) and no winner
    return (n_moves >= 36 && !is_win(player));
}

bool Obstacles_XO_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}


//--------------------------------------- UI Implementation

Obstacles_XO_UI::Obstacles_XO_UI() : UI<char>("Welcome to Obstacles Tic-Tac-Toe (6x6)", 3) {}

Player<char>* Obstacles_XO_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
         << " player: " << name << " (" << symbol << ")\n";
    return new Player<char>(name, symbol, type);
}

Move<char>* Obstacles_XO_UI::get_move(Player<char>* player) {
    int x, y;
    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nPlease enter your move x and y (0 to 5): ";
        cin >> x >> y;
    } else {
        // Random Computer Logic
        do {
            x = rand() % 6;
            y = rand() % 6;
        } while (player->get_board_ptr()->get_board_matrix()[x][y] != '.'); 
    }
    return new Move<char>(x, y, player->get_symbol());
}