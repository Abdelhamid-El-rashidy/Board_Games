#include <iomanip>
#include <cctype>  // for toupper()
#include "../include/X_O_5x5.h"

using namespace std;

X_O_5x5_Board::X_O_5x5_Board() : Board(5, 5) , counter(0){
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

bool X_O_5x5_Board::update_board(Move<char> *move) {
    int x = (*move).get_x();
    int y = (*move).get_y();
    int sym = (*move).get_symbol();

    if (!(x < 0 || x >= rows || y < 0 || y >= columns) &&
        (board[x][y] == blank_symbol || sym == 0)) {

        if (sym == 0) { // Undo move
            n_moves--;
            board[x][y] = blank_symbol;
        }
        else {         // Apply move
            n_moves++;
            board[x][y] = toupper(sym);
        }
        counter++;

        return true;
        }
    return false;
}

bool X_O_5x5_Board::is_win(Player<char>* player) {
    if (n_moves != 24) return false;
    const char sym = (*player).get_symbol();
    const char opponent = (player->get_symbol() == 'X') ? 'O' : 'X';
    int cnt = 0,cnt2 = 0;
    auto all_equal = [&](char a, char b, char c) {
        return a == b && b == c && a == sym;
    };

    // Rows
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns - 2; j++) {
            if (all_equal(board[i][j], board[i][j+1], board[i][j+2]))
                cnt++;
        }
    }

    // Columns
    for (int j = 0; j < columns; j++) {
        for (int i = 0; i < rows - 2; i++) {
            if (all_equal(board[i][j], board[i+1][j], board[i+2][j]))
                cnt++;
        }
    }

    // Diagonal
    for (int i = 0; i < rows - 2; i++) {
        for (int j = 0; j < columns - 2; j++) {
            if (all_equal(board[i][j], board[i+1][j+1], board[i+2][j+2]))
                cnt++;
        }
    }

    // Diagonal
    for (int i = 0; i < rows - 2; i++) {
        for (int j = 2; j < columns; j++) {
            if (all_equal(board[i][j], board[i+1][j-1], board[i+2][j-2]))
                cnt++;
        }
    }

    // for opponent --------------------------------------------
    auto all_equal2 = [&](char a, char b, char c) {
        return a == b && b == c && a == opponent;
    };

    // Rows
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns - 2; j++) {
            if (all_equal2(board[i][j], board[i][j+1], board[i][j+2]))
                cnt2++;
        }
    }

    // Columns
    for (int j = 0; j < columns; j++) {
        for (int i = 0; i < rows - 2; i++) {
            if (all_equal2(board[i][j], board[i+1][j], board[i+2][j]))
                cnt2++;
        }
    }

    // Diagonal
    for (int i = 0; i < rows - 2; i++) {
        for (int j = 0; j < columns - 2; j++) {
            if (all_equal2(board[i][j], board[i+1][j+1], board[i+2][j+2]))
                cnt2++;
        }
    }

    // Diagonal
    for (int i = 0; i < rows - 2; i++) {
        for (int j = 2; j < columns; j++) {
            if (all_equal2(board[i][j], board[i+1][j-1], board[i+2][j-2]))
                cnt2++;
        }
    }
    ///---------------------------------------------------------------

    if (cnt > cnt2) return 1;
    else return 0;
}

bool X_O_5x5_Board::is_lose(Player<char>* player) {
    return (n_moves == 24 && !is_win(player));
}

bool X_O_5x5_Board::is_draw(Player<char>* player) {
    return (n_moves == 24);
}

bool X_O_5x5_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

X_O_5x5_UI::X_O_5x5_UI() : UI<char>("Weclome to FCAI X-O 5X5 Game by Hazem", 3) {}

Player<char>* X_O_5x5_UI::create_player(string& name, char symbol, PlayerType type) {
    // Create player based on type
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

Move<char>* X_O_5x5_UI::get_move(Player<char>* player) {
    int x, y;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nPlease enter your move x and y (0 to 4): ";
        cin >> x >> y;
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        x = rand() % player->get_board_ptr()->get_rows();
        y = rand() % player->get_board_ptr()->get_columns();
    }
    return new Move<char>(x, y, player->get_symbol());
}