//
// Created by abdelhamid on 11/26/25.
//

#ifndef BOARDGAMEFRAMEWORK_FOUR_FOUR_XO_H
#define BOARDGAMEFRAMEWORK_FOUR_FOUR_XO_H
#include "BoardGame_Classes.h"
#include "AiPlayer.h"
#include <limits>
#include <vector>
#include <utility>
using namespace std;

/**
 * @class Four_Four_XO_Board
 * @brief Represent the 4*4 XO Tic Tac Toe Game.
 */
class Four_Four_XO_Board : public Board<char> {
    char blank_symbol = '.';

public:
    /** Default constructor of 4*4 Board Size */
    Four_Four_XO_Board();
    /**
     * @brief Update the board with a new move.
     * @param move The move object containing position and symbol.
     * @return true if the move is valid and applied, false otherwise.
     */
    virtual bool update_board(Move<char>* move);

    /** @brief Check if a player has won. */
    virtual bool is_win(Player<char>*);

    /** @brief Check if a player has lost. */
    virtual bool is_lose(Player<char>*) {return false;};

    /** @brief Check if the game ended in a draw. */
    virtual bool is_draw(Player<char>*);

    /** @brief Check if the game is over. */
    virtual bool game_is_over(Player<char>*);

    /** Destructor */
    virtual ~Four_Four_XO_Board() {};

};

/**
 * @class dualMove
 * @brief Customized move that changed symbol position in board
 *
 * Inherits from the generic `Move<char>` base class and provides
 * specific functionality for move.
 */
class dualMove : public Move<char> {
private:
    int dx;
    int dy;
public:
    /**
     * Constructor to intialize Base Class Move and Derived Class dualMove
     * @param x x-coordinate in grid
     * @param y y-coordinate in grid
     * @param symbol the current player's move symbol
     * @param dy horizontal change (right,left) 1 or -1
     * @param dx vertical change (up,down) 1 or -1
     */
    dualMove(int x, int y, char symbol, int dy, int dx) : Move<char>(x,y,symbol), dy(dy), dx(dx) {};

    /**
     * @return The horizontal change
     */
    int get_dy() const {return dy;};
    /**
     * @return The vertical change
     */
    int get_dx() const {return dx;};

    virtual ~dualMove() {};
};

/**
 * @class FourFourAIPlayer
 * @brief AI player for Four-Four XO game using minimax algorithm.
 */
class FourFourAIPlayer : public AIPlayer<char> {
private:
    // Directions: Up, Right, Down, Left
    vector<pair<int, int>> directions;
    int current_dx, current_dy; // Store direction for createMove

protected:
    /**
     * @brief Check if position has a piece belonging to the player that can be moved.
     */
    virtual bool condition(int x, int y) override {
        Board<char>* board_ptr = this->get_board_ptr();
        if (!board_ptr) return false;
        if (x < 0 || x >= board_ptr->get_rows() || y < 0 || y >= board_ptr->get_columns()) {
            return false;
        }
        char cell = board_ptr->get_cell(x, y);
        return cell == this->get_symbol();
    }

    /**
     * @brief Check if position has a piece belonging to a specific symbol.
     */
    bool condition_for_symbol(int x, int y, char symbol) {
        Board<char>* board_ptr = this->get_board_ptr();
        if (!board_ptr) return false;
        if (x < 0 || x >= board_ptr->get_rows() || y < 0 || y >= board_ptr->get_columns()) {
            return false;
        }
        char cell = board_ptr->get_cell(x, y);
        return cell == symbol;
    }

    /**
     * @brief Helper to create undo move for any symbol.
     */
    Move<char>* createUndoMoveForSymbol(int x, int y, int dx, int dy, char symbol) {
        // After move, piece is at (x+dx, y+dy). To undo, move it back to (x, y)
        // So move from (x+dx, y+dy) with direction (-dx, -dy)
        return new dualMove(x + dx, y + dy, symbol, -dy, -dx);
    }

