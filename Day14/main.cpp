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

// false for left, true for right
void addExtraLine(bool side, std::vector<std::vector<char>> &v, Limits &l) {
    if(side) {
        l.right++;
        for(auto i: v) {
            i.push_back('.');
        }
    } else {
        l.left--;
        for(auto &i: v) {
            i.insert(i.begin(), '.');
        }
    }
}

void printGrid(Limits &l, std::vector<std::vector<char>> &v) {
    v[0][500 - l.left] = '+';
    for(const auto& i: v) {
        for(const auto &j: i) {
            printf("%c", j);
        }
        printf("\n");
    }
}

int fillGrid(Limits &l, std::vector<std::vector<char>> &v, bool floor = false) {
    int sandCount = 0;
    while(true) {
        // Add a new sand element to the grid at the '+' position
        int hzPos = 500 - l.left;
        // Set the character at the position to 'o' to denote the falling sand
        v[0][hzPos] = 'o';
        // Use atRest to detect when to add a new piece of sand, once it is stationary, break the while loop
        bool atRest = false;
        while(!atRest) {
            for(int i = 1; i < v.size(); i++) {
                // Get a reference to the characters at the previous point, the current point, and one left and right of the current point
                // These are used to detect for wall/sand/air
                char &prevPos = v[i - 1][hzPos];
                char &pos = v[i][hzPos];
                char &leftPos = v[i][hzPos - 1];
                char &rightPos = v[i][hzPos + 1];
                // If the sand reaches the left edge and there is no possible move right, it falls into the abyss
                if(hzPos == 0) {
                    if(rightPos != '.' && !floor) {
                        prevPos = 'X';
                        return sandCount;
                    } else if(floor) {
                        addExtraLine(false, v, l);
                    }
                // If the sand reaches the right edge and there is no possible move left, it falls into the abyss
                } else if(hzPos == v[0].size()) {
                    if(leftPos != '.' && !floor) {
                        prevPos = 'X';
                        return sandCount;
                    } else if(floor) {
                        addExtraLine(true, v, l);
                    }
                }
                // If the position is air, and it is free, it can be moved into
                // If the y value of pos is the size of the vector, it is at the floor
                if(pos == '.') {
                    if(i == v.size() - 1 && !floor) {
                        prevPos = '.';
                        pos = 'X';
                        return sandCount;
                    } else if(floor) {
                    }
                    prevPos = '.';
                    pos = 'o';
                // If the position is either wall or sand, detect if there is a free
                // position to the left, then the right, otherwise it is at rest
                } else if(pos == '#' || pos == 'o') {
                    if(i == 1) {
                        return sandCount;
                    }
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
            // Iterate the sandCount to denote how many have been placed.
            printGrid(l, v);
            sandCount++;
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
    // Calculate the width and height to construct a gridPart1
    // Add 1 to allow for the 0th element
    int width = (limits.right - limits.left) + 1;
    int height = limits.down + 1;
    // Construct the gridPart1 consisting of std::vector<char> as horizontal elements
    // by the height of the gridPart1
    std::vector<char> lines(width, '.');
    std::vector<std::vector<char>> gridPart1(height, lines), gridPart2(height, lines);
    // Set the sand start point to a '+'
    // Process all the paths and construct the map
    processPaths(limits, gridPart1);
    processPaths(limits, gridPart2);
    printf("Total sand particles = %i\n", fillGrid(limits, gridPart1));
    printf("Total sand particles = %i\n", fillGrid(limits, gridPart2, true));
    printGrid(limits, gridPart2);
    return 0;
}
