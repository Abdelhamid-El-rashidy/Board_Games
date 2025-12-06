#include <bits/stdc++.h>
#include "../include/Memory Tic-Tac-Toe.h"

using namespace std ;

//-------------------------------------------------Board------------------------------------------------------------------

MEMORY_TIC_TAC_TOE_Board::MEMORY_TIC_TAC_TOE_Board () : Board(3,3) {
    hidden_board = vector<vector<char>>(3, vector<char>(3, blank_symbol));
}

bool MEMORY_TIC_TAC_TOE_Board::update_board(Move<char> *move) {
    int x = move->get_x();
    int y = move->get_y();
    char sym = move->get_symbol();

    if (x < 0 || x >= rows || y < 0 || y >= columns)
        return false;

    if (hidden_board[x][y] != blank_symbol)
        return false;

    hidden_board[x][y] = sym;

    // visible board stays always blank
    board[x][y] = blank_symbol;

    n_moves++;
    return true;
}

bool MEMORY_TIC_TAC_TOE_Board::is_win(Player<char>* p) {

    char sym = p->get_symbol();

    auto eq = [&](char a, char b, char c){
        return a == sym && b == sym && c == sym;
    };

    // rows
    for (int i = 0; i < rows; ++i)
        if (eq(hidden_board[i][0], hidden_board[i][1], hidden_board[i][2]))
            return true;

    // columns
    for (int j = 0; j < columns; ++j)
        if (eq(hidden_board[0][j], hidden_board[1][j], hidden_board[2][j]))
            return true;

    // diagonals
    if (eq(hidden_board[0][0], hidden_board[1][1], hidden_board[2][2])) return true;
    if (eq(hidden_board[0][2], hidden_board[1][1], hidden_board[2][0])) return true;

    return false;
}
bool MEMORY_TIC_TAC_TOE_Board::is_draw(Player<char> *player) {
    return (n_moves == 9 && !is_win(player));
}

bool MEMORY_TIC_TAC_TOE_Board::game_is_over(Player<char> *player) {
    return is_win(player) || is_draw(player) ;
}


//--------------------------------------UI------------------------------------------------------------------------------

MEMORY_TIC_TAC_TOE_UI::MEMORY_TIC_TAC_TOE_UI() : UI<char>("Weclome to FCAI Memory Tic-Tac-Toe Game by Adham", 3) {}

Player<char>* MEMORY_TIC_TAC_TOE_UI::create_player(string& name, char symbol, PlayerType type) {
    // Create player based on type
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

Move<char>* MEMORY_TIC_TAC_TOE_UI::get_move(Player<char>* player) {
    int x, y;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nPlease enter your move x and y (0 to 2): ";
        cin >> x >> y;
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        x = rand() % player->get_board_ptr()->get_rows();
        y = rand() % player->get_board_ptr()->get_columns();
    }
    return new Move<char>(x, y, player->get_symbol());
}