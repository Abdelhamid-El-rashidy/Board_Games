cmake_minimum_required(VERSION 4.0)
project(BoardGameFramework)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(BoardGameFramework
        include/BoardGame_Classes.h
        src/main.cpp
        src/Infinity_XO_Classes.cpp
        include/Infinity_XO_Classes.h
        )