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

std::vector<Pos> visitedPosPt1;
std::vector<Pos> visitedPosPt2;

bool isVisited(const Pos &p, std::vector<Pos> &v) {
    // Checks if a position exists in a vector
    // If both p.x and p.y exist as an element in v, return true
    if(std::any_of(v.cbegin(), v.cend(), [&p](Pos i){
        return (i.x == p.x && i.y == p.y);
    })) {
        return true;
    }
    return false;
}

struct Rope {
    // Create tail vector of 9 elements for the 10 element rope
    std::vector<Pos> ropeVec = std::vector<Pos>(10);
    void processMove(int &dx, int &dy, int pos) {
        // If the delta of X/Y is greater than 1, the tail[pos] must move
        if(abs(dx) > 1) {
            ropeVec[pos].x += dx/abs(dx);
            // If there is an offset in the opposite axis, (in/de)crease
            // the opposite axes according to the sign of said axis.
            if(abs(dy) > 0) {
                ropeVec[pos].y += dy/abs(dy);
            }
            return;
        }
        // If the delta of X/Y is greater than 1, the tail[pos] must move
        if(abs(dy) > 1) {
            ropeVec[pos].y += dy/abs(dy);
            // If there is an offset in the opposite axis, (in/de)crease
            // the opposite axes according to the sign of said axis.
            if(abs(dx) > 0) {
                ropeVec[pos].x += dx/abs(dx);
            }
            return;
        }
    }
    void move(const Move &m) {
        for(int i = 0; i < m.count; i++) {
            // Always increase the head, ropeVec[0], by the direction dictated by the move
            switch(m.direction) {
                case Up:
                    ropeVec[0].y++;
                    break;
                case Down:
                    ropeVec[0].y--;
                    break;
                case Left:
                    ropeVec[0].x--;
                    break;
                case Right:
                    ropeVec[0].x++;
                    break;
            }
            // Iterate through all the tail elements ropeVec[1:9]
            // And move them using processMove
            for(int j = 1; j < ropeVec.size(); j++) {
                int deltaX = ropeVec[j - 1].x - ropeVec[j].x;
                int deltaY = ropeVec[j - 1].y - ropeVec[j].y;
                processMove(deltaX, deltaY, j);
            }
            // Process the position for Part 1 and Part 2
            // If the element already exists, don't add it to the vector
            if(!isVisited(ropeVec[1], visitedPosPt1)) {
                visitedPosPt1.push_back(ropeVec[1]);
            }
            if(!isVisited(ropeVec[9], visitedPosPt2)) {
                visitedPosPt2.push_back(ropeVec[9]);
            }
        }
    }
};

Direction processDirection(const std::string &s) {
    // Convert the string direction into the corresponding enum value
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

Move processMove(const std::string &s) {
    // From the line given, extract the direction character
    // And the number of spaces to move
    Move tempMove{};
    tempMove.direction = processDirection(s.substr(0, 1));
    tempMove.count = std::stoi(s.substr(2, s.size() - 2));
    return tempMove;
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
    Rope rope;
    std::string temp;
    while(getline(input, temp)) {
        rope.move(processMove(temp));
    }
    std::cout << "Move count of tail[1] = " << visitedPosPt1.size()  << std::endl;
    std::cout << "Move count of tail[9] = " << visitedPosPt2.size()  << std::endl;
    return 0;
}

// less than 102456
