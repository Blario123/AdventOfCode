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
Limits l;

Move createMove(const std::string &s) {
    // Split the string of the move into it's x and y constituents
    // Returns a Move
    std::size_t commaPos = s.find(',');
    return {std::stoi(s.substr(0, commaPos++)), std::stoi(s.substr(commaPos, s.size() - commaPos))};
}

void calculateLimits() {
    // Allocate the lowest and highest x values to the left and right limit
    // Allocate the highest y value to the down limit
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
}

void processPaths(std::vector<std::vector<char>> &g) {
    for(const auto &i: paths) {
        std::pair<int, int> pos = {0, 0};
        for(int j = 1; j < i.path.size(); j++) {
            // To construct the rock formations, the from and to positions are needed
            // These are used to calculate the delta between moves
            Move from = i.path[j - 1];
            Move to = i.path[j];
            int deltaX = to.x - from.x;
            int deltaY = to.y - from.y;
            // Only process moves that aren't 0
            if(deltaX != 0) {
                // Correct for 0th position incrementing by either 1 or -1
                deltaX += abs(deltaX)/deltaX;
                if(deltaX < 0) {
                    for(int hz = from.x - l.left; hz > (from.x - l.left) + deltaX; hz--) {
                        g[from.y][hz] = '#';
                    }
                } else {
                    for(int hz = from.x - l.left; hz < (from.x - l.left) + deltaX; hz++) {
                        g[from.y][hz] = '#';
                    }
                }
                pos.first += deltaX;
            } else {
                // Correct for 0th position incrementing by either 1 or -1
                deltaY += abs(deltaY) / deltaY;
                if(deltaY < 0) {
                    for(int vt = from.y; vt > from.y + deltaY; vt--) {
                        g[vt][from.x - l.left] = '#';
                    }
                } else {
                    for(int vt = from.y; vt < from.y + deltaY; vt++) {
                        g[vt][from.x - l.left] = '#';
                    }
                }
                pos.second += deltaY;
            }
        }
    }
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

enum MoveStatus {
    Left,
    Right,
    Down,
    None,
    Edge
};

// false for left, true for right
void addExtraLine(const MoveStatus &m, std::vector<std::vector<char>> &v, bool &f) {
    // If the edge is reached on the left, decrease the left limit and insert at the beginning of the vector a new '.'
    if(m == Left) {
        l.left--;
        for(auto &i: v) {
            if(i == v.back()) {
                continue;
            }
            i.insert(i.begin(), '.');
        }
    // If the edge is reached on the right, increase the right limit and insert at the end of the vector a new '.'
    } else if(m == Right) {
        l.right++;
        for(auto &i: v) {
            if(i == v.back()) {
                continue;
            }
            i.push_back('.');
        }
    }
    // If part 2 is running, f = true, insert a '#' the end of the vector
    if(f) {
        v.back().push_back('#');
    }
}

MoveStatus canMove(std::vector<std::vector<char>> &v, int x, std::size_t y, bool &f) {
    // Get a reference to the character at the current x and y position
    char &pos = v[y][x];
    // If x is at either edge, determine if a move can be made down, or return Edge
    if(x == 0 || x == v[0].size() - 1) {
        if(pos == '.') {
            return Down;
        }
        return Edge;
    }
    // If x lies within the limits, get a references of the left and right of the current position
    char &lPos = v[y][x - 1];
    char &rPos = v[y][x + 1];
    // If the sand has fallen on to a solid piece
    if(pos == 'o' || pos == '#') {
        // If the vertical limit has been reached and the floor is set, there is no move
        if(y == v.size() - 1 && f) {
            return None;
        }
        // Check the left position for an air gap, if so, return Left
        if(lPos == '.') {
            return Left;
        // Check the right position for an air gap, if so, return Right
        } else if(rPos == '.') {
            return Right;
        }
    }
    // If the position is an air gap
    if(pos == '.') {
        // If the vertical limit has been reached and the floor is not set, the edge has been reached
        if(y == v.size() - 1 && !f) {
            return Edge;
        }
        return Down;
    }
    // No other condition is met, assume that there is no move
    return None;
}

int fillGrid(std::vector<std::vector<char>> &v, bool floor = false) {
    int sandCount = 0;
    while(true) {
        // Add a new sand element to the grid at the '+' position
        int hzPos = 500 - l.left;
        // Set the character at the position to 'o' to denote the falling sand
        v[0][hzPos] = 'o';
        bool firstPass = true;
        for(std::size_t i = 1; i < v.size(); i++) {
            // Get the status of the current position
            MoveStatus status = canMove(v, hzPos, i, floor);
            switch(status) {
                // If a move is possible left, decrease the hzPos
                case Left:
                    hzPos--;
                    break;
                // If a move is possible right, increase the hzPos
                case Right:
                    hzPos++;
                    break;
                // If a move is possible down, do nothing
                case Down:
                    break;
                // If no move is available
                case None:
                    // Set the previous position to a 'o' to ensure the sand is calculated correctly
                    v[i - 1][hzPos] = 'o';
                    // If the y position is 1 and the floor is set, return the count
                    if(i == 1 && floor) {
                        return sandCount;
                    }
                    // Set i to the size of the vector to stop the loop once there is no move
                    i = v.size();
                    break;
                // If an edge has been found and floor is not set, return the count
                case Edge:
                    if(!floor) {
                        return sandCount;
                    }
                    // If this is the first time an edge has been found in a sands travel,
                    // add an extra line to the left if the hzPos is 0 or to the right if not 0
                    // i is also decremented to allow the new path to be followed
                    if(firstPass) {
                        firstPass = false;
                        i--;
                        if(hzPos == 0) {
                            addExtraLine(Left, v, floor);
                            // If a new line is added to the left, the hzPos needs to be incremented
                            // to allow the access of the vector to be in the correct place
                            hzPos++;
                        } else {
                            addExtraLine(Right, v, floor);
                        }
                    }
                    break;
            }
        }
        // Iterate the sandCount to denote how many have been placed.
        sandCount++;
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
    calculateLimits();
    // Calculate the width and height to construct a gridPart1
    // Add 1 to allow for the 0th element
    int width = (l.right - l.left) + 1;
    int height = l.down + 1;
    // Construct the gridPart1 consisting of std::vector<char> as horizontal elements
    // by the height of the gridPart1
    std::vector<char> lines(width, '.');
    std::vector<std::vector<char>> gridPart1(height, lines), gridPart2(height + 1, lines);
    gridPart2.emplace_back(width, '#');
    // Process all the paths and construct the map
    processPaths(gridPart1);
    processPaths(gridPart2);
    printf("Total sand particles before falling into void = %i\n", fillGrid(gridPart1));
    printf("Total sand particles to reach the top = %i\n", fillGrid(gridPart2, true));
    return 0;
}
