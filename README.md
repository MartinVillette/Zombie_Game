# ZOMBIE SURVIVAL GAME

## OVERVIEW
This is a 2D top-down zombie survival game created using C++ and SDL2. The player navigates 
through a procedurally generated map, fighting off zombies with a gun while trying to survive 
as long as possible.

## GAME FEATURES
- Top-down 2D gameplay with sprite animations
- Player movement using WASD keys
- Shooting mechanics with mouse control
- Dynamic lighting system
- Pathfinding AI for zombies that hunt the player
- Procedurally generated terrain
- Score tracking based on zombies killed
- Survival timer
- Zoom functionality (P to zoom in, M to zoom out)

## TECHNICAL IMPLEMENTATION
- Built with C++ and SDL2/SDL2_image libraries
- Object-oriented design with class inheritance (Human base class for Player and Zombie)
- Pointer management for game entities
- A* pathfinding algorithm for zombie movement
- Collision detection system
- Texture rendering with sprite animation
- Dynamic lighting effects

## CONTROLS
- W: Move up
- A: Move left
- S: Move down
- D: Move right
- Left mouse button: Shoot
- P: Zoom in
- M: Zoom out

## GAME MECHANICS
- Zombies will chase the player when within their view range
- Each zombie deals damage when they get close enough to the player
- Player must shoot zombies to survive
- Game ends when player health reaches zero
- Score increases with each zombie killed

## SYSTEM REQUIREMENTS
- Windows operating system with SDL2 and SDL2_image libraries
- The game requires the following DLL files (included in the distribution):
  - SDL2.dll
  - SDL2_image.dll

## BUILDING THE PROJECT
The project can be built using the included Makefile. Simply run:
  main.exe

## PROJECT STRUCTURE
- src/: Source code files
- include/: Header files
- assets/: Game sprites and textures
- lib/: SDL2 libraries

## CREDITS
Created as a learning project to explore C++ programming, pointers, and game development 
basics with SDL2.
