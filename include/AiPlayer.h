//
// Created by abdelhamid on 12/9/25.
//

#ifndef BOARDGAMEFRAMEWORK_AIPLAYER_H
#define BOARDGAMEFRAMEWORK_AIPLAYER_H

#include "BoardGame_Classes.h"
#include <map>

using namespace std;

template<typename T>
class AIPlayer : public Player<T> {
private:
    map<T,bool> computerSearchSpace;
    map<T,bool> opponentSearchSpace;
    T blank_symbol;
    int minimax(Board<T>* boardPtr, bool maximizingPlayer, T ai, T opp, int alpha, int beta, int depth, int N = 3) {
        if (boardPtr->is_win(this)) return 1000 + depth;
        if (boardPtr->is_win(this)) return -1000 - depth;
        if (boardPtr->is_draw(this)) return 0;
        if (depth == 0) return evaluate_board(boardPtr, ai, opp, N);

        int rows = boardPtr->size();
        int cols = boardPtr[0]->size();

        if (maximizingPlayer) {
            int best = numeric_limits<int>::min();
            for (int i = 0; i < rows; i++)
                for (int j = 0; j < cols; j++)
                    if (condition(i,j)) {
                        for (auto &[val,state]: computerSearchSpace) {
                            if (state==false) continue;
                            Move<T>* move = createMove(i,j);
                            boardPtr->update_board(move);
                            int val = minimax(boardPtr, false, ai, opp, alpha, beta, depth - 1, N);
                            Move<T>* undomove = createUndoMove(i,j);
                            boardPtr->update_board(undomove);
                            best = max(best, val);
                            alpha = max(alpha, best);
                            if (beta <= alpha) return best;
                        }
                    }
            return best;
        } else {
            int best = numeric_limits<int>::max();
            for (int i = 0; i < rows; i++)
                for (int j = 0; j < cols; j++)
                    if (condition(i,j)) {
                        for (auto &[val,state]: computerSearchSpace) {
                            if (state==false) continue;
                            Move<T>* move = createMove(i,j);
                            boardPtr->update_board(move);
                            int val = minimax(boardPtr, true, ai, opp, alpha, beta, depth - 1, N);
                            Move<T>* undomove = createUndoMove(i,j);
                            boardPtr->update_board(undomove);
                            best = min(best, val);
                            beta = min(beta, best);
                            if (beta <= alpha) return best;
                        }
                    }
            return best;
        }
    }

public:
    AIPlayer(string n, T s, PlayerType t, map<T,int> computerSearchSpace_,map<T,int> opponentSearchSpace_, T blank_) : Player<T>(n, s, t) ,
    computerSearchSpace(computerSearchSpace_) , opponentSearchSpace(opponentSearchSpace_) , blank_symbol(blank_) {}

    /**
     * @brief a setter function for the searchspace for two players.
     * @param searchSpace map of possible moves values of two players
     * @param element the value of move.
     * @param state true of value exits, false otherwise.
     */
    void updateSearchSpace(map<T,bool> &searchSpace ,T element, bool state);;

    /**
     * @brief a function to create a valid move depending on current game rules.
     * @param x the x_coordinate of move within board.
     * @param y the y_coordinate of move within board.
     * @return a pointer to a @class Move object.
     */
    virtual Move<T>* createMove(int x, int y) = 0;
    /**
     * @brief a function to undo the previous move played during backtracking algorithm.
     * @note The protocol to inform the update_board function in @class Board that this is an undo move is to place a blank_symbol in @class Move symbol attribute.
     * @param x the x_coordinate of move within board.
     * @param y the y_coordinate of move within board.
     * @return a pointer to a @class Move object.
     */
    virtual Move<T>* createUndoMove(int x, int y) = 0;

    virtual bool condition(int x, int y) {
        return Player<T>::boardPtr[x][y] == blank_symbol;
    }

    virtual T get_opponent_symbol();

    virtual Move<T>* calculateMove();
};

template<typename T>
void AIPlayer<T>::updateSearchSpace(map<T, bool> &searchSpace, T element, bool state) {
    searchSpace[element] = state;
}

template<typename T>
Move<T> * AIPlayer<T>::calculateMove() {
    Board<T>* board_ptr = this->get_board_ptr();
    if (!board_ptr) throw runtime_error("Board not assigned to player!");

    int rows = board_ptr->rows;
    int cols = board_ptr->columns;

    int bestVal = numeric_limits<int>::min();
    int bestX = -1, bestY = -1;

    T ai = this->get_symbol();
    T opp = get_opponent_symbol();

    int maxDepth = 3; // tweak: higher depth = stronger AI but slower


    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            if (condition(i,j)) {
                for (auto &[val,state]: computerSearchSpace) {
                    if (state==false) continue;
                    Move<T>* move = createMove(i,j);
                    board_ptr->update_board(move);
                    int moveVal = minimax(board_ptr, false, ai, opp,
                                          numeric_limits<int>::min(),
                                          numeric_limits<int>::max(),
                                          maxDepth);
                    Move<T>* undoMove = createUndoMove(i,j);
                    board_ptr->update_board(undoMove);
                    if (moveVal > bestVal) {
                        bestVal = moveVal;
                        bestX = i;
                        bestY = j;
                    }
                }
            }

    Move<T>* bestMove = new Move(bestX,bestY,ai);
    return bestMove;
}


#endif //BOARDGAMEFRAMEWORK_AIPLAYER_H
