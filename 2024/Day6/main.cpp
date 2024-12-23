#include <cstdio>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <format>
#include <cstdarg>

#ifdef GIF
    #include "gif.h"
#endif
typedef enum {
    N = 0,
    E,
    S,
    W
} Direction;

std::vector<std::string> map;
std::vector<std::string> mapRaw;
std::vector<std::pair<int,int>> path;
std::pair<int,int> pos;
std::pair<int,int> startPos;
Direction direction = N;
std::vector<std::pair<int,int>> prevPath;

int countVisited(const std::vector<std::string> &v) {
    int sum = 0;
    for(int i = 0; i < map.size(); i++) {
        for(int j = 0; j < map[i].size(); j++) {
            if(map[i][j] == 'x') {
                sum++;
            }
        }
    }
    return sum;
}

void printMap(const std::vector<std::string> &m = map) {
    for(auto &i: m) {
        printf("%s\n", i.c_str());
    }   
    printf("\n");
}

template <typename T>
bool contains(const std::vector<T> &v, const T &i) {
    for(auto &j: v) {
        if(j == i) {
            return true;
        }
    }
    return false;
}

void addPosToPath(const std::pair<int,int> &p) {
    if(!contains(path, p)) {
        path.emplace_back(p);
    }
}

void print(const char *str, ... ) {
#ifdef DEBUG
    va_list args;
    va_start(args, str);
    printf(str, args);
    va_end(args);
#endif
}

#ifdef GIF
void writeMapToGif(GifWriter &g, const std::vector<std::string> &v) {
    uint8_t image[map[0].size() * map.size() * 4];
    for(int i = 0; i < map.size(); i++) {
        for(int j = 0; j < map[0].size(); j++) {
            std::vector<uint8_t> colour = {0xFF, 0x00, 0xFF, 0xFF};
            switch(map[i][j]) {
                case '.':
                    colour = {0x00, 0x00, 0x00, 0xFF};
                    break;
                case '#':
                    colour = {0x20, 0x20, 0x20, 0xFF};
                    break;
                case 'x':
                    colour = {0xFF, 0xFF, 0xFF, 0xFF};
                    break;
            }
            for(int k = 0; k < 4; k++) {
                image[(i * map[0].size() * 4) + (j * 4) + k] = colour[k]; 
            }
        }
    }
    GifWriteFrame(&g, image, map[0].size(), map.size(), 10);
}
#endif

