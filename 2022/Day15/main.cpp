#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

std::ifstream input;
std::pair<int, int> limits{0, 0};

struct Pos {
    int x;
    int y;
    Pos() = default;
    Pos(int x, int y) : x(x), y(y){
        if(x > limits.first) {
            limits.first = x;
        }
        if(x < limits.second) {
            limits.second = x;
        }
    };
};

enum State {
    False,
    True,
    Beacon
};

struct Line {
    Pos sensor;
    Pos beacon;
    int radius;
    Line() = default;
    Line(Pos s, Pos b) : sensor(s), beacon(b), radius(abs(sensor.x - beacon.x) + abs(sensor.y - beacon.y)){};
};

std::vector<Line> lines;

Pos createPos(const std::string &s) {
    return {
        std::stoi(s.substr(0, s.find(','))),
        std::stoi(s.substr(s.find('=') + 1, s.size()))
    };
}

void processLine(std::string &s) {
    std::size_t pos;
    Pos p1 = createPos(s.erase(0, s.find('=') + 1).substr(0, pos = s.find(':')));
    s.erase(0, pos);
    pos = s.find('=') + 1;
    s.erase(0, pos);
    Pos p2 = createPos(s);
    lines.emplace_back(p1, p2);
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
    input.open("Day15.txt");
    if(!input.is_open()) {
        return -1;
    }
    // Process the lines of the file, adding new RockPath elements to paths
    std::string temp;
    while(getline(input, temp)) {
        processLine(temp);
    }
    int line = 1000000;
    std::vector<State> cover(abs(limits.first - limits.second), False);
    for(auto &i: lines) {
        int deltaR = abs(line - i.sensor.y);
        if(deltaR <= i.radius) {
            int fromPos = i.radius - deltaR;
            for(int j = 0; j < (fromPos * 2) + 1; j++) {
                int pos = (abs(limits.first) + i.sensor.x - fromPos) + j;
                State &arrayPos = cover[pos];
                if(i.beacon.y == line && i.beacon.x == pos - abs(limits.first)) {
                    arrayPos = Beacon;
                } else {
                    arrayPos = True;
                }
            }
            // for(auto j: cover) {
            //     printf("%s\t", j == True ? "True" : "False");
            // }
            // printf("\n");
        }
    }
    printf("Elements not available in line = %zu\n", std::count(cover.begin(), cover.end(), True));
    return 0;
}
