//
// Created by abdelhamid on 12/9/25.
//

#ifndef BOARDGAMEFRAMEWORK_AIPLAYER_H
#define BOARDGAMEFRAMEWORK_AIPLAYER_H

#include "BoardGame_Classes.h"
#include <map>
#include <limits>
#include <vector>
#include <type_traits>

using namespace std;

/**
 * @brief Base class for AI players following Open-Closed Principle (OCP).
 * 
 * This class is open for extension but closed for modification.
 * Derived classes only need to override:
 * - createMove() - Create a move at given position
 * - createUndoMove() - Create an undo move at given position
 * 
 * Optional overrides:
 * - condition() - Check if position is valid for a move (default: checks if cell is blank)
 * - get_opponent_symbol() - Get opponent's symbol (default: works for X/O games)
 * - evaluate_board() - Evaluate board position (default: simple heuristic)
 * - get_max_depth() - Maximum search depth (default: 3)
 * - generate_move_values() - Generate possible move values for position (default: single move)
 */
template<typename T>
class AIPlayer : public Player<T> {
protected:
    T blank_symbol;
    int max_depth;

    /**
     * @brief Evaluate the current board position from AI's perspective.
     * @param boardPtr Pointer to the board
     * @param ai AI's symbol
     * @param opp Opponent's symbol
     * @param N Sequence length to check (default: 3)
     * @return Evaluation score (positive = favorable for AI)
     */
    virtual int evaluate_board(Board<T>* boardPtr, T ai, T opp, int N = 3) {
        // Default simple evaluation: check for wins/losses
        if (boardPtr->is_win(this)) return 1000;
        // Check if opponent would win - need to create a temporary check
        // For default, return 0 (neutral position)
        return 0;
    }

    /**
     * @brief Get the maximum search depth for minimax algorithm.
     * @return Maximum depth
     */
    virtual int get_max_depth() const {
        return max_depth;
    }

    /**
     * @brief Generate possible move values for a given position.
     * 
     * For simple games (like Tic-Tac-Toe), this returns a single value (AI's symbol).
     * For complex games (like Numerical XO), this returns multiple values from search space.
     * 
     * @param x X coordinate
     * @param y Y coordinate
     * @param isMaximizing Whether this is the maximizing player's turn
     * @return Vector of possible move values
     */
    virtual vector<T> generate_move_values(int x, int y, bool isMaximizing) {
        // Default: single move with AI or opponent symbol
        return vector<T>{isMaximizing ? this->get_symbol() : get_opponent_symbol()};
    }

    /**
     * @brief Minimax algorithm with alpha-beta pruning.
     * @param boardPtr Pointer to the board
     * @param maximizingPlayer Whether it's the maximizing player's turn
     * @param ai AI's symbol
     * @param opp Opponent's symbol
     * @param alpha Alpha value for pruning
     * @param beta Beta value for pruning
     * @param depth Current depth
     * @param N Sequence length parameter
     * @return Evaluation score
     */
    int minimax(Board<T>* boardPtr, bool maximizingPlayer, T ai, T opp, int alpha, int beta, int depth, int N = 3) {
        // Terminal conditions
        // Check if AI (this player) wins
        if (boardPtr->is_win(this)) return 1000 + depth; // Prefer faster wins
        
        // Check if opponent wins - create temporary player with opponent symbol
        Player<T>* tempOpponent = new Player<T>("temp_opp", opp, PlayerType::AI);
        tempOpponent->set_board_ptr(boardPtr);
        if (boardPtr->is_win(tempOpponent)) {
            delete tempOpponent;
            return -1000 - depth; // Prefer slower losses
        }
        delete tempOpponent;
        
        if (boardPtr->is_draw(this)) return 0;
        if (depth == 0) return evaluate_board(boardPtr, ai, opp, N);

        int rows = boardPtr->get_rows();
        int cols = boardPtr->get_columns();

        if (maximizingPlayer) {
            int best = numeric_limits<int>::min();
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    if (condition(i, j)) {
                        vector<T> moveValues = generate_move_values(i, j, true);
                        for (T moveVal : moveValues) {
                            Move<T>* move = createMove(i, j);
                            // Update move symbol if needed (for games with value-based moves)
                            if (move) {
                                boardPtr->update_board(move);
                                int val = minimax(boardPtr, false, ai, opp, alpha, beta, depth - 1, N);
                                Move<T>* undomove = createUndoMove(i, j);
                                if (undomove) {
                                    boardPtr->update_board(undomove);
                                }
                                delete move;
                                delete undomove;
                                
                                best = max(best, val);
                                alpha = max(alpha, best);
                                if (beta <= alpha) {
                                    return best; // Alpha-beta pruning
                                }
                            }
                        }
                    }
                }
            }
            return best;
        } else {
            int best = numeric_limits<int>::max();
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    if (condition(i, j)) {
                        vector<T> moveValues = generate_move_values(i, j, false);
                        for (T moveVal : moveValues) {
                            Move<T>* move = createMove(i, j);
                            if (move) {
                                boardPtr->update_board(move);
                                int val = minimax(boardPtr, true, ai, opp, alpha, beta, depth - 1, N);
                                Move<T>* undomove = createUndoMove(i, j);
                                if (undomove) {
                                    boardPtr->update_board(undomove);
                                }
                                delete move;
                                delete undomove;
                                
                                best = min(best, val);
                                beta = min(beta, best);
                                if (beta <= alpha) {
                                    return best; // Alpha-beta pruning
                                }
                            }
                        }
                    }
                }
            }
            return best;
        }
    }

