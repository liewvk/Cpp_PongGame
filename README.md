# C++ Pong Game

A classic Pong game implementation in C++ using the SFML (Simple and Fast Multimedia Library) graphics framework. This project is part of a C++ Game Programming course (Chapter 5) and demonstrates fundamental game development concepts including game loops, physics, collision detection, and AI.

## Overview

This is a two-player Pong game where you play against an AI opponent. The game features a ball that bounces off paddles and walls, score tracking, and a simple but effective AI for the computer opponent.

## Features

- **Player vs Computer Gameplay**: Control the left paddle with arrow keys and compete against an AI-controlled opponent
- **Collision Detection**: Ball collisions with paddles, walls, and goal boundaries
- **Score Tracking**: Real-time score display in the window title
- **AI Opponent**: Computer paddle follows the ball with realistic movement constraints
- **Fixed Timestep**: Frame-rate independent physics simulation using a fixed timestep (120 Hz)
- **Visual Polish**: Color-coded paddles (blue for player, orange for computer), centered field line, and smooth animations

## Controls

- **Up Arrow**: Move player paddle up
- **Down Arrow**: Move player paddle down
- **Space**: Serve the ball (when waiting for serve)
- **Escape**: Exit the game

## Project Structure

```
Cpp_PongGame/
├── CMakeLists.txt          # CMake build configuration
├── CMakePresets.json       # CMake preset configurations
├── src/
│   └── main.cpp            # Main game implementation
├── .gitignore              # Git ignore rules
└── README.md               # This file
```

## Game Configuration

The game uses a modular configuration system defined in the `Config` namespace:

- **Window**: 800x600 pixels
- **Paddle Dimensions**: 16px width × 100px height
- **Player Speed**: 340 px/s
- **Computer Speed**: 190 px/s (intentionally slower for balance)
- **Ball Radius**: 10px
- **Serve Speed**: 260 px/s horizontal, 180 px/s vertical
- **Fixed Timestep**: 1/120 second

## Gameplay Mechanics

### Ball Physics
- Bounces off top and bottom walls with reflection
- Bounces off paddles with velocity preserved
- Exits from left or right edge to score points

### Scoring
- Player scores when the ball exits the right side
- Computer scores when the ball exits the left side
- Score is displayed in the window title along with game status

### AI Behavior
- Computer paddle tracks the ball's vertical position
- Movement is rate-limited to simulate realistic AI difficulty
- Intentionally slower than player speed for gameplay balance

### Game Loop
- Implements a fixed timestep architecture (120 Hz)
- Frame rate capped at 60 FPS for display
- Separate logic updates from rendering

## Building

### Prerequisites
- CMake 3.28 or higher
- C++17 compatible compiler
- Internet connection (to download SFML dependency)

### Build Instructions

#### Using CMake directly:
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

#### Using CMake presets:
```bash
cmake --preset default
cmake --build build --preset default
```

### Running
The compiled executable will be in `build/bin/`:
```bash
./build/bin/PongGame
```

## Technical Details

### Dependencies
- **SFML 3.0.2**: Graphics rendering library
  - Only graphics features are enabled (audio and network disabled for minimal build size)
  - Automatically downloaded and built by CMake

### Code Highlights

- **Configuration Namespace**: Centralized game parameters for easy tuning
- **Fixed Timestep Simulation**: Ensures consistent physics behavior regardless of frame rate
- **Vector Math**: Uses SFML's `sf::Vector2f` for positions and velocities
- **Collision Detection**: Implements bounding box and basic intersection testing
- **Event Handling**: Keyboard input processing and window event management

## Learning Outcomes

This project demonstrates:
- Game loop architecture and timing
- Physics simulation and vector mathematics
- Collision detection systems
- AI movement and decision making
- Graphics rendering with SFML
- C++ best practices (namespaces, const correctness, STL algorithms)
- CMake build system configuration

## Future Enhancements

Possible improvements and extensions:
- Add sound effects and background music
- Implement difficulty levels with varying AI speeds
- Add ball spin and angle calculations based on paddle hit location
- Create a pause/resume feature
- Implement a two-player mode
- Add visual effects (particle system, screen shake)
- Track and display high scores
- Add game states (menu, playing, game over)

## License

This is an educational project created as part of a C++ Game Programming course.

## Author

Created as a learning exercise in game development with C++ and SFML.
