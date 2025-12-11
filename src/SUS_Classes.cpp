#include "../include/SUS_Classes.h"
#include "../include/Smart_Player.h"
#include <bits/stdc++.h>

using namespace std;

const vector<char> LETTERS = {'S','U'};

class SUS_SmartPlayer : public smartPlayer<char> {
    using smartPlayer<char>::smartPlayer;

protected:
    vector<string> dictionary;
    vector<char> possibleLetters;      // only letters that appear in the dictionary
    mutable unsigned long node_counter = 0; // count nodes visited (mutable to update in const funcs)
    const unsigned long MAX_NODES = 50000; // tune this: 200k node budget as example


public:
    SUS_SmartPlayer(string n, char s, PlayerType t, const string& dictFile)
            : smartPlayer<char>(n, s, t)
    {
        loadDictionary(dictFile);
    }

protected:
    void loadDictionary(const string& filename) {
        ifstream in(filename);
        string w;
        unordered_set<char> letters;
        dictionary.push_back("SUS");
        // build possibleLetters vector from set (stable order not required)
        possibleLetters.push_back('S');
        possibleLetters.push_back('U');
        // if dictionary is empty for whatever reason, fall back to full alphabet
        if (possibleLetters.empty()) {
            possibleLetters = LETTERS;
        }
    }


    virtual bool is_win(const vector<vector<char>>& b, char s, int N = 3) const override {
        int R = b.size();
        int C = b[0].size();

        // Check all rows
        for (int i = 0; i < R; i++) {
            string row;
            for (int j = 0; j < C; j++) row += b[i][j];
            if (is_valid_word(row)) return true;
        }

        // Check all columns
        for (int j = 0; j < C; j++) {
            string col;
            for (int i = 0; i < R; i++) col += b[i][j];
            if (is_valid_word(col)) return true;
        }

        // Check TL-BR diagonal
        if (R == C) {
            string diag;
            for (int k = 0; k < R; k++) diag += b[k][k];
            if (is_valid_word(diag)) return true;
        }

        // Check TR-BL diagonal
        if (R == C) {
            string diag;
            for (int k = 0; k < R; k++) diag += b[k][C - 1 - k];
            if (is_valid_word(diag)) return true;
        }

        return false;
    }

    bool is_valid_word(const string& s) const {
        // remove '.' (empty cells)
        string cleaned;
        for (char c : s)
            if (c != '.') cleaned += c;

        if (cleaned.size() < 3) return false;

        return find(dictionary.begin(), dictionary.end(), cleaned) != dictionary.end();
    }

    virtual int evaluate_board(const vector<vector<char>>& b, char ai, char opp, int N = 3) const override {
        // Scoring: more letters forming a prefix of any word = higher score
        int score = 0;

        for (auto& w : dictionary) {
            score += score_line(b, w, ai);
            score -= score_line(b, w, opp);
        }

        return score;
    }

    int score_line(const vector<vector<char>>& b, const string& word, char symbol) const {
        int R = b.size();
        int C = b[0].size();
        int score = 0;

        // Example scoring: +10 for correct letter in position, +3 for empty, -5 otherwise
        for (int i = 0; i < R; i++)
            for (int j = 0; j < C; j++) {
                // Check horizontal word match possibility
                if (j + word.size() <= C) {
                    int s = 0;
                    for (int k = 0; k < word.size(); k++) {
                        if (b[i][j+k] == word[k]) s += 10;
                        else if (b[i][j+k] == '.') s += 3;
                        else { s = -50; break; }
                    }
                    score += s;
                }
            }

        return score;
    }

    int minimax(vector<vector<char>>& b,
            bool maximizingPlayer,
            char ai,
            char opp,
            int alpha,
            int beta,
            int depth,
            int N = 3)
{
    // node accounting
    ++node_counter;
    if (node_counter > MAX_NODES) {
        // abort search early: return heuristic evaluation to let caller use partial result
        return evaluate_board(b, ai, opp, N);
    }

    if (is_win(b, ai, N)) return 1000 + depth; // prefer faster win
    if (is_win(b, opp, N)) return -1000 - depth; // prefer slower loss
    if (is_draw(b, N)) return 0;
    if (depth == 0) return evaluate_board(b, ai, opp, N);

    int rows = b.size();
    int cols = b[0].size();

    if (maximizingPlayer) {
        int best = numeric_limits<int>::min();
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (b[i][j] != '.') continue;
                for (char c : possibleLetters) {
                    b[i][j] = c;
                    int val = minimax(b, false, ai, opp, alpha, beta, depth - 1, N);
                    b[i][j] = '.';
                    if (val > best) best = val;
                    if (best > alpha) alpha = best;
                    if (beta <= alpha) {
                        // pruning - break out early
                        goto OUT_MAX_LOOP;
                    }
                    // early node budget check
                    if (node_counter > MAX_NODES) goto OUT_MAX_LOOP;
                }
            }
        }
OUT_MAX_LOOP:
        return best;
    } else {
        int best = numeric_limits<int>::max();
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (b[i][j] != '.') continue;
                for (char c : possibleLetters) {
                    b[i][j] = c;
                    int val = minimax(b, true, ai, opp, alpha, beta, depth - 1, N);
                    b[i][j] = '.';
                    if (val < best) best = val;
                    if (best < beta) beta = best;
                    if (beta <= alpha) {
                        goto OUT_MIN_LOOP;
                    }
                    if (node_counter > MAX_NODES) goto OUT_MIN_LOOP;
                }
            }
        }
