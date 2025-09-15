#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

#define maze_size 8   // Define maze size as 8x8

// Structure for each cell's wall information
// walls[0] = up, walls[1] = right, walls[2] = down, walls[3] = left
typedef struct cell_infos{
    bool walls[4];
}cell_info;

// Structure for the entire maze
typedef struct wall_mazes{
    cell_info cells[maze_size][maze_size];
}wall_maze;

// Function declarations
void print_arr(vector<vector<int>> arr,int n);
void print_walls(wall_maze maze);
void init_maze(wall_maze &maze);
vector<vector<int>> init_arr(int n);
vector<vector<int>> floodfill(wall_maze maze, vector<pair<int,int>> end_cords);
void get_move_direction(pair<int,int>ncord, pair<int,int>cord, int &prev_direct);
void solve(wall_maze maze, pair<int,int> start_cod, vector<pair<int,int>> end_cords);

// Direction arrays: Up, Right, Down, Left
const int dx[] = {-1,0,1,0};
const int dy[] = {0,1,0,-1};

// Initialize a 2D array filled with -1 (unvisited state)
vector<vector<int>> init_arr(int n){
    return vector<vector<int>> (n, vector<int>(n,-1));
}

// Floodfill algorithm to compute shortest distance from target cells
vector<vector<int>> floodfill(wall_maze maze, vector<pair<int,int>> end_cords){
    vector<vector<int>> arr = init_arr(maze_size);  // Distance array
    queue<pair<int,int>> queue;                    // BFS queue
    int value = 0;

    // Initialize BFS with goal/end coordinates
    for (pair<int,int> p: end_cords){
        arr[p.first][p.second] = value;
        queue.push(p);
    }

    // BFS loop
    while (!queue.empty()){
        pair<int,int> cord = queue.front();
        queue.pop();

        // Explore 4 directions
        for (int i=0; i<4; i++){
            int x = cord.first+dx[i];
            int y = cord.second+dy[i];
            // Check bounds, wall conditions, and unvisited
            if (x>=0 && x<maze_size && y>=0 && y<maze_size 
                && !maze.cells[cord.first][cord.second].walls[i] 
                && !maze.cells[x][y].walls[(i+2)%4] 
                && arr[x][y]==-1){
                
                arr[x][y] = arr[cord.first][cord.second]+1; // Update distance
                pair<int,int> new_cord = {x,y};
                queue.push(new_cord);
            }
        }
    }
    return arr;
}

// Maze solving function using floodfill + wall discovery
void solve(wall_maze maze, pair<int,int> start_cod, vector<pair<int,int>> end_cords){
    wall_maze initial_maze={}; // Initially unknown maze
    vector<vector<int>> arr = floodfill(initial_maze,end_cords); // Distance map
    vector<pair<int,int>> visited; // Path storage

    int x = start_cod.first;
    int y = start_cod.second;
    int prev_direct=0; // Initial facing direction

    // Continue until reaching goal (distance 0)
    while (arr[x][y]!=0){
        pair<int,int> cord = {x,y};

        // Store visited path
        if (visited.empty() || visited.back() != cord) {
            visited.push_back(cord);
        }

        // Look for neighbor with smaller distance
        int min_value = arr[x][y];
        int min_x = x;
        int min_y = y;
        bool found = false;

        for (int i=0; i<4; i++){
            int new_x = x+dx[i];
            int new_y = y+dy[i];

            // Update initial_maze with actual discovered walls
            if(maze.cells[x][y].walls[i]){
                initial_maze.cells[x][y].walls[i] = true;
                if (new_x>=0 && new_x<maze_size && new_y>=0 && new_y<maze_size){
                    initial_maze.cells[new_x][new_y].walls[(i+2)%4] = true;
                }
            }

            // Choose next move if better path exists
            if(new_x>=0 && new_x<maze_size && new_y>=0 && new_y<maze_size 
                && min_value>arr[new_x][new_y] 
                && !maze.cells[x][y].walls[i] 
                && !maze.cells[new_x][new_y].walls[(i+2)%4]){
                
                min_x = new_x;
                min_y = new_y;
                min_value = arr[new_x][new_y];
                found = true;
            }
        }

        if(found){
            // Move to better cell and print movement direction
            get_move_direction({min_x,min_y},{x,y},prev_direct);
            x=min_x;
            y=min_y;
        }else{
            // If stuck, recompute distances with updated walls
            arr = floodfill(initial_maze,end_cords);
            continue;
        }
    }

    // Add goal cell
    visited.push_back({x,y});

    // Print visited path
    int n = visited.size();
    cout<<"\n";
    for (int i=0; i<n; i++){
        cout<<"("<<visited[i].first<<","<<visited[i].second<<") ";
    }
    
}