    /**
     * @brief Override minimax to handle opponent pieces correctly.
     */
    int minimax(Board<char>* boardPtr, bool maximizingPlayer, char ai, char opp, int alpha, int beta, int depth, int N = 3) {
        // Terminal conditions
        if (boardPtr->is_win(this)) return 1000 + depth;

        Player<char>* tempOpp = new Player<char>("temp", opp, PlayerType::AI);
        tempOpp->set_board_ptr(boardPtr);
        if (boardPtr->is_win(tempOpp)) {
            delete tempOpp;
            return -1000 - depth;
        }
        delete tempOpp;

        if (boardPtr->is_draw(this)) return 0;
        if (depth == 0) return evaluate_board(boardPtr, ai, opp, N);

        int rows = boardPtr->get_rows();
        int cols = boardPtr->get_columns();

        if (maximizingPlayer) {
            int best = numeric_limits<int>::min();
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    // Check for AI's pieces
                    if (!condition_for_symbol(i, j, ai)) continue;

                    for (auto& dir : directions) {
                        int dx = dir.first;
                        int dy = dir.second;
                        if (i + dx < 0 || i + dx >= rows || j + dy < 0 || j + dy >= cols) continue;
                        if (boardPtr->get_cell(i + dx, j + dy) != blank_symbol) continue;

                        // Try the move
                        Move<char>* move = new dualMove(i, j, ai, dy, dx);
                        if (!move) continue;

                        if (boardPtr->update_board(move)) {
                            int val = minimax(boardPtr, false, ai, opp, alpha, beta, depth - 1, N);

                            // Undo the move - move piece back from (i+dx, j+dy) to (i, j)
                            Move<char>* undomove = createUndoMoveForSymbol(i, j, dx, dy, ai);
                            if (undomove && boardPtr->update_board(undomove)) {
                                delete undomove;
                            } else {
                                // Undo failed - this should never happen but handle it
                                delete undomove;
                            }
                            delete move;

                            best = max(best, val);
                            alpha = max(alpha, best);
                            if (beta <= alpha) {
                                return best;
                            }
                        } else {
                            delete move;
                        }
                    }
                }
            }
            return best;
        } else {
            int best = numeric_limits<int>::max();
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    // Check for opponent's pieces
                    if (!condition_for_symbol(i, j, opp)) continue;

                    for (auto& dir : directions) {
                        int dx = dir.first;
                        int dy = dir.second;
                        if (i + dx < 0 || i + dx >= rows || j + dy < 0 || j + dy >= cols) continue;
                        if (boardPtr->get_cell(i + dx, j + dy) != blank_symbol) continue;

                        // Create move with opponent's symbol
                        Move<char>* move = new dualMove(i, j, opp, dy, dx);
                        if (!move) continue;

                        if (boardPtr->update_board(move)) {
                            int val = minimax(boardPtr, true, ai, opp, alpha, beta, depth - 1, N);

                            // Undo the move - move piece back from (i+dx, j+dy) to (i, j)
                            Move<char>* undomove = createUndoMoveForSymbol(i, j, dx, dy, opp);
                            if (undomove && boardPtr->update_board(undomove)) {
                                delete undomove;
                            } else {
                                // Undo failed - this should never happen but handle it
                                delete undomove;
                            }
                            delete move;

                            best = min(best, val);
                            beta = min(beta, best);
                            if (beta <= alpha) {
                                return best;
                            }
                        } else {
                            delete move;
                        }
                    }
                }
            }
            return best;
        }
    }

    /**
     * @brief Evaluate board position with heuristic.
     */
    virtual int evaluate_board(Board<char>* boardPtr, char ai, char opp, int N = 3) override {
        if (boardPtr->is_win(this)) return 1000;

        Player<char>* tempOpp = new Player<char>("temp", opp, PlayerType::AI);
        tempOpp->set_board_ptr(boardPtr);
        if (boardPtr->is_win(tempOpp)) {
            delete tempOpp;
            return -1000;
        }
        delete tempOpp;

        // Simple heuristic: count pieces in winning positions
        // Check for 2 in a row (potential win)
        int score = 0;
        auto board = boardPtr->get_board_matrix();
        int rows = board.size();
        int cols = board[0].size();
        char blank = '.';

        // Check for potential winning lines
        auto count_line = [&](int x, int y, int dx, int dy, char sym) -> int {
            int count = 0;
            for (int k = 0; k < 3; k++) {
                int nx = x + k * dx;
                int ny = y + k * dy;
                if (nx >= 0 && nx < rows && ny >= 0 && ny < cols) {
                    if (board[nx][ny] == sym) count++;
                    else if (board[nx][ny] != blank) return -1; // blocked
                }
            }
            return count;
        };

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                // Check horizontal
                int ai_count = count_line(i, j, 0, 1, ai);
                int opp_count = count_line(i, j, 0, 1, opp);
                if (ai_count > 0) score += ai_count * 10;
                if (opp_count > 0) score -= opp_count * 10;

                // Check vertical
                ai_count = count_line(i, j, 1, 0, ai);
                opp_count = count_line(i, j, 1, 0, opp);
                if (ai_count > 0) score += ai_count * 10;
                if (opp_count > 0) score -= opp_count * 10;

                // Check diagonal
                ai_count = count_line(i, j, 1, 1, ai);
                opp_count = count_line(i, j, 1, 1, opp);
                if (ai_count > 0) score += ai_count * 10;
                if (opp_count > 0) score -= opp_count * 10;
            }
        }

        return score;
    }

