#include <bits/stdc++.h>
#include "../include/Diamond_Tic_Tac_Toe.h"
#include "../include/Smart_Player.h"


using namespace std;

//--------------------------------------- Board Implementation

Diamond_Tic_Tac_Toe_Board::Diamond_Tic_Tac_Toe_Board() : Board<char>(7, 7) {
    // Logic to draw the Diamond shape on a 7x7 grid.
    // The center is at (3, 3). The Manhattan distance radius is 3.
    // Equation: abs(row - 3) + abs(col - 3) <= 3

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (abs(i - 3) + abs(j - 3) <= 3) {
                board[i][j] = blank_symbol; // Inside the Diamond (Valid cell)
            } else {
                board[i][j] = '#'; // Outside the Diamond (Invalid/Decor)
            }
        }
    }
}

bool Diamond_Tic_Tac_Toe_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    // 1. Check boundaries.
    // 2. Check if the cell is valid (not '#').
    // 3. Allow move if cell is empty OR if mark is 0 (Undo operation).
    if (x >= 0 && x < rows && y >= 0 && y < columns &&
        board[x][y] != '#' &&
        (board[x][y] == blank_symbol || mark == 0)) {

        if (mark == 0) { // Undo move
            n_moves--;
            board[x][y] = blank_symbol;
        } else {         // Apply move
            n_moves++;
            board[x][y] = toupper(mark);
        }
        return true;
    }
    return false;
}

bool Diamond_Tic_Tac_Toe_Board::is_win(Player<char>* player) {
    char sym = player->get_symbol();

    // Variables to store the maximum consecutive sequence found in each direction
    int max_row = 0;   // Horizontal
    int max_col = 0;   // Vertical
    int max_diag1 = 0; // Main Diagonal (Top-Left to Bottom-Right)
    int max_diag2 = 0; // Anti-Diagonal (Top-Right to Bottom-Left)

    // Iterate over every cell in the board
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {

            // If the cell does not belong to the current player, skip it
            if (board[i][j] != sym) continue;

            // 1. Check Horizontal (Row)
            int current_row = 0;
            for (int k = 0; k < 7; k++) {
                if (j + k < columns && board[i][j + k] == sym) current_row++;
                else break;
            }
            if (current_row > max_row) max_row = current_row;

            // 2. Check Vertical (Column)
            int current_col = 0;
            for (int k = 0; k < 7; k++) {
                if (i + k < rows && board[i + k][j] == sym) current_col++;
                else break;
            }
            if (current_col > max_col) max_col = current_col;

            // 3. Check Main Diagonal
            int current_diag1 = 0;
            for (int k = 0; k < 7; k++) {
                if (i + k < rows && j + k < columns && board[i + k][j + k] == sym) current_diag1++;
                else break;
            }
            if (current_diag1 > max_diag1) max_diag1 = current_diag1;

            // 4. Check Anti-Diagonal
            int current_diag2 = 0;
            for (int k = 0; k < 7; k++) {
                if (i + k < rows && j - k >= 0 && board[i + k][j - k] == sym) current_diag2++;
                else break;
            }
            if (current_diag2 > max_diag2) max_diag2 = current_diag2;
        }
    }

    // Win Condition: One line of length >= 4 AND another line of length >= 3
    // The two lines must be in DIFFERENT directions.

    // Case 1: Primary line (4) is Horizontal. Secondary (3) must be Vertical or Diagonal.
    if (max_row >= 4 && (max_col >= 3 || max_diag1 >= 3 || max_diag2 >= 3)) return true;

    // Case 2: Primary line (4) is Vertical. Secondary (3) must be Horizontal or Diagonal.
    if (max_col >= 4 && (max_row >= 3 || max_diag1 >= 3 || max_diag2 >= 3)) return true;

    // Case 3: Primary line (4) is Main Diagonal. Secondary (3) must be Row, Col, or Anti-Diag.
    if (max_diag1 >= 4 && (max_row >= 3 || max_col >= 3 || max_diag2 >= 3)) return true;

    // Case 4: Primary line (4) is Anti-Diagonal. Secondary (3) must be Row, Col, or Main Diag.
    if (max_diag2 >= 4 && (max_row >= 3 || max_col >= 3 || max_diag1 >= 3)) return true;

    return false;
}

bool Diamond_Tic_Tac_Toe_Board::is_draw(Player<char>* player) {
    return (n_moves == 25 && !is_win(player));
}

bool Diamond_Tic_Tac_Toe_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}


char DiamondSmartPlayer::get_opponent_symbol() const {
    return (this->symbol == 'X') ? 'O' : 'X';
}