int main(){
    wall_maze maze={};
    init_maze(maze); // Initialize predefined maze layout
    vector<pair<int,int>> end_cords = {{3,3},{3,4},{4,3},{4,4}}; // Goal zone
    solve(maze,{7,0},end_cords); // Start at (7,0), solve maze
}

// Print the integer 2D array (distances)
void print_arr(vector<vector<int>> arr,int n){
    cout<<"\n";
    for (int i=0; i<n; i++){
        for (int j=0; j<n; j++){
            cout << arr[i][j]<<",";
        }
        cout<<"\n";
    }
}

// Print all wall information of the maze
void print_walls(wall_maze maze){
    cout<<"\n";
    for (int i=0; i<maze_size; i++){
        for (int j=0; j<maze_size; j++){
            cout<<"(";
            cout<< maze.cells[i][j].walls[0]<<",";
            cout<<maze.cells[i][j].walls[1]<<",";
            cout<<maze.cells[i][j].walls[2]<<",";
            cout<<maze.cells[i][j].walls[3]<<")"<<",";
        }
        cout<<"\n";
    }
}

// Determine and print movement direction (F=forward, R=right, L=left, B=back)
void get_move_direction(pair<int,int>ncord, pair<int,int>cord, int &prev_direct){
    int n_dx = ncord.first-cord.first;
    int n_dy = ncord.second-cord.second;
    int new_direct;

    // Match new direction with dx/dy arrays
    if (n_dx==dx[0] && n_dy==dy[0]){
        new_direct=0;
    }else if(n_dx==dx[1] && n_dy==dy[1]){
        new_direct=1;
    }else if(n_dx==dx[2] && n_dy==dy[2]){
        new_direct=2;
    }else if(n_dx==dx[3] && n_dy==dy[3]){
        new_direct=3;
    }

    // Compute turn direction
    int dif = new_direct - prev_direct;
    switch (dif)
    {
    case 0:
        cout<<"F "; // Forward
        break;
    case 1:
        cout<<"R "; // Right turn
        break;
    case 2:
        cout<<"B "; // Backward
        break;
    case 3:
        cout<<"L "; // Left turn
        break;
    case -1:
        cout<<"L "; // Left turn
        break;
    case -2:
        cout<<"B "; // Backward
    case -3:
        cout<<"R "; // Right turn
    }
    prev_direct = new_direct;
}

