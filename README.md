# Board Games Framework

A comprehensive C++ framework for implementing and playing various board games, particularly Tic-Tac-Toe variants. This project demonstrates object-oriented design principles including the Open-Closed Principle (OCP), template-based design, and polymorphism.

## 📋 Table of Contents

* [Overview](#overview)
* [Features](#features)
* [Architecture](#architecture)
* [Games Included](#games-included)
* [Building and Running](#building-and-running)
* [Project Structure](#project-structure)
* [Design Patterns](#design-patterns)
* [AI Implementation](#ai-implementation)
* [Adding New Games](#adding-new-games)
* [Testing](#testing)
* [Known Issues & Future Work](#known-issues--future-work)
* [License](#license)

## 🎮 Overview

This project provides a flexible and extensible framework for creating board games. It includes a generic base framework (`BoardGame_Classes.h`) and **13 different game implementations**, each demonstrating different gameplay mechanics and AI strategies.

The framework follows the **Open-Closed Principle** — it is open for extension (new games can be added) but closed for modification (the core framework remains unchanged).

Game selection and execution are handled through a **modular menu system**, allowing games to be added or removed without modifying the application entry point.

### Report

* [Project Report](https://docs.google.com/document/d/1GhDm3uQc0b17LqqxxypT8vBVlTFlVtnRxI1i0f75TiQ/edit?tab=t.0)

## ✨ Features

* **13 Different Games**: Multiple Tic-Tac-Toe variants and board games
* **AI Players**: Intelligent computer players using the minimax algorithm with alpha-beta pruning
* **Extensible Architecture**: New games can be added without changing existing core logic
* **Template-based Design**: Generic implementations that work with different symbol types
* **Console-based UI**: Clean and simple text-based user interfaces
* **Polymorphic Design**: Shared interfaces with game-specific behavior
* **Unified Game Menu**: Centralized menu system for launching games polymorphically

## 🏗️ Architecture

The project follows a layered architecture:

```
┌─────────────────────────────────────┐
│      Game-Specific Classes          │
│  (Boards + UI Implementations)     │
└─────────────────────────────────────┘
              ↓ extends
┌─────────────────────────────────────┐
│       Framework Core Classes        │
│  (Board, Player, UI, GameManager)   │
└─────────────────────────────────────┘
```

### Core Components

1. **Board<T>**: Abstract base class for all game boards
2. **Player<T>**: Base class for human and computer players
3. **Move<T>**: Represents a single move in the game
4. **UI<T>**: Handles user interaction and display
5. **GameManager<T>**: Orchestrates the game loop

### Menu & Game Launching System

The framework includes a **generic menu-based game launcher** that decouples game selection from game execution.

Key components:

* **GameLauncher**: Abstract interface defining a common contract for launching any game.
* **XOGameLauncher**: A templated launcher that binds together a specific `Board`, `UI`, and symbol type.
* **Menu**: Registers available games and launches them polymorphically based on user selection.

This design removes hard-coded logic from `main.cpp` and strictly follows the **Open-Closed Principle** — new games can be added without modifying existing code.

## 🎯 Games Included

| #  | Game Name            | Description                           |
| -- | -------------------- | ------------------------------------- |
| 1  | Infinity X-O         | Standard 3x3 Tic-Tac-Toe              |
| 2  | Numerical X-O        | Numbers-based Tic-Tac-Toe (sum to 15) |
| 3  | 5x5 X-O              | Larger 5x5 board variant              |
| 4  | Misere Tic-Tac-Toe   | Reverse win condition                 |
| 5  | Four Four X-O        | Movable pieces on a 4x4 board         |
| 6  | SUS Game             | Rule-based pattern game               |
| 7  | Pyramid X-O          | Pyramid-shaped board                  |
| 8  | Four-in-a-row        | Connect-Four style game               |
| 9  | Diamond Tic-Tac-Toe  | Diamond-shaped grid                   |
| 10 | Obstacles X-O        | Board with blocked cells              |
| 11 | Word X-O             | Word-forming Tic-Tac-Toe              |
| 12 | Memory Tic-Tac-Toe   | Memory-based gameplay                 |
| 13 | Ultimate Tic-Tac-Toe | 9x9 meta Tic-Tac-Toe                  |

## 🔨 Building and Running

### Prerequisites

* C++17 or later
* CMake 3.10 or later
* GCC, Clang, or MSVC

### Build Instructions

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

Run:

```bash
./BoardGameFramework
```

## 📁 Project Structure

```
Board_Games/
│
├── include/
│   ├── BoardGame_Classes.h
│   ├── GamesMenu.h               # Menu & launcher system
│   ├── AiPlayer.h
│   ├── Smart_Player.h
│   └── [Game Headers]
│
├── src/
│   ├── main.cpp                 # Entry point (uses Menu)
│   └── [Game Implementations]
│
├── CMakeLists.txt
├── Class_Diagram.puml
├── Class_Diagram.png
└── README.md
```

## 🎨 Design Patterns

### 1. Template Method Pattern

Used by `Board<T>`, `Player<T>`, and `UI<T>` for type-generic behavior.

### 2. Strategy Pattern

Different AI behaviors are implemented through interchangeable AI player classes.

### 3. Factory Pattern

`UI::create_player()` acts as a factory for creating player types.

### 4. Open-Closed Principle (OCP)

The framework allows adding new games and AI players without modifying existing core classes.

### 5. Command / Launcher Pattern

Each game is wrapped inside a `GameLauncher` object and executed polymorphically by the menu system.

## 🤖 AI Implementation

* Minimax algorithm with alpha-beta pruning
* Depth-limited search
* Heuristic board evaluation
* Game-specific move generation

## ➕ Adding New Games

To add a new game:

1. Create a new `Board` class
2. Create a new `UI` class
3. (Optional) Create an AI player
4. Register the game in the menu:

```cpp
options[14] = new XOGameLauncher<char, MyGame_UI, MyGame_Board>("My Custom Game");
```

No changes to `main.cpp` are required.

## 🧪 Testing

* Select a game from the menu
* Choose player types (Human / Computer)
* Verify win, draw, and AI behavior

## 🐛 Known Issues & Future Work

* Some games may require additional edge-case testing
* AI depth is configurable and may need tuning per game
* **A LAN-based multiplayer network interface is planned**

## 📝 License

This project is provided for educational purposes.

## 👥 Authors

* Abdelhamid
* Hazem
* Adham
* Hussien

## 🙏 Acknowledgments

* Starter framework by Dr. ElRamly (FCAI)
* Classic game theory AI algorithms
* Design patterns from *Design Patterns: Elements of Reusable Object-Oriented Software*
