#include <iostream>
#include <iomanip>
#include <cctype>  // for toupper()
#include <random>
#include <fstream>
#include "../include/Words_XO.h"


using namespace std;

bool Check_word(string W) {
    ifstream file("dic.txt");  // file name

    if (!file.is_open()) {            // check if it opened correctly
        cerr << "Error: cannot open file!\n";
        return 1;
    }

    string line;
    while (getline(file, line)) {   // read line by line
        if (W == line) return true;  // check if word is in the file ( line == W )
    }
    file.close();  // not required but good practice
    return false;
}

Word_XO_Board::Word_XO_Board() : Board(3, 3), counter(0) {
    // Initialize all cells with blank_symbol
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

bool Word_XO_Board::update_board(Move<char>* move) {

    neMove* nm = dynamic_cast<neMove*>(move);
    if (!nm) return false;

    int x = nm->get_x();
    int y = nm->get_y();
    char val = nm->get_value();
    char ch = move->get_symbol();


    // Validate move and apply if valid
    if (!(x < 0 || x >= rows || y < 0 || y >= columns) && board[x][y] == blank_symbol) {

        n_moves++;
        board[x][y] = toupper(val);

        counter++;

        return true;
    }

    return false;
}

bool Word_XO_Board::is_win(Player<char>* player) {
    const char sym = player->get_symbol();

    auto valid = [&](char a, char b, char c) {
        if (a == '.' || b == '.' || c == '.') return false;
        string s;
        s.push_back(a);s.push_back(b);s.push_back(c);
        // see if the word is in the file
        return Check_word(s);
    };

    // Check rows and columns
    for (int i = 0; i < rows; ++i) {
        if ((valid(board[i][0], board[i][1], board[i][2])) ||
            (valid(board[0][i], board[1][i], board[2][i])))
            return true;
    }

    // Check diagonals
    if ((valid(board[0][0], board[1][1], board[2][2])) ||
        (valid(board[0][2], board[1][1], board[2][0])))
        return true;

    return false;
}

bool Word_XO_Board::is_draw(Player<char>* player) {
    return (n_moves == 9 && !is_win(player));
}

bool Word_XO_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

//--------------------------------------- XO_UI Implementation

Word_XO_UI::Word_XO_UI() : UI<char>("Weclome to FCAI Words X-O Game", 3) {}

Player<char>* Word_XO_UI::create_player(string& name, char symbol, PlayerType type) {
    // Create player based on type
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

Move<char>* Word_XO_UI::get_move(Player<char>* player) {
    int x, y;
    char c;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nPlease enter your move x and y (0 to 2) and cahracter:";
        cin >> x >> y >> c;
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        x = rand() % player->get_board_ptr()->get_rows();
        y = rand() % player->get_board_ptr()->get_columns();
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> dist(0, 25);
        c = 'A' + dist(gen);

    }
    return new neMove(x, y, player->get_symbol() , c);
}

