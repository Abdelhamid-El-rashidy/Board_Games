#include "../include/Four-in-a-row.h"
#include <bits/stdc++.h>

using namespace std;

Four_in_a_row_Board::Four_in_a_row_Board() : Board(6, 7) {
    // Initialize all cells with blank_symbol
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

bool Four_in_a_row_Board::update_board(Move<char>* move) {
    int col = move->get_y();
    char sym = move->get_symbol();

    if (col < 0 || col >= columns)
        return false;

    // drop piece to low position
    for (int r = rows - 1; r >= 0; r--) {
        if (board[r][col] == blank_symbol) {
            board[r][col] = sym;
            n_moves++;
            return true;
        }
    }
    return false; // column full
}


bool Four_in_a_row_Board::is_win(Player<char>* player) {

    char sym = player->get_symbol();

    //Rows
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns - 3; j++) {
            if (board[i][j] == sym && board[i][j+1] == sym && board[i][j+2] == sym && board[i][j+3] == sym)
                return true;
        }
    }

    // Columns
    for (int j = 0; j < columns; j++) {
        for (int i = 0; i < rows - 3; i++) {
            if (board[i][j] == sym && board[i+1][j] == sym && board[i+2][j] == sym && board[i+3][j] == sym)
                return true;
        }
    }

    // Main diagonal
    for (int i = 0; i < rows - 3; i++) {
        for (int j = 0; j < columns - 3; j++) {
            if (board[i][j] == sym && board[i+1][j+1] == sym && board[i+2][j+2] == sym && board[i+3][j+3] == sym)
                return true;
        }
    }

    //  Anti diagonal
    for (int i = 0; i < rows - 3; i++) {
        for (int j = 3; j < columns; j++) {
            if (board[i][j] == sym && board[i+1][j-1] == sym && board[i+2][j-2] == sym && board[i+3][j-3] == sym)
                return true;
        }
    }

    return false;

}

bool Four_in_a_row_Board::is_draw(Player<char> *player) {
    return (n_moves == rows * columns && !is_win(player));
}


bool Four_in_a_row_Board::game_is_over(Player<char> *player) {
    return  (is_win(player) || is_draw(player));
}


Four_in_a_row_UI::Four_in_a_row_UI() : UI<char>("Weclome to FCAI Four_in_a_row Game by Adham", 3) {}

Player<char>* Four_in_a_row_UI::create_player(string& name, char symbol, PlayerType type) {
    // Create player based on type
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

Move<char>* Four_in_a_row_UI::get_move(Player<char>* player) {
    int y;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nPlease enter your column (0 to 6): ";
        cin >> y ;
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        // x = rand() % player->get_board_ptr()->get_rows();
         y = rand() % player->get_board_ptr()->get_columns();
    }
    return new Move<char>(0,y, player->get_symbol());
}

