//
// Created by abdelhamid on 12/11/25.
//

#ifndef BOARDGAMEFRAMEWORK_GAMESMENU_H
#define BOARDGAMEFRAMEWORK_GAMESMENU_H

#include "BoardGame_Classes.h"
#include <map>

using namespace std;

/**
 * @brief a base interface for launching a game
 */
class GameLauncher {
public:
    virtual void launch() = 0;
    virtual string get_name() const = 0;
    virtual ~GameLauncher() {}
};


template<typename T, typename UIType, typename BoardType>
class XOGameLauncher : public GameLauncher {
private:
    string name;

public:
    XOGameLauncher(const string& n) : name(n) {}

    string get_name() const override {
        return name;
    }

    void launch() override {
        UI<T>* ui = new UIType();
        Board<T>* board = new BoardType();
        Player<T>** players = ui->setup_players();

        GameManager<T> game(board, players, ui);
        game.run();

        delete board;
        delete ui;
        for (int i = 0; i < 2; i++)
            delete players[i];
        delete[] players;
    }
};

class Menu {
private:
    map<int, GameLauncher*> options;

public:
    Menu() {
        // Register for all games.
        options[1] = new XOGameLauncher<char, Infinit_XO_UI, Infinity_X_O_Board>("Infinity X-O");
        options[2] = new XOGameLauncher<char, Numerical_XO_UI, Numerical_X_O_Board>("Numerical X-O");
        options[3] = new XOGameLauncher<char, X_O_5x5_UI, X_O_5x5_Board>("5x5 X-O");
        options[4] = new XOGameLauncher<char, Misere_Tic_Tac_Toe_UI, Misere_Tic_Tac_Toe_board>("Misere Tic-Tac-Toe");
        options[5] = new XOGameLauncher<char, Four_Four_XO_UI, Four_Four_XO_Board>("Four Four X-O");
        options[6] = new XOGameLauncher<char, SUS_UI, SUS_Board>("SUS Game");
        options[7] = new XOGameLauncher<char, Pyramid_XO_UI, Pyramid_XO_Board>("Pyramid X-O");
        options[8] = new XOGameLauncher<char, Four_in_a_row_UI, Four_in_a_row_Board>("Four-in-a-row");
        options[9] = new XOGameLauncher<char, Diamond_Tic_Tac_Toe_UI, Diamond_Tic_Tac_Toe_Board>("Diamond Tic-Tac-Toe");
        options[10] = new XOGameLauncher<char, Obstacles_XO_UI, Obstacles_XO_Board>("Obstacles X-O");
        options[11] = new XOGameLauncher<char, Word_XO_UI, Word_XO_Board>("Word X-O");
        options[12] = new XOGameLauncher<char, MEMORY_TIC_TAC_TOE_UI, MEMORY_TIC_TAC_TOE_Board>("Memory Tic-Tac-Toe");
        options[13] = new XOGameLauncher<char, Ultimate_XO_UI, Ultimate_XO_Board>("Ultimate Tic-Tac-Toe");
    }

    ~Menu() {
        for (auto& p : options) delete p.second;
    }

    void print_menu() {
        cout << "\n-- Welcome to the FCAI Board Game Collection --\n";
        cout << "------------------------------------------------\n";
        cout << "0. Exit\n";

        for (auto& item : options) {
            int id = item.first;
            GameLauncher* game = item.second;
            cout << id << ". " << game->get_name() << "\n";
        }
    }

    void run() {
        int choice;
        while (true) {
            print_menu();
            cout << "Select a game (0-13): ";
            cin >> choice;

            if (choice == 0) {
                cout << "Goodbye!\n";
                break;
            }

            if (options.count(choice)) {
                options[choice]->launch();
            } else {
                cout << "Invalid option!\n";
            }
        }
    }
};

#endif //BOARDGAMEFRAMEWORK_GAMESMENU_H