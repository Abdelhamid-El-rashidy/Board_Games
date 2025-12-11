#include "../include/Four-in-a-row.h"
#include "../include/Smart_Player.h"
#include <bits/stdc++.h>

using namespace std;

class smartPlayer2 : public smartPlayer<char> {
    using smartPlayer<char>::smartPlayer;
public:
    smartPlayer2(string n, char s, PlayerType t)
            : smartPlayer<char>(n, s, t) {}

    int evaluate_board(const vector<vector<char>>& b, char ai, char opp, int N = 4) const override {
        return smartPlayer<char>::evaluate_board(b, ai, opp, N);
    }

    bool is_win(const vector<vector<char>>& b, char symbol, int N = 4) const override {
        return smartPlayer<char>::is_win(b, symbol, N);
    }

    virtual bool is_draw(const vector<vector<char>>& b, int N = 4) const {
        return smartPlayer<char>::is_draw(b, N);
    }

    int minimax(vector<vector<char>>& b, bool maximizingPlayer, char ai, char opp, int alpha, int beta, int depth, int N = 4) {
        if (is_win(b, ai, N)) return 1000 + depth; // prefer faster win
        if (is_win(b, opp, N)) return -1000 - depth; // prefer slower loss
        if (is_draw(b, N)) return 0;
        if (depth == 0) return evaluate_board(b, ai, opp, N);

        int rows = b.size();
        int cols = b[0].size();

        if (maximizingPlayer) {
            int best = numeric_limits<int>::min();
            for (int j = 0; j < cols; j++)
                for (int i = rows-1; i >= 0; i--)
                    if (b[i][j] == '.') {
                        b[i][j] = ai;
                        int val = minimax(b, false, ai, opp, alpha, beta, depth - 1, N);
                        b[i][j] = '.';
                        best = max(best, val);
                        alpha = max(alpha, best);
                        if (beta <= alpha) return best;
                    }
            return best;
        } else {
            int best = numeric_limits<int>::max();
            for (int j = 0; j < cols; j++)
                for (int i = rows-1; i >= 0; i--)
                    if (b[i][j] == '.') {
                        b[i][j] = opp;
                        int val = minimax(b, true, ai, opp, alpha, beta, depth - 1, N);
                        b[i][j] = '.';
                        best = min(best, val);
                        beta = min(beta, best);
                        if (beta <= alpha) return best;
                    }
            return best;
        }
    }

    pair<int,int> calculateMove()  {
        Board<char>* board_ptr = this->get_board_ptr();
        if (!board_ptr) throw runtime_error("Board not assigned to player!");

        auto tempBoard = board_ptr->get_board_matrix();
        int rows = tempBoard.size();
        int cols = tempBoard[0].size();

        int bestVal = numeric_limits<int>::min();
        int bestX = -1, bestY = -1;

        char ai = this->get_symbol();
        char opp = get_opponent_symbol();

        int maxDepth = 2; // tweak: higher depth = stronger AI but slower

        for (int j = 0; j < cols; j++)
            for (int i = rows-1; i >= 0; i--)
                if (tempBoard[i][j] == '.') {
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

        return {bestX, bestY};
    }

};


Four_in_a_row_Board::Four_in_a_row_Board() : Board(6, 7) {
    // Initialize all cells with blank_symbol
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

bool Four_in_a_row_Board::update_board(Move<char>* move) {
    int col = move->get_y();
    char sym = move->get_symbol();

    if (col < 0 || col >= columns)
        return false;

    // drop piece to low position
    for (int r = rows - 1; r >= 0; r--) {
        if (board[r][col] == blank_symbol) {
            board[r][col] = sym;
            n_moves++;
            return true;
        }
    }
    return false; // column full
}


bool Four_in_a_row_Board::is_win(Player<char>* player) {

    char sym = player->get_symbol();

    //Rows
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns - 3; j++) {
            if (board[i][j] == sym && board[i][j+1] == sym && board[i][j+2] == sym && board[i][j+3] == sym)
                return true;
        }
    }

    // Columns
    for (int j = 0; j < columns; j++) {
        for (int i = 0; i < rows - 3; i++) {
            if (board[i][j] == sym && board[i+1][j] == sym && board[i+2][j] == sym && board[i+3][j] == sym)
                return true;
        }
    }

    // Main diagonal
    for (int i = 0; i < rows - 3; i++) {
        for (int j = 0; j < columns - 3; j++) {
            if (board[i][j] == sym && board[i+1][j+1] == sym && board[i+2][j+2] == sym && board[i+3][j+3] == sym)
                return true;
        }
    }

    //  Anti diagonal
    for (int i = 0; i < rows - 3; i++) {
        for (int j = 3; j < columns; j++) {
            if (board[i][j] == sym && board[i+1][j-1] == sym && board[i+2][j-2] == sym && board[i+3][j-3] == sym)
                return true;
        }
    }

    return false;

}

bool Four_in_a_row_Board::is_draw(Player<char> *player) {
    return (n_moves == rows * columns && !is_win(player));
}


bool Four_in_a_row_Board::game_is_over(Player<char> *player) {
    return  (is_win(player) || is_draw(player));
}


Four_in_a_row_UI::Four_in_a_row_UI() : UI<char>("Weclome to FCAI Four_in_a_row Game by Adham", 3) {}

Player<char>* Four_in_a_row_UI::create_player(string& name, char symbol, PlayerType type) {
    // Create player based on type
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    if (type == PlayerType::HUMAN) return new Player<char>(name, symbol, type);
    return new smartPlayer<char>(name, symbol, type);
}

Move<char>* Four_in_a_row_UI::get_move(Player<char>* player) {
    int y;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nPlease enter your column (0 to 6): ";
        cin >> y ;
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        auto smart_player = dynamic_cast<smartPlayer<char>*>(player);
        auto move = smart_player->calculateMove();
        // x = move.first;
        y = move.second;
    }
    return new Move<char>(0,y, player->get_symbol());
}

