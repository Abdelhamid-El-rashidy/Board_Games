cmake_minimum_required(VERSION 3.31.6)
project(BoardGameFramework)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(BoardGameFramework
        include/BoardGame_Classes.h
        src/main.cpp
        src/Infinity_XO_Classes.cpp
        include/Infinity_XO_Classes.h
        src/Four_Four_XO.cpp
        include/Four_Four_XO.h
        src/X_O_5x5.cpp
        include/X_O_5x5.h
        include/Numerical_X_O.h
        src/Numerical_X_O.cpp
        include/SUS_Classes.h
        src/SUS_Classes.cpp
        include/Misere_Tic_Tac_Toe.h
        src/Misere_Tic_Tac_Toe.cpp
        src/Pyramid_XO.cpp
        include/Pyramid_XO.h
        src/Four-in-a-row.cpp
        include/Four-in-a-row.h
        src/Diamond_Tic_Tac_Toe.cpp
        include/Diamond_Tic_Tac_Toe.h
        src/Obstacles_XO.cpp
        include/Obstacles_XO.h
        src/Words_XO.cpp
        include/Words_XO.h
)
