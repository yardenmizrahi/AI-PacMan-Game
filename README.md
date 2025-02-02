# AI-PacMan-Game
***
### The Run_Game function controls the game loop. It iterates until either Pacman collects all the coins or is caught by the ghosts. This function calls for the Ghost_Iteration function. and the Pacman_Iteration function.
***
## Pacman Iteration:
* Pacman's movement involves collecting coins and avoiding ghosts. Pacman uses a depth-limited BFS algorithm to find the closest coin. If a coin is closer than a predetermined distance, Pacman moves in the opposite direction to the nearest ghost. Pacman also uses a pathfinding algorithm to navigate the maze.
* Pacman's Decision Making (Finite State Machine) is implemented using a Finite State Machine (FSM). Pacman transitions between states based on the current game conditions. The states include:
* CollectingCoinsState: Pacman moves towards coins to collect them.
* EscapingGhostsState: If ghosts are nearby, Pacman switches to this state and tries to avoid the ghosts.
***
## Ghost Iteration:
* The ghosts use the A* algorithm to chase Pacman. If ghosts are unable to find a path to Pacman, they switch to a state where they are considered "stuck."
* Ghosts iteratively move towards Pacman using the A* algorithm. They prioritize cells closer to Pacman and avoid obstacles in the maze.
