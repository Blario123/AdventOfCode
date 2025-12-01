#include <cstdio>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <cstdint>

std::vector<std::vector<int>> map;
std::vector<std::pair<int, int>> startingLocations;

void processLine(const std::string &l) {
    map.emplace_back();
    for(const auto& i: l) {
        map[map.size() - 1].emplace_back(i - '0');
    }
}

void printMap() {
    for(auto &i: map) {
        for(auto &j: i) {
            printf("%d", j);
        }
        printf("\n");
    }
}

void getTrailheads() {
    startingLocations.clear();
    for(int i = 0; i < map.size(); i++) {
        for(int j = 0; j < map[i].size(); j++) {
            if(map[i][j] == 0) {
                startingLocations.emplace_back(std::pair<int,int>(i,j));
            }
        }
    }
}

void printTrailheads() {
    for(const auto& i: startingLocations) {
        printf("Start at %d,%d\n", i.first, i.second);
    }
}

int calcPaths(std::pair<int,int> pos) {
    int sum = 0;
    int current = map[pos.first][pos.second];
    printf("pos is %d,%d\n", pos.first, pos.second);
    for(int i = pos.first - 1; i <= pos.first + 1; i++) {
        printf("i = %d\n", i);
        // If vertical is within bounds
        if(i >= 0 && i < map.size()) {
            for(int j = pos.second - 1; j <= pos.second + 1; j++) {
                printf("j = %d\n", j);
                // If horizontal is within bounds
                if(j >= 0 && j < map[0].size()) {
                    if(!(i == pos.first && j == pos.second)) {
                        int check = map[i][j];
                        printf("Checking if %d is a step from %d\n", check, current);
                        if(check == (current + 1)) {
                            sum += calcPaths(std::pair<int,int>(i,j));
                        } else if(check == 9 && current == 8) {
                            return 1;
                        }
                    }
                }
            }
        }
    }
    return sum;
}

int main(int argc, char** argv) {
    if(argc > 1) {
        // Data input
        std::ifstream input;
        input.open(argv[1]);
        std::string line;
        getline(input, line, '\n');
        while(input.good()) {
            processLine(line);
            getline(input, line, '\n');
        }
        printMap();
    }
    getTrailheads();
    int sum = 0;
    printTrailheads();
    for(const auto& i: startingLocations) {
        printf("Total paths = %d\n", calcPaths(i)); 
        break;
    }
    printf("%d\n", sum);
    return 0;
}

