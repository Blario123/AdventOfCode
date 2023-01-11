#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

std::ifstream input;
struct Rope;
struct Move;

enum Direction {
    Up,
    Down,
    Left,
    Right
};

std::vector<std::string> directionNames = {"Up", "Down", "Left", "Right"};

struct Move {
    Direction direction;
    int count;
};

struct Pos {
    int x = 0;
    int y = 0;
};

std::vector<Pos> visitedPosPt1;
std::vector<Pos> visitedPosPt2;

bool isVisited(const Pos &p, std::vector<Pos> &v) {
    bool alreadyVisited = false;
    if(std::any_of(v.cbegin(), v.cend(), [&p](Pos i){
        return (i.x == p.x && i.y == p.y);
    })) {
        alreadyVisited = true;
    }
    return alreadyVisited;
}

struct Rope {
    Pos head;
    // Create tail vector of 9 elements for the 10 element rope
    std::vector<Pos> tail = std::vector<Pos>(9);

    void move(const Direction &d, int c) {
        std::cout << "Move " << directionNames[d] << " by " << c << std::endl;
        for(int i = 0; i < c; i++) {
            std::cout << "Move: " << i << std::endl;
            switch(d) {
                case Up:
                    head.y++;
                    break;
                case Down:
                    head.y--;
                    break;
                case Left:
                    head.x--;
                    break;
                case Right:
                    head.x++;
                    break;
            }
            std::cout << "head pos = " << head.x << "," << head.y << std::endl;
            for(int j = 0; j < tail.size(); j++) {
                int deltaX, deltaY;
                if(j == 0) {
                    deltaX = head.x - tail[j].x;
                    deltaY = head.y - tail[j].y;
                } else {
                    deltaX = tail[j - 1].x - tail[j].x;
                    deltaY = tail[j - 1].y - tail[j].y;
                }
//                std::cout << "deltaX = " << deltaX << " deltaY = " << deltaY << std::endl;
                if(abs(deltaX) > 1) {
                    if(deltaY != 0) {
                        if(deltaY < 0) {
                            tail[j].y--;
                        } else {
                            tail[j].y++;
                        }
                    }
                    // Correct for the sign of deltaX
                    if(deltaX < 0) {
                        tail[j].x--;
                    } else {
                        tail[j].x++;
                    }
                }
                if(abs(deltaY) > 1) {
                    if(deltaX != 0) {
                        if(deltaX < 0) {
                            tail[j].x--;
                        } else {
                            tail[j].x++;
                        }
                    }
                    // Correct for the sign of deltaY
                    if(deltaY < 0) {
                        tail[j].y--;
                    } else {
                        tail[j].y++;
                    }
                }
                std::cout << "tail[" << j << "] pos = " << tail[j].x << "," << tail[j].y << std::endl;
                if(!isVisited(tail[0], visitedPosPt1)) {
                    visitedPosPt1.push_back(tail[0]);
                }
                if(!isVisited(tail[8], visitedPosPt2)) {
                    visitedPosPt2.push_back(tail[8]);
                }
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
};

std::vector<Move> moves;

Direction processDirection(const std::string &s) {
    if(s == "U") {
        return Up;
    } else if(s == "D") {
        return Down;
    } else if(s == "L") {
        return Left;
    } else {
        return Right;
    }
}

void processMove(const std::string &s) {
    Move tempMove{};
    tempMove.direction = processDirection(s.substr(0, 1));
    tempMove.count = std::stoi(s.substr(2, s.size() - 2));
    moves.push_back(tempMove);
}

Rope rope;

void run() {
    for(auto i: moves) {
        rope.move(i.direction, i.count);
    }
}

int main(int argc, char *argv[]) {
    // Template for command line arguments
#if 0
    if(argc > 1) {
         = atoi(argv[1]);
    } else {
        std::cout << "Usage: " << argv[0] << "" << std::endl;
        return 0;
    }
#else
    (void) argc;
    (void) argv;
#endif
    // Load file and process the lines
    input.open("Day9.txt");
    if(!input.is_open()) {
        return -1;
    }
    std::string temp;
    while(getline(input, temp)) {
        processMove(temp);
    }
    run();
    std::cout << "Move count of tail[0] = " << visitedPosPt1.size()  << std::endl;
    std::cout << "Move count of tail[8] = " << visitedPosPt2.size()  << std::endl;
    return 0;
}

// less than 102456