// Hardcoded maze walls initialization
// (Defines the structure of the maze used for simulation)
void init_maze(wall_maze &maze){
    // Large block of manual wall assignments
    // Format: maze.cells[row][col].walls[direction] = true;
    // direction: 0=up, 1=right, 2=down, 3=left
    maze.cells[0][0].walls[0]=true;
    maze.cells[0][1].walls[0]=true;
    maze.cells[0][2].walls[0]=true;
    maze.cells[0][3].walls[0]=true;
    maze.cells[0][4].walls[0]=true;
    maze.cells[0][5].walls[0]=true;
    maze.cells[0][6].walls[0]=true;
    maze.cells[0][7].walls[0]=true;
    maze.cells[0][0].walls[3]=true;
    maze.cells[1][0].walls[3]=true;
    maze.cells[2][0].walls[3]=true;
    maze.cells[3][0].walls[3]=true;
    maze.cells[4][0].walls[3]=true;
    maze.cells[5][0].walls[3]=true;
    maze.cells[6][0].walls[3]=true;
    maze.cells[7][0].walls[3]=true;
    maze.cells[7][0].walls[2]=true;
    maze.cells[7][1].walls[2]=true;
    maze.cells[7][2].walls[2]=true;
    maze.cells[7][3].walls[2]=true;
    maze.cells[7][4].walls[2]=true;
    maze.cells[7][5].walls[2]=true;
    maze.cells[7][6].walls[2]=true;
    maze.cells[7][7].walls[2]=true;
    maze.cells[0][7].walls[1]=true;
    maze.cells[1][7].walls[1]=true;
    maze.cells[2][7].walls[1]=true;
    maze.cells[3][7].walls[1]=true;
    maze.cells[4][7].walls[1]=true;
    maze.cells[5][7].walls[1]=true;
    maze.cells[6][7].walls[1]=true;
    maze.cells[7][7].walls[1]=true;
    maze.cells[0][0].walls[2]=true;
    maze.cells[0][2].walls[2]=true;
    maze.cells[0][3].walls[1]=true;
    maze.cells[0][4].walls[3]=true;
    maze.cells[0][5].walls[2]=true;
    maze.cells[0][6].walls[2]=true;
    maze.cells[1][0].walls[0]=true;
    maze.cells[1][1].walls[1]=true;
    maze.cells[1][1].walls[2]=true;
    maze.cells[1][2].walls[0]=true;
    maze.cells[1][2].walls[1]=true;
    maze.cells[1][2].walls[3]=true;
    maze.cells[1][3].walls[2]=true;
    maze.cells[1][3].walls[3]=true;
    maze.cells[1][4].walls[2]=true;
    maze.cells[1][5].walls[0]=true;
    maze.cells[1][5].walls[1]=true;
    maze.cells[1][6].walls[0]=true;
    maze.cells[1][6].walls[2]=true;
    maze.cells[1][6].walls[3]=true;
    maze.cells[2][0].walls[1]=true;
    maze.cells[2][1].walls[0]=true;
    maze.cells[2][1].walls[1]=true;
    maze.cells[2][1].walls[3]=true;
    maze.cells[2][2].walls[3]=true;
    maze.cells[2][3].walls[0]=true;
    maze.cells[2][3].walls[2]=true;
    maze.cells[2][4].walls[0]=true;
    maze.cells[2][4].walls[1]=true;
    maze.cells[2][5].walls[1]=true;
    maze.cells[2][5].walls[3]=true;
    maze.cells[2][6].walls[0]=true;
    maze.cells[2][6].walls[3]=true;
    maze.cells[3][1].walls[1]=true; 
    maze.cells[3][1].walls[2]=true;
    maze.cells[3][2].walls[1]=true;
    maze.cells[3][2].walls[3]=true;
    maze.cells[3][3].walls[0]=true;
    maze.cells[3][3].walls[3]=true; 
    maze.cells[3][4].walls[1]=true; 
    maze.cells[3][5].walls[1]=true;
    maze.cells[3][5].walls[2]=true;
    maze.cells[3][5].walls[3]=true;
    maze.cells[3][6].walls[1]=true;
    maze.cells[3][6].walls[3]=true;
    maze.cells[3][7].walls[3]=true;
    maze.cells[4][0].walls[2]=true;
    maze.cells[4][1].walls[0]=true;
    maze.cells[4][1].walls[2]=true;
    maze.cells[4][2].walls[1]=true; 
    maze.cells[4][3].walls[2]=true;
    maze.cells[4][3].walls[3]=true;
    maze.cells[4][4].walls[1]=true;
    maze.cells[4][4].walls[2]=true;
    maze.cells[4][5].walls[0]=true;
    maze.cells[4][5].walls[3]=true;
    maze.cells[4][6].walls[1]=true;
    maze.cells[4][6].walls[2]=true;
    maze.cells[4][7].walls[3]=true;
    maze.cells[5][0].walls[0]=true;
    maze.cells[5][1].walls[0]=true;
    maze.cells[5][1].walls[1]=true;
    maze.cells[5][2].walls[3]=true;
    maze.cells[5][3].walls[0]=true;           
    maze.cells[5][3].walls[1]=true; 
    maze.cells[5][3].walls[2]=true;
    maze.cells[5][4].walls[0]=true;
    maze.cells[5][4].walls[3]=true;
    maze.cells[5][5].walls[2]=true;
    maze.cells[5][6].walls[0]=true;
    maze.cells[5][6].walls[1]=true;
    maze.cells[5][7].walls[3]=true;                                                       
    maze.cells[6][0].walls[1]=true; 
    maze.cells[6][1].walls[1]=true;
    maze.cells[6][1].walls[3]=true;
    maze.cells[6][2].walls[1]=true;
    maze.cells[6][2].walls[2]=true;
    maze.cells[6][2].walls[3]=true;
    maze.cells[6][3].walls[0]=true; 
    maze.cells[6][3].walls[3]=true;
    maze.cells[6][4].walls[1]=true;
    maze.cells[6][4].walls[2]=true;
    maze.cells[6][5].walls[0]=true; 
    maze.cells[6][5].walls[3]=true;
    maze.cells[6][6].walls[1]=true;
    maze.cells[6][6].walls[2]=true;
    maze.cells[6][7].walls[3]=true;
    maze.cells[7][0].walls[1]=true;  
    maze.cells[7][1].walls[3]=true; 
    maze.cells[7][2].walls[0]=true;
    maze.cells[7][3].walls[1]=true;
    maze.cells[7][4].walls[0]=true;
    maze.cells[7][4].walls[3]=true;
    maze.cells[7][6].walls[0]=true;
    maze.cells[2][6].walls[1]=true;
    maze.cells[2][7].walls[3]=true;
}
