#include <iostream>
#include <fstream>
#include <vector>

std::ifstream input;

struct Move {
    int x;
    int y;
    Move() = default;
    Move(int x, int y) : x(x), y(y){};
};

struct RockPath {
    std::vector<Move> path;
};

struct Limits {
    int left = 500;
    int right = 500;
    int down = 0;
};

std::vector<RockPath> paths;

Move createMove(const std::string &s) {
    // Split the string of the move into it's x and y constituents
    // Returns a Move
    std::size_t commaPos = s.find(',');
    return {std::stoi(s.substr(0, commaPos++)), std::stoi(s.substr(commaPos, s.size() - commaPos))};
}

Limits calculateLimits() {
    Limits l;
    for(const auto& i: paths) {
        for(const auto &j: i.path) {
            if(j.x < l.left) {
                l.left = j.x;
            } else if(j.x > l.right) {
                l.right = j.x;
            }
            if(j.y > l.down) {
                l.down = j.y;
            }
        }
    }
    return l;
}

void processPaths() {

}

void processLine(std::string &s) {
    // Split the line into moves, seperated by " -> "
    // Add the complete RockPath to paths
    RockPath rPath;
    std::size_t start;
    std::size_t end = 0;
    while((start = s.find_first_not_of(" -> ", end)) != std::string::npos) {
        end = s.find(" -> ", start);
        rPath.path.emplace_back(createMove(s.substr(start, end - start)));
    }
    paths.emplace_back(rPath);
}

void printGrid(std::vector<std::vector<char>> &v) {
    for(const auto& i: v) {
        for(const auto &j: i) {
            printf("%c", j);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    // Template for command line arguments
#if 0
    if(argc > 1) {
    } else {
        std::cout << "Usage: " << argv[0] << " " << std::endl;
        return 0;
    }
#else
    (void) argc;
    (void) argv;
#endif
    // Load file and process the lines
    input.open("Day14.txt");
    if(!input.is_open()) {
        return -1;
    }
    // Process the lines of the file, adding new RockPath elements to paths
    std::string temp;
    while(getline(input, temp)) {
        processLine(temp);
    }
    // Determine the left, right and down limits
    Limits limits = calculateLimits();
    // Calculate the width and height to construct a grid
    // Add 1 to allow for the 0th element
    int width = (limits.right - limits.left) + 1;
    int height = limits.down + 1;
    // Construct the grid consisting of std::vector<char> as horizontal elements
    // by the height of the grid
    std::vector<char> lines(width, '.');
    std::vector<std::vector<char>> grid(height, lines);
    // Set the sand start point to a '+'
    grid[0][500 - limits.left] = '+';
    printGrid(grid);
    // Process all the paths and construct the map
    processPaths();
    return 0;
}
