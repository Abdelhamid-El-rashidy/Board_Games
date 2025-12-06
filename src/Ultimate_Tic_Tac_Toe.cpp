//
// Created by abdelhamid on 12/5/25.
//

#include "../include/Ultimate_Tic_Tac_Toe.h"

//===========main Board Implementation===============
mainBoard::mainBoard() :Board<char>(3,3) {
    // Initialize all cells with blank_symbol
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;

}

bool mainBoard::update_board(Move<char>* move) {
    int x = move->get_x()/3;
    int y = move->get_y()/3;
    char mark = move->get_symbol();

    // Validate move and apply if valid
    if (!(x < 0 || x >= rows || y < 0 || y >= columns) && (board[x][y] == blank_symbol)) {
        board[x][y] = toupper(mark);
        n_moves++;
        return true;
    }
    return false;
}

bool mainBoard::is_win(Player<char>* player) {
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

bool mainBoard::is_draw(Player<char>* player) {
    return (n_moves == 9 && !is_win(player));
}

bool mainBoard::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

//================Ultimate Board Implementation================

Ultimate_XO_Board::Ultimate_XO_Board() : Board<char> (9,9 ) {
    // Initialize all cells with blank_symbol
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

bool Ultimate_XO_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    auto checkboard = [&](int dx, int dy) {
        const char sym = move->get_symbol();

        auto all_equal = [&](char a, char b, char c) {
            return a == b && b == c && a != blank_symbol;
        };

        // Check rows and columns
        for (int i = 0; i < 3; ++i) {
            if ((all_equal(board[i+dx][0+dy], board[i+dx][1+dy], board[i+dx][2+dy]) && board[i+dx][0+dy] == sym) ||
                (all_equal(board[0+dx][i+dy], board[1+dx][i+dy], board[2+dx][i+dy]) && board[0+dx][i+dy] == sym))
                return true;
        }

        // Check diagonals
        if ((all_equal(board[dx+0][dy+0], board[dx+1][dy+1], board[dx+2][dy+2]) && board[dx+1][dy+1] == sym) ||
            (all_equal(board[dx+0][dy+2], board[dx+1][dy+1], board[dx+2][dy+0]) && board[dx+1][dy+1] == sym))
            return true;
    };

    // Validate move and apply if valid
    if (!(x < 0 || x >= rows || y < 0 || y >= columns) && (board[x][y] == blank_symbol)) {
        board[x][y] = toupper(mark);
        n_moves++;
        if (checkboard((x/3) * 3,(y/3) * 3)) mainBoardPtr->update_board(move);
        return true;
    }
    return false;
}

bool Ultimate_XO_Board::is_win(Player<char>* player) {

    if (mainBoardPtr->is_win(player)) return true;

    return false;
}

bool Ultimate_XO_Board::is_draw(Player<char>* player) {
    return (n_moves == 81 && mainBoardPtr->is_draw(player));
}

bool Ultimate_XO_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}



// ---------------------- UI ----------------------

Ultimate_XO_UI::Ultimate_XO_UI() : UI<char>("Welcome to FCAI Ultimate Tic Tac Toe Game by Abdelhamid ElRashidy", 3) {}

Player<char>** Ultimate_XO_UI::setup_players() {
    Player<char>** p = new Player<char>*[2];

    string n1, n2;
    cout << "Enter Player 1 name (X): ";
    getline(cin >> ws, n1);
    p[0] = create_player(n1, 'X', get_player_type_choice("Player1", {"Human", "Computer"}));

    cout << "Enter Player 2 name (O): ";
    getline(cin >> ws, n2);
    p[1] = create_player(n2, 'O', get_player_type_choice("Player2", {"Human", "Computer"}));

    return p;
}

Player<char>* Ultimate_XO_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}


Move<char>* Ultimate_XO_UI::get_move(Player<char>* player) {
    int x, y;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nPlease enter your move x and y (0 to 8): ";
        cin >> x >> y;
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        x = rand() % player->get_board_ptr()->get_rows();
        y = rand() % player->get_board_ptr()->get_columns();
    }
    return new Move<char>(x, y, player->get_symbol());
}

// void Ultimate_XO_UI::display_board_matrix(const vector<vector<char> > &matrix) const {
//     cout << "-------------------Result Board-----------------------" << endl;
//
//     cout << "------------------Main Board---------------------------" << endl;
//     if (matrix.empty() || matrix[0].empty()) return;
//
//     int rows = matrix.size();
//     int cols = matrix[0].size();
//
//     cout << "\n    ";
//     for (int j = 0; j < cols; ++j)
//         cout << setw(cell_width + 1) << j;
//     cout << "\n   " << string((cell_width + 2) * cols, '-') << "\n";
//
//     for (int i = 0; i < rows; ++i) {
//         cout << setw(2) << i << " |";
//         for (int j = 0; j < cols; ++j)
//             cout << setw(cell_width) << matrix[i][j] << " |";
//         cout << "\n   " << string((cell_width + 2) * cols, '-') << "\n";
//     }
//     cout << endl;
// }
