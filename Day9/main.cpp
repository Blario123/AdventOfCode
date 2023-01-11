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

struct Move {
    Direction direction;
    int count;
};

struct Pos {
    int x = 0;
    int y = 0;
};

std::vector<Pos> visitedPos;

bool isVisited(const Pos &p) {
    bool alreadyVisited = false;
    if(std::any_of(visitedPos.cbegin(), visitedPos.cend(), [&p](Pos i){
        return (i.x == p.x && i.y == p.y);
    })) {
        alreadyVisited = true;
    }
    return alreadyVisited;
}

struct Rope {
    Pos head;
    Pos tail;
    void move(const Direction &d, int c) {
        for(int i = 0; i < c; i++) {
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
            int deltaX = head.x - tail.x;
            int deltaY = head.y - tail.y;
            if(abs(deltaX) > 1) {
                if(deltaY != 0) {
                    if(deltaY < 0) {
                        tail.y--;
                    } else {
                        tail.y++;
                    }
                }
                // Correct for the sign of deltaX
                if(deltaX < 0) {
                    tail.x--;
                } else {
                    tail.x++;
                }
            }
            if(abs(deltaY) > 1) {
                if(deltaX != 0) {
                    if(deltaX < 0) {
                        tail.x--;
                    } else {
                        tail.x++;
                    }
                }
                // Correct for the sign of deltaY
                if(deltaY < 0) {
                    tail.y--;
                } else {
                    tail.y++;
                }
            }
            if(!isVisited(tail)) {
                visitedPos.push_back(tail);
            }
        }
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

unsigned long run() {
    for(auto i: moves) {
        rope.move(i.direction, i.count);
    }
    return visitedPos.size();
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
    std::cout << "Move count = " << run() << std::endl;
    return 0;
}