public:
    /**
     * @brief Constructor for AIPlayer.
     * @param n Player name
     * @param s Player symbol
     * @param t Player type
     * @param blank Blank symbol used on the board
     * @param depth Maximum search depth (default: 3)
     */
    AIPlayer(string n, T s, PlayerType t, T blank, int depth = 3) 
        : Player<T>(n, s, t), blank_symbol(blank), max_depth(depth) {}

    virtual ~AIPlayer() {}

    /**
     * @brief Pure virtual function to create a valid move depending on current game rules.
     * Must be implemented by derived classes.
     * @param x the x_coordinate of move within board.
     * @param y the y_coordinate of move within board.
     * @return a pointer to a Move object. Caller is responsible for deletion.
     */
    virtual Move<T>* createMove(int x, int y) = 0;

    /**
     * @brief Pure virtual function to undo the previous move played during backtracking algorithm.
     * Must be implemented by derived classes.
     * @note The protocol to inform the update_board function in @class Board that this is an undo move 
     * is to place a blank_symbol in @class Move symbol attribute.
     * @param x the x_coordinate of move within board.
     * @param y the y_coordinate of move within board.
     * @return a pointer to a Move object. Caller is responsible for deletion.
     */
    virtual Move<T>* createUndoMove(int x, int y) = 0;

    /**
     * @brief Check if a position is valid for making a move.
     * @param x X coordinate
     * @param y Y coordinate
     * @return true if position is valid, false otherwise
     */
    virtual bool condition(int x, int y) {
        Board<T>* board_ptr = this->get_board_ptr();
        if (!board_ptr) return false;
        if (x < 0 || x >= board_ptr->get_rows() || y < 0 || y >= board_ptr->get_columns()) {
            return false;
        }
        return board_ptr->get_cell(x, y) == blank_symbol;
    }

    /**
     * @brief Get the opponent's symbol.
     * @return Opponent's symbol
     */
    virtual T get_opponent_symbol() {
        // Default implementation for X/O games
        if constexpr (std::is_same_v<T, char>) {
            T sym = this->get_symbol();
            if (sym == 'X' || sym == 'x') return 'O';
            if (sym == 'O' || sym == 'o') return 'X';
        }
        return T(); // Default fallback
    }

    /**
     * @brief Calculate and return the best move using minimax algorithm.
     * @return Pointer to the best move. Caller is responsible for deletion.
     */
    virtual Move<T>* calculateMove() {
        Board<T>* board_ptr = this->get_board_ptr();
        if (!board_ptr) throw runtime_error("Board not assigned to player!");

        int rows = board_ptr->get_rows();
        int cols = board_ptr->get_columns();

        int bestVal = numeric_limits<int>::min();
        int bestX = -1, bestY = -1;

        T ai = this->get_symbol();
        T opp = get_opponent_symbol();

        int maxDepth = get_max_depth();

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (condition(i, j)) {
                    vector<T> moveValues = generate_move_values(i, j, true);
                    for (T moveValue : moveValues) {
                        Move<T>* move = createMove(i, j);
                        if (!move) continue;
                        
                        board_ptr->update_board(move);
                        int eval = minimax(board_ptr, false, ai, opp,
                                           numeric_limits<int>::min(),
                                           numeric_limits<int>::max(),
                                           maxDepth);
                        Move<T>* undoMove = createUndoMove(i, j);
                        if (undoMove) {
                            board_ptr->update_board(undoMove);
                            delete undoMove;
                        }
                        delete move;
                        
                        if (eval > bestVal) {
                            bestVal = eval;
                            bestX = i;
                            bestY = j;
                        }
                    }
                }
            }
        }

        if (bestX == -1 || bestY == -1) {
            // Fallback: find first valid move
            for (int i = 0; i < rows && bestX == -1; i++) {
                for (int j = 0; j < cols && bestX == -1; j++) {
                    if (condition(i, j)) {
                        bestX = i;
                        bestY = j;
                    }
                }
            }
        }

        return createMove(bestX, bestY);
    }
};

#endif //BOARDGAMEFRAMEWORK_AIPLAYER_H
