# Maze-solving-algorithm-using-Flood-Fill-algorithm

🧩 **Maze Solver Using Floodfill Algorithm**

This project implements a maze-solving algorithm in C++ using the floodfill (Breadth-First Search) technique. It simulates how a robot (similar to a micromouse) navigates through the maze, discovers walls, updates its knowledge of the environment, and finds the shortest path to a goal area.

<img width="600" height="600" alt="Figure_1" src="https://github.com/user-attachments/assets/2ee847e7-69d5-4346-868c-941dab08b860" />

🚀 **Features**
- ✅ 8x8 maze representation using a 2D grid.
- ✅ Each cell stores wall information (up, right, down, left).
- ✅ Floodfill algorithm to calculate the shortest distances from goal(s).
- ✅ Dynamic wall discovery – the solver updates its knowledge as it moves.
- ✅ Pathfinding and movement output with directions:
  - F → Forward
  - L → Turn Left
  - R → Turn Right
  - B → Backward
- ✅ Hardcoded maze structure (modifiable for different layouts).
- ✅ Prints the path of visited cells.

📂 **Project Structure**
- `cell_info` → Stores wall information for a single cell.
- `wall_maze` → Stores the entire maze as a 2D array of `cell_info`.
- `init_maze()` → Initializes the maze with wall configurations.
- `floodfill()` → Implements the BFS algorithm to compute distances from the goal.
- `solve()` → Navigates from the start to the goal using updated distance maps.
- `get_move_direction()` → Converts grid movements into robot-style directions.
- `print_walls()` & `print_arr()` → Debugging utilities.

🏁 **How It Works**
The maze walls are hardcoded in the `init_maze()` function. The goal area is designated (e.g., the center cells: (3,3), (3,4), (4,3), (4,4)). The algorithm starts from the position (7,0) and performs the following steps:
1. Runs floodfill to compute distance values.
2. Moves step by step toward the goal, updating walls as they are discovered.
3. Recomputes distances if a dead end is encountered.
4. Prints movement instructions (F, L, R, B) and the final path of visited cells.

🛠️ **Build & Run**
To compile the program, use the following command:

```
g++ maze_solver.cpp -o maze_solver
```

To run the program, execute:

```
./maze_solver
```

🖼️ **Example Output**
The output will show movement directions (F, R, L, B) along with the final path of coordinates leading to the goal. An example output format may look like:

```
F F R F L F ...
(7,0) (6,0) (5,0) ... (3,3)
```

🔮 **Future Improvements**
- Load the maze from file input instead of hardcoding.
- Visualize the maze and the solver in a graphical user interface (GUI).
- Support larger mazes (16x16) for a complete micromouse challenge.
- Implement different solving strategies (DFS, A*).
