//
// Created by abdelhamid on 12/5/25.
//

#ifndef BOARDGAMEFRAMEWORK_SMART_PLAYER_H
#define BOARDGAMEFRAMEWORK_SMART_PLAYER_H

#include "BoardGame_Classes.h"
#include <limits>
#include <algorithm>

using namespace std;

template <typename T>
class smartPlayer : public Player<T> {
protected:
    // Virtual so derived games can override for custom symbols
    virtual T get_opponent_symbol() const {
        // This is default X-O symbols
        if constexpr (is_same<T, char>::value) {
            return (this->get_symbol() == 'X') ? 'O' : 'X';
        }
        return T();
    }

    // Heuristic evaluation for large boards
    virtual int evaluate_board(const vector<vector<T>>& b, T ai, T opp, int N = 3) const {
        int score = 0;
        int rows = b.size();
        int cols = b[0].size();

        auto count_sequence = [&](int i, int j, int dx, int dy, T symbol) {
            int cnt = 0;
            for (int k = 0; k < N; k++) {
                int x = i + k * dx;
                int y = j + k * dy;
                if (x < 0 || x >= rows || y < 0 || y >= cols) return 0;
                if (b[x][y] == symbol) cnt++;
                else if (b[x][y] != '.') return 0; // blocked
            }
            return cnt;
        };

        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++) {
                score += count_sequence(i, j, 0, 1, ai);   // row
                score += count_sequence(i, j, 1, 0, ai);   // column
                score += count_sequence(i, j, 1, 1, ai);   // diag TL-BR
                score += count_sequence(i, j, 1, -1, ai);  // diag TR-BL

                score -= count_sequence(i, j, 0, 1, opp);
                score -= count_sequence(i, j, 1, 0, opp);
                score -= count_sequence(i, j, 1, 1, opp);
                score -= count_sequence(i, j, 1, -1, opp);
            }

        return score;
    }

    virtual bool is_win(const vector<vector<T>>& b, T symbol, int N = 3) const {
        int rows = b.size();
        int cols = b[0].size();

        // Rows
        for (int i = 0; i < rows; i++)
            for (int j = 0; j <= cols - N; j++) {
                bool win = true;
                for (int k = 0; k < N; k++)
                    if (b[i][j + k] != symbol) { win = false; break; }
                if (win) return true;
            }

        // Columns
        for (int j = 0; j < cols; j++)
            for (int i = 0; i <= rows - N; i++) {
                bool win = true;
                for (int k = 0; k < N; k++)
                    if (b[i + k][j] != symbol) { win = false; break; }
                if (win) return true;
            }

        // Diagonal TL-BR
        for (int i = 0; i <= rows - N; i++)
            for (int j = 0; j <= cols - N; j++) {
                bool win = true;
                for (int k = 0; k < N; k++)
                    if (b[i + k][j + k] != symbol) { win = false; break; }
                if (win) return true;
            }

        // Diagonal TR-BL
        for (int i = 0; i <= rows - N; i++)
            for (int j = N - 1; j < cols; j++) {
                bool win = true;
                for (int k = 0; k < N; k++)
                    if (b[i + k][j - k] != symbol) { win = false; break; }
                if (win) return true;
            }

        return false;
    }

    virtual bool is_draw(const vector<vector<T>>& b, int N = 3) const {
        for (auto &row : b)
            for (auto &cell : row)
                if (cell == '.') return false;
        return !is_win(b, this->get_symbol(), N) && !is_win(b, get_opponent_symbol(), N);
    }

    // Minmax Alg with alpha bata optimization and depth optimization to avoid excessive search for large boards
    // https://www.geeksforgeeks.org/dsa/minimax-algorithm-in-game-theory-set-4-alpha-beta-pruning/
    int minimax(vector<vector<T>>& b, bool maximizingPlayer, T ai, T opp, int alpha, int beta, int depth, int N = 3) {
        if (is_win(b, ai, N)) return 1000 + depth; // prefer faster win
        if (is_win(b, opp, N)) return -1000 - depth; // prefer slower loss
        if (is_draw(b, N)) return 0;
        if (depth == 0) return evaluate_board(b, ai, opp, N);

        int rows = b.size();
        int cols = b[0].size();

        if (maximizingPlayer) {
            int best = numeric_limits<int>::min();
            for (int i = 0; i < rows; i++)
                for (int j = 0; j < cols; j++)
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
            for (int i = 0; i < rows; i++)
                for (int j = 0; j < cols; j++)
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

public:
    smartPlayer(string n, T s, PlayerType t) : Player<T>(n, s, t) {}

    virtual pair<int,int> calculateMove()  {
        Board<T>* board_ptr = this->get_board_ptr();
        if (!board_ptr) throw runtime_error("Board not assigned to player!");

        auto tempBoard = board_ptr->get_board_matrix();
        int rows = tempBoard.size();
        int cols = tempBoard[0].size();

        int bestVal = numeric_limits<int>::min();
        int bestX = -1, bestY = -1;

        T ai = this->get_symbol();
        T opp = get_opponent_symbol();

        int maxDepth = 3; // tweak: higher depth = stronger AI but slower

        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
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

    virtual ~smartPlayer() {}
};

#endif //BOARDGAMEFRAMEWORK_SMART_PLAYER_H