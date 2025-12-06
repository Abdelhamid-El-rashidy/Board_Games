//
// Created by abdelhamid on 12/5/25.
//

#ifndef BOARDGAMEFRAMEWORK_COMPUTER_PLAYER_H
#define BOARDGAMEFRAMEWORK_COMPUTER_PLAYER_H

#include "BoardGame_Classes.h"

using namespace std;

template <typename T>
class computerPlayer : public Player<T> {
private:

public:
    computerPlayer(string n, T s, PlayerType t) : Player<T>(n,s,t) {};

    /**
     * @brief Calculate the best move using Minimax algorithm.
     * @return coordinates of move.
     */
    virtual pair<int,int> calculateMove() {
        Board<T>ptr = this->get_board_ptr();
        if (!ptr) {
            throw runtime_error("Board not assigned to player!");
        }
        int rows = ptr.get_rows(); int cols = ptr.get_columns();
        int x = -1, y = -1;



    }

    virtual ~computerPlayer() {};
};

#endif //BOARDGAMEFRAMEWORK_COMPUTER_PLAYER_H