bool DiamondSmartPlayer::is_win(const vector<vector<char>>& b, char symbol, int N) const {
    int rows = b.size();
    int cols = b[0].size();

    int max_row = 0;   // Horizontal
    int max_col = 0;   // Vertical
    int max_diag1 = 0; // Main Diagonal
    int max_diag2 = 0; // Anti-Diagonal

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (b[i][j] != symbol) continue;

            // 1. Horizontal
            int current = 0;
            for (int k = 0; k < 7; k++) {
                if (j + k < cols && b[i][j + k] == symbol) current++;
                else break;
            }
            max_row = max(max_row, current);

            // 2. Vertical
            current = 0;
            for (int k = 0; k < 7; k++) {
                if (i + k < rows && b[i + k][j] == symbol) current++;
                else break;
            }
            max_col = max(max_col, current);

            // 3. Diagonal 1
            current = 0;
            for (int k = 0; k < 7; k++) {
                if (i + k < rows && j + k < cols && b[i + k][j + k] == symbol) current++;
                else break;
            }
            max_diag1 = max(max_diag1, current);

            // 4. Diagonal 2
            current = 0;
            for (int k = 0; k < 7; k++) {
                if (i + k < rows && j - k >= 0 && b[i + k][j - k] == symbol) current++;
                else break;
            }
            max_diag2 = max(max_diag2, current);
        }
    }

    if (max_row >= 4 && (max_col >= 3 || max_diag1 >= 3 || max_diag2 >= 3)) return true;
    if (max_col >= 4 && (max_row >= 3 || max_diag1 >= 3 || max_diag2 >= 3)) return true;
    if (max_diag1 >= 4 && (max_row >= 3 || max_col >= 3 || max_diag2 >= 3)) return true;
    if (max_diag2 >= 4 && (max_row >= 3 || max_col >= 3 || max_diag1 >= 3)) return true;

    return false;
}


int DiamondSmartPlayer::evaluate_board(const vector<vector<char>>& b, char ai, char opp, int N) const {


    int position_weights[7][7] = {
        {0, 0, 0, 1, 0, 0, 0},
        {0, 0, 2, 3, 2, 0, 0},
        {0, 2, 4, 5, 4, 2, 0},
        {1, 3, 5, 8, 5, 3, 1},
        {0, 2, 4, 5, 4, 2, 0},
        {0, 0, 2, 3, 2, 0, 0},
        {0, 0, 0, 1, 0, 0, 0}
    };

    int score = 0;
    int rows = b.size();
    int cols = b[0].size();

    for(int i=0; i<rows; i++) {
        for(int j=0; j<cols; j++) {
            if (b[i][j] == ai) score += position_weights[i][j];
            else if (b[i][j] == opp) score -= position_weights[i][j];
        }
    }


    auto analyze_player = [&](char symbol) -> int {
        int max_h = 0, max_v = 0, max_d1 = 0, max_d2 = 0;


        for(int i=0; i<rows; i++) {
            for(int j=0; j<cols; j++) {
                if(b[i][j] != symbol) continue;

                // Horizontal
                int cnt = 0;
                for(int k=0; k<7; k++) { if(j+k<cols && b[i][j+k]==symbol) cnt++; else break; }
                max_h = max(max_h, cnt);

                // Vertical
                cnt = 0;
                for(int k=0; k<7; k++) { if(i+k<rows && b[i+k][j]==symbol) cnt++; else break; }
                max_v = max(max_v, cnt);

                // Diag 1
                cnt = 0;
                for(int k=0; k<7; k++) { if(i+k<rows && j+k<cols && b[i+k][j+k]==symbol) cnt++; else break; }
                max_d1 = max(max_d1, cnt);

                // Diag 2
                cnt = 0;
                for(int k=0; k<7; k++) { if(i+k<rows && j-k>=0 && b[i+k][j-k]==symbol) cnt++; else break; }
                max_d2 = max(max_d2, cnt);
            }
        }

        int p_score = 0;


        vector<int> lines = {max_h, max_v, max_d1, max_d2};
        sort(lines.rbegin(), lines.rend());

        int best = lines[0];
        int second_best = lines[1];



        if (best >= 4 && second_best >= 3) return 100000;


        if (best >= 4) p_score += 2000 + (second_best * 100);

        if (best == 3) p_score += 500 + (second_best * 50);

        if (best == 2) p_score += 50;

        return p_score;
    };

    score += analyze_player(ai);
    score -= (analyze_player(opp) * 1.5);

    return score;
}

pair<int, int> DiamondSmartPlayer::calculateMove() {
    Board<char>* board_ptr = this->get_board_ptr();
    auto tempBoard = board_ptr->get_board_matrix();

    char ai = this->get_symbol();
    char opp = get_opponent_symbol();

    int bestVal = numeric_limits<int>::min();
    int bestX = -1, bestY = -1;

    int maxDepth = 3;

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            if (tempBoard[i][j] == '.' && abs(i - 3) + abs(j - 3) <= 3) {

                tempBoard[i][j] = ai;


                int moveVal = minimax(tempBoard, false, ai, opp,
                                      numeric_limits<int>::min(),
                                      numeric_limits<int>::max(),
                                      maxDepth);

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




//--------------------------------------- UI Implementation

Diamond_Tic_Tac_Toe_UI::Diamond_Tic_Tac_Toe_UI() : UI<char>("Welcome to 7x7 Diamond Tic-Tac-Toe Game", 3) {}

Player<char>* Diamond_Tic_Tac_Toe_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
         << " player: " << name << " (" << symbol << ")\n";

    if (type == PlayerType::COMPUTER)
        return new smartPlayer<char>(name, symbol, type);   // create smartPlayer

    return new Player<char>(name, symbol, type);            // human is regular player
}

Move<char>* Diamond_Tic_Tac_Toe_UI::get_move(Player<char>* player) {
    int x, y;
    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nPlease enter your move x and y (0 to 6): ";
        cin >> x >> y;
    }else if (player->get_type() == PlayerType::COMPUTER) {
        auto smart_player = dynamic_cast<smartPlayer<char>*>(player);
        auto move = smart_player->calculateMove();
        x = move.first;
        y = move.second;
    }
    return new Move<char>(x, y, player->get_symbol());
}
Player<char> **Diamond_Tic_Tac_Toe_UI::setup_players() {
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