int main(int argc, char** argv) {
    if(argc > 1) {
        // Data input
        std::ifstream input;
        input.open(argv[1]);

        std::string line;
        getline(input, line, '\n');
        // Day 1 - (built in to processing) Find good pages with rules provided
        while(input.good()) {
            map.emplace_back(line);
            mapRaw.emplace_back(line);
            if(line.find('^') != std::string::npos) {
                pos = {line.find('^'), map.size() - 1};
                startPos = pos;
            }
            getline(input, line, '\n');
        }
        // Day 1 - All visited positions
        bool searching = true;
        while(searching) {
            switch(direction) {
                case N:
                    if(pos.second == 0) {
                        searching = false;
                        break;
                    }
                    if(map[pos.second - 1][pos.first] == '#') {
                        direction = E;
                    } else {
                        pos.second--;
                        addPosToPath(pos);
                    }
                    break;
                case E:
                    if(pos.first == (map[0].size() - 1)) {
                        searching = false;
                        break;
                    }
                    if(map[pos.second][pos.first + 1] == '#') {
                        direction = S;
                    } else {
                        pos.first++;
                        addPosToPath(pos);
                    }
                    break;
                case S:
                    if(pos.second == (map.size() - 1)) {
                        searching = false;
                        break;
                    }
                    if(map[pos.second + 1][pos.first] == '#') {
                        direction = W;
                    } else {
                        pos.second++;
                        addPosToPath(pos);
                    }
                    break;
                case W:
                    if(pos.first == 0) {
                        searching = false;
                        break;
                    }
                    if(map[pos.second][pos.first - 1] == '#') {
                        direction = N;
                    } else {
                        pos.first--;
                        addPosToPath(pos);
                    }
                    break;
            }
            map[pos.second][pos.first] = 'x';
        }
        printf("Visited positions = %d\n", countVisited(map));
        // Day 2 - Looking for possible loops with an extra obstacle placed.
        int sum = 0;
        int ii = 0;
        for(auto &i: path) {
            map = mapRaw; // Reset the map for processing
            map[i.second][i.first] = '#';
            direction = N;
            searching = true;
            pos = startPos;
            int visitedDupes = 0; // Visited in a row..
            print("Placing block at %d,%d\n", i.first, i.second);
#ifdef GIF
            GifWriter g;
            GifBegin(&g, std::format("{}{}.gif", argv[1], ii++).c_str(), map[0].size(), map.size(), 10);
#endif
            while(searching) {
                char next;
                switch(direction) {
                    case N:
                        if(pos.second == 0) {
                            searching = false;
                            break;
                        }
                        next = map[pos.second - 1][pos.first];
                        if(next == '#') {
                            direction = E;
                            print("Direction: E\n");
                        } else {
                            if(next == 'x') {
                                visitedDupes++;
                            }
                            prevPath.insert(prevPath.begin(), pos);
                            pos.second--;
                        }
                        break;
                    case E:
                        if(pos.first == (map[0].size() - 1)) {
                            searching = false;
                            break;
                        }
                        next = map[pos.second][pos.first + 1];
                        if(map[pos.second][pos.first + 1] == '#') {
                            direction = S;
                            print("Direction: S\n");
                        } else {
                            if(next == 'x') {
                                visitedDupes++;
                            }
                            prevPath.insert(prevPath.begin(), pos);
                            pos.first++;
                        }
                        break;
                    case S:
                        if(pos.second == (map.size() - 1)) {
                            searching = false;
                            break;
                        }
                        next = map[pos.second + 1][pos.first];
                        if(map[pos.second + 1][pos.first] == '#') {
                            direction = W;
                            print("Direction: W\n");
                        } else {
                            if(next == 'x') {
                                visitedDupes++;
                            }
                            prevPath.insert(prevPath.begin(), pos);
                            pos.second++;
                        }
                        break;
                    case W:
                        if(pos.first == 0) {
                            searching = false;
                            break;
                        }
                        next = map[pos.second][pos.first - 1];
                        if(map[pos.second][pos.first - 1] == '#') {
                            direction = N;
                            print("Direction: N\n");
                        } else {
                            if(next == 'x') {
                                visitedDupes++;
                            }
                            prevPath.insert(prevPath.begin(), pos);
                            pos.first--;
                        }
                        break;
                }
                int repeatLimit = 3;
                if(prevPath.size() > repeatLimit) {
                    prevPath.erase(prevPath.begin() + repeatLimit, prevPath.end());
                    // Check that the path does not repeat back on itself
                    print("Pos = %d,%d  prev = %d,%d  %d,%d  %d,%d\n", pos.first, pos.second, prevPath[0].first, prevPath[0].second, prevPath[1].first, prevPath[1].second, prevPath[2].first, prevPath[2].second);
                    if(contains(prevPath, pos)) {
                        print("Already visited\n");
//                        searching = false;
//                        break;
                    }
                }
#ifdef GIF
                writeMapToGif(g, map);
#endif       
                map[pos.second][pos.first] = 'x';
                if(visitedDupes == path.size()) {// Ensure that the loop is sufficient length
                    break;                // and not back on itself
                }
            }
            prevPath.resize(0);
            if(searching) { // Ensure that the puzzle did not exit due to map edge.
                sum++;
            }
#ifdef GIF
            GifEnd(&g);
#endif
        }
        printf("Possible loops = %d\n", sum);
    }
    return 0;
}


