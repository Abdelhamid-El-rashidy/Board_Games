//
// Created by abdelhamid on 11/26/25.
//

#include <bits/stdc++.h>
#include "../include/Four_Four_XO.h"


Four_Four_XO_Board::Four_Four_XO_Board(): Board(4, 4) {
    // Initialize all cells with blank_symbol
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;

    for (int i = 0;i<4;i+=2) {
        board[0][i] = 'X'; board[0][i+1] = 'O';
        board[3][i] = 'O'; board[3][i+1] = 'X';
    }
}


bool Four_Four_XO_Board::update_board(Move<char> *move) {

    dualMove* dmove = dynamic_cast<dualMove*>(move);
    if (!dmove) return false;

    int x = dmove->get_x();
    int y = dmove->get_y();
    int dy = dmove->get_dy();
    int dx = dmove->get_dx();
    char symbol = dmove->get_symbol();

    if (x<0 || x>=rows || y<0 || y>=columns || board[x][y] != symbol || abs(dy+dx)!=1 ) return false;
    if (x+dx<0 || x+dx>=rows || y+dy<0 || y+dy>=columns || board[x+dx][y+dy] != blank_symbol) return false;

    n_moves++;
    board[x][y] = blank_symbol;
    board[x+dx][y+dy] = symbol;
    return true;
}

bool Four_Four_XO_Board::is_win(Player<char>* player) {
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

bool Four_Four_XO_Board::is_draw(Player<char>* player) {
    return (n_moves == 16 && !is_win(player));
}

bool Four_Four_XO_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}


//=========Four-Four Tic Tac Toe UI Implementation =========

Move<char> *Four_Four_XO_UI::get_move(Player<char> *player) {
    int x,y,dx,dy;
    int dir;
    vector<pair<int,int>> dirs = {{1,0}, {0,1}, {-1,0}, {0,-1}}; // Up, Right, Down , Left


    if (player->get_type() == PlayerType::HUMAN) {
        cout << "Please enter the coordinates of a one of your pieces to move (0 to 3): ";
        cin >> x >> y;
        cout << "Which direction you want to move your piece? (1 -> 'U', 2 -> 'R', 3 -> 'D', 4 -> 'L'): ";
        cin >> dir;
        dir--;
        while (dir<0||dir>3) {
            cout << "Enter Valid direction: (1 -> 'U', 2 -> 'R', 3 -> 'D', 4 -> 'L'): ";
            cin >> dir;
            dir--;
        }
        dx = dirs[dir].first;
        dy = dirs[dir].second;
    } else if (player->get_type() == PlayerType::COMPUTER) {
        x = rand() % player->get_board_ptr()->get_rows();
        y = rand() % player->get_board_ptr()->get_columns();

        random_device rd;
        mt19937 gen(rd());
        int L = 0, R = 3;
        uniform_int_distribution<int> normal(L, R);
        int num = normal(gen);
        dx = dirs[num].first;
        dy = dirs[num].second;
    }

    return new dualMove(x,y,player->get_symbol(),dy,dx);

}


