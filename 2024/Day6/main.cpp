#include <cstdio>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <cstdarg>
#include <thread>
#include <mutex>
#include <chrono>

#ifdef GIF
    #include "gif.h"
#endif
typedef enum {
    N = 0,
    E,
    S,
    W
} Direction;

struct Path {
    explicit Path(std::vector<std::pair<int,int>> p_) : p(p_) {};
    std::vector<std::pair<int,int>> p;
    std::mutex m;
    void set(std::vector<std::pair<int,int>> p_) {
        p = p_;
    }
};

struct Sum {
    explicit Sum(int s) : sum(s) {};
    int sum;
    std::mutex m;
    void set(int s) {
        sum = s;
    }
};

std::vector<std::string> map;
std::vector<std::string> mapRaw;
std::vector<std::pair<int,int>> path;
std::pair<int,int> pos;
std::pair<int,int> startPos;
Direction direction = N;

std::vector<std::pair<int,int>> prevPath;
std::vector<std::thread> workers;

Path t_p(path);
Sum t_s(0);

int sum = 0;

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
    printf("%d, %d\n", p.first, p.second);
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

void workerProcessMapBlock(int id) {
    printf("Thread %d spawned.\n", id);
    // Copy the map to local thread
    std::vector<std::pair<int,int>> t_prevPath;
    fflush(stdout);
    while(1) {
        std::vector<std::string> mapCopy = mapRaw;
        std::unique_lock pathLock(t_p.m, std::defer_lock);
        std::unique_lock sumLock(t_s.m, std::defer_lock);
        // Stop the other threads from stealing the same element.
        // If another thread is currently locking, wait until it can be unlocked.
        while(!pathLock.try_lock()) {
        }
        if(t_p.p.size() == 0) {
            // Allow the worker to exit.
            pathLock.unlock();
            break;
        }
        // Copy the first element off the array, and remove it.
        std::pair<int,int> posCopy = t_p.p[0];
        t_p.p.erase(t_p.p.begin());
        // Allow the other threads to modify the vector.
        pathLock.unlock();
        std::this_thread::sleep_for(std::chrono::duration<int, std::ratio<1, 2>>());
        mapCopy[posCopy.second][posCopy.first] = '#';
        printf("Thread %d working on (%d, %d).\n", id, posCopy.first, posCopy.second);
        // Process the loop.
        Direction t_direction = N;
        bool t_searching = true;
        std::pair<int,int> t_pos = startPos;
        int t_visitedDupes = 0; // Visited in a row..
        // print("Placing block at %d,%d\n", i.first, i.second);
        while(t_searching) {
            char next;
            switch(t_direction) {
                case N:
                    if(t_pos.second == 0) {
                        t_searching = false;
                        break;
                    }
                    next = mapCopy[t_pos.second - 1][t_pos.first];
                    if(next == '#') {
                        t_direction = E;
                    } else {
                        if(next == 'x') {
                            t_visitedDupes++;
                        }
                        t_prevPath.insert(t_prevPath.begin(), t_pos);
                        t_pos.second--;
                    }
                    break;
                case E:
                    if(t_pos.first == (mapCopy[0].size() - 1)) {
                        t_searching = false;
                        break;
                    }
                    next = mapCopy[t_pos.second][t_pos.first + 1];
                    if(mapCopy[t_pos.second][t_pos.first + 1] == '#') {
                        t_direction = S;
                    } else {
                        if(next == 'x') {
                            t_visitedDupes++;
                        }
                        t_prevPath.insert(t_prevPath.begin(), t_pos);
                        t_pos.first++;
                    }
                    break;
                case S:
                    if(t_pos.second == (mapCopy.size() - 1)) {
                        t_searching = false;
                        break;
                    }
                    next = mapCopy[t_pos.second + 1][t_pos.first];
                    if(mapCopy[t_pos.second + 1][t_pos.first] == '#') {
                        t_direction = W;
                    } else {
                        if(next == 'x') {
                            t_visitedDupes++;
                        }
                        t_prevPath.insert(t_prevPath.begin(), t_pos);
                        t_pos.second++;
                    }
                    break;
                case W:
                    if(t_pos.first == 0) {
                        t_searching = false;
                        break;
                    }
                    next = mapCopy[t_pos.second][t_pos.first - 1];
                    if(mapCopy[t_pos.second][t_pos.first - 1] == '#') {
                        t_direction = N;
                    } else {
                        if(next == 'x') {
                            t_visitedDupes++;
                        }
                        t_prevPath.insert(t_prevPath.begin(), t_pos);
                        t_pos.first--;
                    }
                    break;
                }
                int repeatLimit = 3;
                if(t_prevPath.size() > repeatLimit) {
                    t_prevPath.erase(t_prevPath.begin() + repeatLimit, t_prevPath.end());
                    // Check that the path does not repeat back on itself
                }
                mapCopy[t_pos.second][t_pos.first] = 'x';
                if(t_visitedDupes == path.size()) {// Ensure that the loop is sufficient length
                    break;                // and not back on itself
                }
            }
            t_prevPath.resize(0);
            if(t_searching) { // Ensure that the puzzle did not exit due to map edge.
                while(!sumLock.try_lock()) {
                }
                sum += 1;
                sumLock.unlock();
            }
    }
    printf("Thread %d terminating.\n", id);
}

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
        t_p.set(path);
        t_s.set(sum);
        int maxThreads = 8;
        workers.resize(maxThreads);
        for(int i = 0; i < maxThreads; i++) {
            workers[i] = std::thread(workerProcessMapBlock, i);
        }
        workers[0].join();
        workers[1].join();
        workers[2].join();
        workers[3].join();
        workers[4].join();
        workers[5].join();
        workers[6].join();
        workers[7].join();
        printf("Possible loops = %d\n", sum - 1); // 1 is removed to accommodate for an incorrect placement at the starting position of the guard.
    }
    return 0;
}


