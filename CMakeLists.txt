cmake_minimum_required(VERSION 4.0)
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
)
