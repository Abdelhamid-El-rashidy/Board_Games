#include <iostream>
#include <unordered_set>
#include <iomanip>
#include <cctype>  // for toupper()
#include <random>
#include <fstream>
#include "../include/Words_XO.h"
#include "../include/Smart_Player.h"


using namespace std;

const vector<char> LETTERS = {
    'A','B','C','D','E','F','G','H','I','J',
    'K','L','M','N','O','P','Q','R','S','T',
    'U','V','W','X','Y','Z'
};

class wordSmartPlayer : public smartPlayer<char> {
    using smartPlayer<char>::smartPlayer;

protected:
    vector<string> dictionary;
    vector<char> possibleLetters;      // only letters that appear in the dictionary
    mutable unsigned long node_counter = 0; // count nodes visited (mutable to update in const funcs)
    const unsigned long MAX_NODES = 10000; // tune this: 200k node budget as example


public:
    wordSmartPlayer(string n, char s, PlayerType t, const string& dictFile)
            : smartPlayer<char>(n, s, t)
    {
        loadDictionary(dictFile);
    }

protected:
    void loadDictionary(const string& filename) {
        ifstream in(filename);
        string w;
        unordered_set<char> letters;
        while (in >> w) {
            // convert to uppercase and keep only alphabetic characters
            for (char &ch : w) ch = toupper(static_cast<unsigned char>(ch));
            dictionary.push_back(w);
            for (char ch : w) if (isalpha((unsigned char)ch)) letters.insert(ch);
        }
        // build possibleLetters vector from set (stable order not required)
        possibleLetters.assign(letters.begin(), letters.end());
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


bool Check_word(const string& W) {
    ifstream file("dic.txt");
    if (!file.is_open()) {
        cerr << "Error: cannot open file dic.txt\n";
        return false; // previously returned 1 (true) — wrong
    }
    string line;
    while (getline(file, line)) {
        if (W == line) return true;
    }
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
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
         << " player: " << name << " (" << symbol << ")\n";

    if (type == PlayerType::HUMAN) {
        return new Player<char>(name, symbol, type);
    } else {
        // Construct a wordSmartPlayer and pass the dictionary file path
        // Make sure "dic.txt" path is correct relative to your working dir
        return new wordSmartPlayer(name, symbol, type, "dic.txt");
    }
}


Move<char>* Word_XO_UI::get_move(Player<char>* player) {
    int x, y;
    char c;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nPlease enter your move x and y (0 to 2) and character: ";
        cin >> x >> y >> c;
    }
    else if (player->get_type() == PlayerType::COMPUTER) {
        auto smart_player = dynamic_cast<wordSmartPlayer*>(player);
        if (!smart_player) {
            // fallback: the player is not a wordSmartPlayer (shouldn't happen if create_player fixed)
            cerr << "Warning: computer player is not wordSmartPlayer — using default move.\n";
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
    return new neMove(x, y, player->get_symbol(), c);
}


