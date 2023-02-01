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

void processPaths(Limits &l, std::vector<std::vector<char>> &g) {
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

void printGrid(std::vector<std::vector<char>> &v) {
    for(const auto& i: v) {
        for(const auto &j: i) {
            printf("%c", j);
        }
        printf("\n");
    }
}

void fillGrid(Limits &l, std::vector<std::vector<char>> &v) {
    bool enterTheAbyss = false;
    int rounds = 0;
    while(!enterTheAbyss) {
        rounds++;
        // Add a new sand element to the grid at the '+' position
        int hzPos = 500 - l.left;
        v[0][hzPos] = 'o';
        bool atRest = false;
        while(!atRest) {
            hzPos = 500 - l.left;
            for(int i = 1; i < v.size(); i++) {
                char &prevPos = v[i - 1][hzPos];
                char &pos = v[i][hzPos];
                char &leftPos = v[i][hzPos - 1];
                char &rightPos = v[i][hzPos + 1];
                if(hzPos == 0) {
                    if(rightPos != '.') {
                        prevPos = 'X';
                        atRest = true;
                        enterTheAbyss = true;
                        break;
                    }
                } else if(hzPos == v[0].size() - 1) {
                    if(leftPos != '.') {
                        prevPos = 'X';
                        atRest = true;
                        enterTheAbyss = true;
                        break;
                    }
                }
                if(pos == '.') {
                    prevPos = '.';
                    pos = 'o';
                } else if(pos == '#') {
                    if(leftPos == '#' && rightPos == '#') {
                        atRest = true;
                        break;
                    } else if(leftPos == '.') {
                        prevPos = '.';
                        leftPos = 'o';
                        hzPos--;
                    } else if(rightPos == '.') {
                        prevPos = '.';
                        rightPos = 'o';
                        hzPos++;
                    }
                } else if(pos == 'o') {
                    // Check left
                    if(leftPos == '.') {
                        prevPos = '.';
                        leftPos = 'o';
                        hzPos--;
                    } else if(rightPos == '.') {
                        prevPos = '.';
                        rightPos = 'o';
                        hzPos++;
                    } else {
                        atRest = true;
                        break;
                    }
                }
            }
        }
        // Used for debugging the iterations of sand
        if(rounds == 10) {
            break;
        }
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
    // Process all the paths and construct the map
    processPaths(limits, grid);
    fillGrid(limits, grid);
    grid[0][500 - limits.left] = '+';
    printGrid(grid);
    int count = 0;
    for(const auto &i: grid) {
        for(const auto &j: i) {
            if(j == 'o') {
                count++;
            }
        }
    }
    printf("Total sand particles = %i\n", count);
    return 0;
}

// 7825 Too high
// 967 Too low