OUT_MIN_LOOP:
        return best;
    }
}

public:
    pair<pair<int,int>,int> calculateMove2()  {
        Board<char>* board_ptr = this->get_board_ptr();
        if (!board_ptr) throw runtime_error("Board not assigned to player!");

        auto tempBoard = board_ptr->get_board_matrix();
        int rows = tempBoard.size();
        int cols = tempBoard[0].size();

        int bestVal = numeric_limits<int>::min();
        int bestX = -1, bestY = -1;
        char bestc = '#';

        char ai = this->get_symbol();
        char opp = get_opponent_symbol();

        // search configuration
        int maxDepth = 1; // can drop to 1 if slow; raise if you add pruning/time limits
        node_counter = 0; // reset before search

        // If possibleLetters empty (shouldn't be), fall back
        if (possibleLetters.empty()) possibleLetters = LETTERS;

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (tempBoard[i][j] != '.') continue;
                for (char c : possibleLetters) {
                    tempBoard[i][j] = c;
                    int moveVal = minimax(tempBoard, false, ai, opp,
                                          numeric_limits<int>::min(),
                                          numeric_limits<int>::max(),
                                          maxDepth);
                    tempBoard[i][j] = '.';
                    if (moveVal > bestVal) {
                        bestVal = moveVal;
                        bestX = i;
                        bestY = j;
                        bestc = c;
                    }
                    // optional: if node budget reached, break entirely
                    if (node_counter > MAX_NODES) break;
                }
                if (node_counter > MAX_NODES) break;
            }
            if (node_counter > MAX_NODES) break;
        }

        // Debug: print nodes visited (remove in production)
        // cerr << "AI nodes visited: " << node_counter << ", bestVal=" << bestVal << "\n";

        return {{bestX, bestY}, bestc};
    }

};


bool Check_SUS(const string& W) {
    if (W == "SUS") return true;
    return false;
}

SUS_Board::SUS_Board() : Board(3, 3) {
    // Initialize all cells with blank_symbol
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

bool SUS_Board::update_board(Move<char>* move) {
    // Convert Move<char>* → ne_Move*
    ne_Move* nm = dynamic_cast<ne_Move*>(move);
    if (!nm) return false;

    int x = nm->get_x();
    int y = nm->get_y();
    char val = nm->get_value();
    char mark = move->get_symbol();

    // Validate move and apply if valid
    if (!(x < 0 || x >= rows || y < 0 || y >= columns) && board[x][y] == blank_symbol) {
        n_moves++;
        board[x][y] = toupper(val);
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


Player<char>* SUS_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    if (type == PlayerType::HUMAN) {
        return new Player<char>(name, symbol, type);
    } else {
        // Construct a wordSmartPlayer and pass the dictionary file path
        // Make sure "dic.txt" path is correct relative to your working dir
        return new SUS_SmartPlayer(name, symbol, type, "dic.txt");
    }
}


Move<char>* SUS_UI::get_move(Player<char>* player) {
    int x, y;
    char c;
    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nPlease enter your move x and y (0 to 2): and 'S' or 'U':";
        cin >> x >> y >> c;
        while (c != 'S' && c != 'U') {
            cout << "Enter 'S' or 'U': ";
            cin >> c;
        }
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        auto smart_player = dynamic_cast<SUS_SmartPlayer*>(player);
        if (!smart_player) {
            // fallback: the player is not a SUS_SmartPlayer (shouldn't happen if create_player fixed)
            // cerr << "Warning: computer player is not SUS_SmartPlayer — using default move.\n";
            // produce a simple default move (first empty cell with 'A') to avoid crash
            Board<char>* board_ptr = player->get_board_ptr();
            if (!board_ptr) throw runtime_error("Board not assigned to computer player!");
            auto tmp = board_ptr->get_board_matrix();
            bool found = false;
            for (int i = 0; i < (int)tmp.size() && !found; ++i)
                for (int j = 0; j < (int)tmp[i].size() && !found; ++j)
                    if (tmp[i][j] == '.') { x = i; y = j; c = 'A'; found = true; }
            if (!found) throw runtime_error("No valid moves available for computer.");
        } else {
            auto move = smart_player->calculateMove2();
            x = move.first.first;
            y = move.first.second;
            c = static_cast<char>(move.second);
        }
    }
    return new ne_Move(x, y, player->get_symbol() , c);
}