public:
    FourFourAIPlayer(string n, char s, PlayerType t)
        : AIPlayer<char>(n, s, t, '.', 3), current_dx(0), current_dy(0) {
        directions = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}}; // Up, Right, Down, Left
    }

    /**
     * @brief Override calculateMove to handle direction-based moves.
     */
    virtual Move<char>* calculateMove() override {
        Board<char>* board_ptr = this->get_board_ptr();
        if (!board_ptr) throw runtime_error("Board not assigned to player!");

        int rows = board_ptr->get_rows();
        int cols = board_ptr->get_columns();

        int bestVal = numeric_limits<int>::min();
        int bestX = -1, bestY = -1, bestDx = 0, bestDy = 0;

        char ai = this->get_symbol();
        char opp = get_opponent_symbol();

        int maxDepth = get_max_depth();

        // Iterate over all positions
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                // Check if position has player's piece
                if (!condition(i, j)) continue;

                // Try each direction
                for (auto& dir : directions) {
                    int dx = dir.first;
                    int dy = dir.second;

                    // Check if move is valid
                    if (i + dx < 0 || i + dx >= rows || j + dy < 0 || j + dy >= cols) continue;
                    if (board_ptr->get_cell(i + dx, j + dy) != blank_symbol) continue;

                    // Try the move
                    current_dx = dx;
                    current_dy = dy;
                    Move<char>* move = createMove(i, j);
                    if (!move) continue;

                    if (board_ptr->update_board(move)) {
                        int eval = minimax(board_ptr, false, ai, opp,
                                          numeric_limits<int>::min(),
                                          numeric_limits<int>::max(),
                                          maxDepth);

                        // Undo the move - move piece back from (i+dx, j+dy) to (i, j)
                        Move<char>* undoMove = createUndoMoveForSymbol(i, j, dx, dy, ai);
                        if (undoMove && board_ptr->update_board(undoMove)) {
                            delete undoMove;
                        } else {
                            delete undoMove;
                        }
                        delete move;

                        if (eval > bestVal) {
                            bestVal = eval;
                            bestX = i;
                            bestY = j;
                            bestDx = dx;
                            bestDy = dy;
                        }
                    } else {
                        delete move;
                    }
                }
            }
        }

        if (bestX == -1 || bestY == -1) {
            // Fallback: find first valid move
            for (int i = 0; i < rows && bestX == -1; i++) {
                for (int j = 0; j < cols && bestX == -1; j++) {
                    if (condition(i, j)) {
                        for (auto& dir : directions) {
                            int dx = dir.first;
                            int dy = dir.second;
                            if (i + dx >= 0 && i + dx < rows && j + dy >= 0 && j + dy < cols) {
                                if (board_ptr->get_cell(i + dx, j + dy) == blank_symbol) {
                                    bestX = i;
                                    bestY = j;
                                    bestDx = dx;
                                    bestDy = dy;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }

        current_dx = bestDx;
        current_dy = bestDy;
        return createMove(bestX, bestY);
    }

    /**
     * @brief Create a dualMove at position (x, y) with stored direction.
     */
    virtual Move<char>* createMove(int x, int y) override {
        return new dualMove(x, y, this->get_symbol(), current_dy, current_dx);
    }

    /**
     * @brief Create an undo move (reverse the move).
     */
    virtual Move<char>* createUndoMove(int x, int y) override {
        // Reverse the move: move piece back from (x+dx, y+dy) to (x, y)
        return new dualMove(x + current_dx, y + current_dy, this->get_symbol(), -current_dy, -current_dx);
    }
};

/**
 * @class Four_Four_XO_UI
 * @brief User Interface class for 4*4 XO Tic Tac Toe Game.
 *
 * Inherits from the generic `UI<char>` base class and provides
 * X-O�specific functionality for player setup and move input.
 *
 * @see UI
 */
class Four_Four_XO_UI : public UI<char> {

public:
    Four_Four_XO_UI() : UI<char>("Welcome to Four-Four Tic Tac Toe Game",3) {}

    /**
     * @brief Set up players (Human or Computer).
     */
    virtual Player<char>** setup_players() override;

    /**
     * @brief Create a player based on name, symbol, and type.
     */
    virtual Player<char>* create_player(string& name, char symbol, PlayerType type) override;

    /**
     * @brief Retrieves the next move from a player.
     * @param player Pointer to the player whose move is being requested.
     * @return A pointer to a new `Move<char>` object representing the player's action.
     */
    virtual Move<char>* get_move(Player<char>* player);

    virtual ~Four_Four_XO_UI() {};

};

#endif //BOARDGAMEFRAMEWORK_FOUR_FOUR_XO_H