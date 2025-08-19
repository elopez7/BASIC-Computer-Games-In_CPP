/**
Description
This program will print out a different maze every time it
is run and guarantees only one path through. You can choose
the dimensions of the maze--i.e. the number of squares wide
and Iong-.

computer Limitations (Does not apply to the C++ project).
The amount of memory available will determine the maximum
size maze that may be constructed. An 8K EduSystem 20
initialized for one user can draw a l3x13 maze.
RSTS/E can draw a 23 (width of paper limit) x 50 maze, even larger
using virtual memory.
Experiment on your system with the maze dimensions in
Statement 110.

Program Author
Jack Hauber
Loomis School
Windsor, CT 06095

Rewritten in C++ by Esteban Lopez
*/

#include <array>
#include <format>
#include <iostream>
#include <limits>
#include <random>
#include <stack>
#include <vector>

class MazeData {
public:
    MazeData();
    ~MazeData() = default;
    void Init();
    int GetUserInput();
    void GenerateMaze();
    void GenerateMazeDFS(int start_logical_row, int start_logical_col);
    void PrintMaze();

private:
    int num_logical_rows_;
    int num_logical_cols_;
    int width_;
    int length_;
    int start_logical_row_;
    int start_logical_col_;
    std::vector<std::vector<char>> maze_grid_; // The actual grid of characters
    std::vector<std::vector<bool>> visited_logical_; // Tracks logical cells visited
    std::mt19937 engine_; // The random number engine
};

MazeData::MazeData()
    : num_logical_rows_ {}
    , num_logical_cols_ {}
    , width_ {}
    , length_ {}
    , start_logical_row_ {}
    , start_logical_col_ {}
    , maze_grid_ {}
    , visited_logical_ {}
    , engine_ {}
{
}

void MazeData::Init()
{
    std::cout << "What is the maze's width (Enter a number greater than 2): ";
    width_ = GetUserInput();
    std::cout << "What is the maze's height (Enter a number greater than 2): ";
    length_ = GetUserInput();

    // Resize the maze grid according to user input
    maze_grid_.resize(width_, std::vector<char>(length_, 'O'));

    // Initialize maze grid with appropiate wall characters
    for (int r = 0; r < width_; ++r) {
        for (int c = 0; c < length_; ++c) {
            if (r % 2 == 0 && c % 2 == 0) {
                // This is a corner or an intersection point
                maze_grid_[r][c] = '+';
            } else if (r % 2 == 0) {
                // This is a horizontal wall segment
                maze_grid_[r][c] = '-';
            } else if (c % 2 == 0) {
                // This is a vertical wall segment
                maze_grid_[r][c] = ':';
            } else {
                // unvisited logical cells
                maze_grid_[r][c] = 'v';
            }
        }
    }

    // Calculate logical dimensions after getting physical width/length
    // This assumes an outer wall, so actual maze cells are at odd indices
    num_logical_rows_ = (width_ - 1) / 2;
    num_logical_cols_ = (length_ - 1) / 2;

    // Initialize visited_logical_ based on logical dimensions
    visited_logical_.assign(num_logical_rows_, std::vector<bool>(num_logical_cols_, false));

    // Seed the random engine once
    engine_.seed(std::random_device {}());

    // Choose random starting logical cell
    std::uniform_int_distribution<int> row_dist(0, num_logical_rows_ - 1);
    std::uniform_int_distribution<int> col_dist(0, num_logical_cols_ - 1);
    start_logical_row_ = row_dist(engine_);
    start_logical_col_ = col_dist(engine_);
}

int MazeData::GetUserInput()
{
    bool valid_input {};
    int user_input {};
    while (valid_input == false) {
        while (!(std::cin >> user_input)) {
            std::cout << "Invalid input. Please enter a number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (user_input <= 2) {
            std::cout << "Please enter a value grater than 2\n";
            continue;
        }
        valid_input = true;
    }
    return user_input;
}

void MazeData::GenerateMaze()
{
    GenerateMazeDFS(start_logical_row_, start_logical_col_);
}

// Recursive Depth-First Search (DFS) for maze generation
// This function carves out paths in the maze grid.
void MazeData::GenerateMazeDFS(int logical_row, int logical_col)
{
    // Mark current logical cell as visited
    visited_logical_[logical_row][logical_col] = true;

    // Carve the current logical cell (physical center of the cell)
    // Convert logical coordinates to physical coordinates for maze_grid_
    int current_physical_row { 2 * logical_row + 1 };
    int current_physical_col { 2 * logical_col + 1 };

    maze_grid_[current_physical_row][current_physical_col] = ' ';

    // Define possible directions and their corresponding wall offsets relative to the current physical cell
    //  {logical_row_change, logical_col_change, physical_wall_row_offset, physical_wall_col_offset}
    std::vector<std::tuple<int, int, int, int>> directions {
        { -1, 0, -1, 0 }, // North: logical_r-1, wall above
        { 1, 0, 1, 0 }, // South: logical_r+1, wall below
        { 0, -1, 0, -1 }, // West: logical_c-1, wall left
        { 0, 1, 0, 1 } // East: logical_c+1, wall right
    };

    // Shuffle directions for random exploration
    std::shuffle(directions.begin(), directions.end(), engine_);

    for (const auto& dir : directions) {
        int dr { std::get<0>(dir) }; // Logical row change
        int dc { std::get<1>(dir) }; // Logical col change
        int wall_pr_offset { std::get<2>(dir) }; // Physical row offset for the wall to carve
        int wall_pc_offset { std::get<3>(dir) }; // Physical col offset for the wall to carve

        int next_logical_r { logical_row + dr };
        int next_logical_c { logical_col + dc };

        // Check if the next logical cell is within bounds and unvisited
        if (next_logical_r >= 0 && next_logical_r < num_logical_rows_ && next_logical_c >= 0 && next_logical_c < num_logical_cols_ && !visited_logical_[next_logical_r][next_logical_c]) {
            // Calculate physical coordinates for the wall to carve
            int wall_physical_r { current_physical_row + wall_pr_offset };
            int wall_physical_c { current_physical_col + wall_pc_offset };

            // Carve path through the wall
            maze_grid_[wall_physical_r][wall_physical_c] = 'O';

            GenerateMazeDFS(next_logical_r, next_logical_c);
        }
    }
}

void MazeData::PrintMaze()
{
    for (const auto& row : maze_grid_) {
        for (char cell : row) {
            std::cout << cell;
        }
        std::cout << '\n';
    }
}

int main()
{

    MazeData maze {};
    maze.Init();
    maze.GenerateMaze();
    maze.PrintMaze();

    return 0;
}
