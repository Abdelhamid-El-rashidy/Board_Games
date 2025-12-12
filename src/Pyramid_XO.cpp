//
// Created by abdelhamid on 11/30/25.
//

#include "../include/Pyramid_XO.h"
#include "include/BoardGame_Classes.h"
#include "include/Smart_Player.h"

using namespace std;

//=======Pyramid XO Board Implementation==================

Pyramid_XO_Board::Pyramid_XO_Board() : Board<char>(3,5) {
    int distant[3] = {0,1,2}; // range for each row to build pyramid shape
    for (int i = 0; i<rows; i++) {
        for (int j = 0; j<columns; j++) {
            if (abs(j-2)<=distant[i]) board[i][j] = blank_symbol;
            else board[i][j] = '#';
        }
    }
}


bool Pyramid_XO_Board::update_board(Move<char> *move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    // Validate move and apply if valid
    if (!(x < 0 || x >= rows || y < 0 || y >= columns) &&
    (board[x][y] == blank_symbol)) {

        n_moves++;
        board[x][y] = mark;
        return true;
    }
    return false;
}

bool Pyramid_XO_Board::is_win(Player<char> *player) {
    const char sym = player->get_symbol();

    auto all_equal = [&](char a, char b, char c) {
        return a == b && b == c && a != blank_symbol;
    };

    // Check Column (3*5 pyramid has only one column possible column of size 3)
    if (all_equal(board[0][2],board[1][2],board[2][2]) && board[0][2]==sym ) return true;

    // Check 2nd row
    if (all_equal(board[1][1],board[1][2],board[1][3]) && board[1][1]==sym ) return true;

    // Check 3rd row
    if (all_equal(board[2][0],board[2][1],board[2][2]) && board[2][2]==sym ) return true;
    if (all_equal(board[2][1],board[2][2],board[2][3]) && board[2][2]==sym ) return true;
    if (all_equal(board[2][2],board[2][3],board[2][4]) && board[2][2]==sym ) return true;



    // Check diagonals of pyramid (has only two diagonals)
    if ((all_equal(board[0][2], board[1][1], board[2][0]) && board[0][2] == sym) ||
        (all_equal(board[0][2], board[1][3], board[2][4]) && board[0][2] == sym))
        return true;

    return false;
}

bool Pyramid_XO_Board::is_draw(Player<char>* player) {
    return (n_moves == 9 && !is_win(player));
}

bool Pyramid_XO_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

//=========Pyramid UI Implementation =========


Pyramid_XO_UI::Pyramid_XO_UI() : UI<char>("Weclome to Pyramid Tic Tac Toe", 3) {

}


Player<char>* Pyramid_XO_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
         << " player: " << name << " (" << symbol << ")\n";

    if (type == PlayerType::COMPUTER)
        return new smartPlayer<char>(name, symbol, type);   // create smartPlayer

    return new Player<char>(name, symbol, type);            // human is regular player
}


Move<char>* Pyramid_XO_UI::get_move(Player<char>* player) {
    int x, y;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nPlease enter your move x and y (0 to 2): ";
        cin >> x >> y;
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        auto smart_player = dynamic_cast<smartPlayer<char>*>(player);
        auto move = smart_player->calculateMove();
        x = move.first;
        y = move.second;
    }
    return new Move<char>(x, y, player->get_symbol());
}

Player<char> **Pyramid_XO_UI::setup_players() {
    Player<char>** players = new Player<char>*[2];
    vector<string> type_options = { "Human", "Computer" };

    string nameX = get_player_name("Player X");
    PlayerType typeX = get_player_type_choice("Player X", type_options);
    players[0] = create_player(nameX, static_cast<char>('X'), typeX);

    string nameO = get_player_name("Player O");
    PlayerType typeO = get_player_type_choice("Player O", type_options);
    players[1] = create_player(nameO, static_cast<char>('O'), typeO);

    return players;
}



