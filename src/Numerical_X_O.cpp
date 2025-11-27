#include <iostream>
#include <iomanip>
#include <map>
#include <cctype>  // for toupper()
#include "../include/Numerical_X_O.h"
#include <bits/stdc++.h>
using namespace std;

map<char,int> X,O;
X['1'] = 1;
X['3'] = 1;
X['5'] = 1;
X['7'] = 1;
X['9'] = 1;
///////////////
O['2'] = 1;
O['4'] = 1;
O['6'] = 1;
O['8'] = 1;
Numerical_X_O_Board::Numerical_X_O_Board() : Board(3, 3), counter(0) {
    // Initialize all cells with blank_symbol
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

bool Numerical_X_O_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    int val = move->get_value();
    char mark = move->get_symbol();

    if (!(x < 0 || x >= rows || y < 0 || y >= columns) && (board[x][y] == blank_symbol)) {

        if(mark == 'X' && X[(val+'0')] == 1) {
            X[(val+'0')] = 2;
        }
        else if(mark == 'O' && O[(val+'0')] == 1) {
            O[(val+'0')] == 2;
        }
        else return false;
        n_moves++;
        board[x][y] = val;
        counter++;
        return true;
    }
    return false;
}

bool Numerical_X_O_Board::is_win(Player<char>* player) {
    const char sym = player->get_symbol();

    auto all_equal = [&](char a, char b, char c) {
        return (a-'0')+(b-'0')+(c-'0') == 15;
        };

    // Check rows and columns
    for (int i = 0; i < rows; ++i) {
        if ((all_equal(board[i][0], board[i][1], board[i][2])) ||
            (all_equal(board[0][i], board[1][i], board[2][i])))
            return true;
    }

    // Check diagonals
    if ((all_equal(board[0][0], board[1][1], board[2][2])) ||
        (all_equal(board[0][2], board[1][1], board[2][0])))
        return true;

    return false;
}

bool Numerical_X_O_Board::is_draw(Player<char>* player) {
    return (n_moves == 9 && !is_win(player));
}

bool Numerical_X_O_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

//--------------------------------------- XO_UI Implementation

Numerical_XO_UI::Numerical_XO_UI() : UI<char>("Weclome to FCAI X-O Game", 3) {}

Player<char>* Numerical_XO_UI::create_player(string& name, char symbol, PlayerType type) {
    // Create player based on type
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

Move<char>* Numerical_XO_UI::get_move(Player<char>* player) {
    int x, y,val;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nPlease enter your move x and y (0 to 2) and value: ";
        cin >> x >> y >> val;
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        x = rand() % player->get_board_ptr()->get_rows();
        y = rand() % player->get_board_ptr()->get_columns();
        random_device rd;
        mt19937 gen(rd());

        int L = 1, R = 9;
        if(player->get_symbol() == 'X') {
            uniform_int_distribution<int> odd_dist(L/2, (R-1)/2);
            val = odd_dist(gen) * 2 + 1;
        }
        else {
            uniform_int_distribution<int> even_dist((L+1)/2, R/2);
            val = even_dist(gen) * 2;
        }

    }
    return new nMove<char>(x, y, player->get_symbol(), val);
}

