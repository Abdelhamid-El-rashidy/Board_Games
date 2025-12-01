#include "../include/SUS_Classes.h"
#include <bits/stdc++.h>

using namespace std;

SUS_Board::SUS_Board() : Board(3, 3) {
    // Initialize all cells with blank_symbol
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

bool SUS_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    // Validate move and apply if valid
    if (!(x < 0 || x >= rows || y < 0 || y >= columns) &&
        (board[x][y] == blank_symbol || mark == 0)) {

        if (mark == 0) { // Undo move
            n_moves--;
            board[x][y] = blank_symbol;
        }
        else {         // Apply move
            n_moves++;
            board[x][y] = toupper(mark);
        }
        return true;
        }
    return false;
}

bool SUS_Board::is_win(Player<char>* player) {

    const char sym = player->get_symbol();

    int score = 0;

    auto isSUS = [&](char a, char b, char c) {
        return (a == 'S' && b == 'U' && c == 'S');
    };

    // Check rows
    for (int i = 0; i < 3; i++) {
        if (isSUS(board[i][0], board[i][1], board[i][2])) score++;
    }

    // Check columns
    for (int j = 0; j < 3; j++) {
        if (isSUS(board[0][j], board[1][j], board[2][j])) score++;
    }

    // Check main diagonal
    if (isSUS(board[0][0], board[1][1], board[2][2])) score++;

    // Check anti-diagonal
    if (isSUS(board[0][2], board[1][1], board[2][0])) score++;

    if (score % 2 == 1) return true;
    return false;

}

bool SUS_Board::is_draw(Player<char>* player) {
    return (n_moves == 9 && !is_win(player));
}

bool SUS_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player) ;
}



// ---------------------- UI ----------------------

SUS_UI::SUS_UI() : UI<char>("Welcome to FCAI S-U-S Game by Adham", 3) {}

Player<char>** SUS_UI::setup_players() {
    Player<char>** p = new Player<char>*[2];

    string n1, n2;
    cout << "Enter Player 1 name (S): ";
    getline(cin >> ws, n1);
    p[0] = create_player(n1, 'S', get_player_type_choice("Player1", {"Human", "Computer"}));

    cout << "Enter Player 2 name (U): ";
    getline(cin >> ws, n2);
    p[1] = create_player(n2, 'U', get_player_type_choice("Player2", {"Human", "Computer"}));

    return p;
}

Player<char>* SUS_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}


Move<char>* SUS_UI::get_move(Player<char>* player) {
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
