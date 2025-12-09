



#include <bits/stdc++.h>
#include "../include/Misere_Tic_Tac_Toe.h"
#include "include/Smart_Player.h"

using namespace std;

//--------------------------------------- X_O_Board Implementation

Misere_Tic_Tac_Toe_board::Misere_Tic_Tac_Toe_board() : Board(3, 3){
    // Initialize all cells with blank_symbol
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}


bool Misere_Tic_Tac_Toe_board::update_board(Move<char>* move) {
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
bool Misere_Tic_Tac_Toe_board::is_lose(Player<char>* player) {
    const char sym = player->get_symbol();

    auto all_equal = [&](char a, char b, char c) {
        return a == b && b == c && a != blank_symbol;
    };

    // Check rows and columns
    for (int i = 0; i < rows; ++i) {
        if ((all_equal(board[i][0], board[i][1], board[i][2]) && board[i][0] == sym) ||
            (all_equal(board[0][i], board[1][i], board[2][i]) && board[0][i] == sym))
            return true;
    }

    // Check diagonals
    if ((all_equal(board[0][0], board[1][1], board[2][2]) && board[1][1] == sym) ||
        (all_equal(board[0][2], board[1][1], board[2][0]) && board[1][1] == sym))
        return true;

    return false;
}

bool Misere_Tic_Tac_Toe_board::is_draw(Player<char>* player) {
    return (n_moves == 9 && !is_lose(player));
}

bool Misere_Tic_Tac_Toe_board::game_is_over(Player<char>* player) {
    return is_lose(player) || is_draw(player);
}

char MisereSmartPlayer::get_opponent_symbol() const {
    return (this->symbol == 'X') ? 'O' : 'X';
}

bool MisereSmartPlayer::check_3_in_row(const vector<vector<char>>& b, char symbol) const {
    for (int i = 0; i < 3; i++) {
        if (b[i][0] == symbol && b[i][1] == symbol && b[i][2] == symbol) return true;
        if (b[0][i] == symbol && b[1][i] == symbol && b[2][i] == symbol) return true;
    }
    if (b[0][0] == symbol && b[1][1] == symbol && b[2][2] == symbol) return true;
    if (b[0][2] == symbol && b[1][1] == symbol && b[2][0] == symbol) return true;
    return false;
}

int MisereSmartPlayer::minimax_misere(vector<vector<char>>& b, bool isMaximizing, int depth) {
    char ai = this->symbol;
    char opp = get_opponent_symbol();

    if (check_3_in_row(b, ai)) return -10 + depth;

    if (check_3_in_row(b, opp)) return 10 - depth;


    bool full = true;
    for(auto& r : b) for(auto c : r) if(c == '.') full = false;
    if(full) return 0;

    // 2. Recursion
    if (isMaximizing) {
        int best = -1000;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (b[i][j] == '.') {
                    b[i][j] = ai;
                    best = max(best, minimax_misere(b, false, depth + 1));
                    b[i][j] = '.';
                }
            }
        }
        return best;
    } else {
        int best = 1000;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (b[i][j] == '.') {
                    b[i][j] = opp;
                    best = min(best, minimax_misere(b, true, depth + 1));
                    b[i][j] = '.';
                }
            }
        }
        return best;
    }
}

pair<int, int> MisereSmartPlayer::calculateMove() {
    Board<char>* board_ptr = this->get_board_ptr();
    auto tempBoard = board_ptr->get_board_matrix();

    int bestVal = -1000;
    int bestX = -1, bestY = -1;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (tempBoard[i][j] == '.') {

                tempBoard[i][j] = symbol;

                int moveVal = minimax_misere(tempBoard, false, 0);

                tempBoard[i][j] = '.';

                if (moveVal > bestVal) {
                    bestVal = moveVal;
                    bestX = i;
                    bestY = j;
                }
            }
        }
    }
    return {bestX, bestY};
}

//--------------------------------------- XO_UI Implementation

Misere_Tic_Tac_Toe_UI::Misere_Tic_Tac_Toe_UI() : UI<char>("Weclome to FCAI X-O Game by Dr El-Ramly", 3) {}


Player<char>* Misere_Tic_Tac_Toe_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
         << " player: " << name << " (" << symbol << ")\n";

    if (type == PlayerType::COMPUTER)
        return new smartPlayer<char>(name, symbol, type);   // create smartPlayer

    return new Player<char>(name, symbol, type);            // human is regular player
}


Move<char>* Misere_Tic_Tac_Toe_UI::get_move(Player<char>* player) {
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

Player<char> **Misere_Tic_Tac_Toe_UI::setup_players() {
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

Misere_Tic_Tac_Toe_UI::~Misere_Tic_Tac_Toe_UI() {}
