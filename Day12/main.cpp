#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

std::ifstream input;
std::vector<std::string> lines;

struct Pos {
    int x = 0;
    int y = 0;
    Pos() = default;
    Pos(int pX, int pY) : x(pX), y(pY) {
    }
};

struct Node {
    Pos pos;
    char c = 0;
    int depth = 0;
    std::vector<Node> children;
    Node() = default;
    Node(Pos pPos, char pC, int pDepth) : pos(pPos), c(pC), depth(pDepth) {
    }
};

std::vector<Pos> visitedPositions;

Pos currPos;
bool isVisited(const Pos &p) {
//    printf("isVisited processing pos %i,%i against currPos of %i,%i\n", p.x, p.y, currPos.x, currPos.y);
    if(p.x == currPos.x && p.y == currPos.y) {
//        printf("Already visited\n");
        return true;
    }
    return false;
}

void processNode(Node &n) {
    // Search around the node for possible children
    if(n.c == 'S') {
        visitedPositions.push_back(n.pos);
        n.c = 'a' - 1;
    }
    if(n.c == 'z') {
        printf("Depth = %i\n", n.depth - 1);
        n.c = 'E' - 1;
    }
    printf("Node %c at %i,%i\n", n.c, n.pos.x, n.pos.y);
    // Check directly up and down
    std::vector<Pos> pointsToAdd;
    for(int i = n.pos.y - 1; i < n.pos.y + 2; i++) {
        if(i < 0 || i >= lines.size() || i == n.pos.y) {
            continue;
        }
        if(lines[i][n.pos.x] == n.c || lines[i][n.pos.x] == n.c + 1) {
            printf("UD Point of interest = %c at %i,%i\n", lines[i][n.pos.x], n.pos.x, i);
            pointsToAdd.emplace_back(Pos(n.pos.x, i));
        }
    }
    // Check directly left and right
    for(int i = n.pos.x - 1; i < n.pos.x + 2; i++) {
        if(i < 0 || i >= lines[0].size() || i == n.pos.x) {
            continue;
        }
        if(lines[n.pos.y][i] == n.c || lines[n.pos.y][i] == n.c + 1) {
            printf("LR Point of interest = %c at %i,%i\n", lines[n.pos.y][i], i, n.pos.y);
            pointsToAdd.emplace_back(Pos(i, n.pos.y));
        }
    }
    for(auto &i: pointsToAdd) {
//        printf("pointsToAdd processing %i,%i\n", i.x, i.y);
        currPos = i;
        if(!std::any_of(visitedPositions.begin(), visitedPositions.end(), isVisited)) {
            Node newNode;
            newNode.pos = i;
            newNode.c = lines[i.y][i.x];
            newNode.depth = n.depth + 1;
            n.children.push_back(newNode);
            visitedPositions.emplace_back(i);
//            printf("Adding new pos %c at %i,%i\n", newNode.c, newNode.pos.x, newNode.pos.y);
        }
    }
    for(auto &i: n.children) {
        processNode(i);
    }
}

int main(int argc, char *argv[]) {
    // Template for command line arguments
#if 0
    if(argc > 1) {
         = std::atoi(argv[1]);
    } else {
        std::cout << "Usage: " << argv[0] << " " << std::endl;
        return 0;
    }
#else 
    (void) argc;
    (void) argv;
#endif
    // Load file and process the lines
    input.open("Day12.txt");
    if(!input.is_open()) {
        return -1;
    }
    Node start;
    Pos end;
    std::string temp;
    while(getline(input, temp)) {
        lines.emplace_back(temp);
        if(temp.find('S') != std::string::npos) {
            start.pos.x = (int) temp.find('S');
            start.pos.y = (int) lines.size() - 1;
            start.c = 'S';
        }
        if(temp.find('E') != std::string::npos) {
            end.x = (int) temp.find('E');
            end.y = (int) lines.size() - 1;
        }
    }
    printf("Start node pos = %i,%i\n", start.pos.x, start.pos.y);
    printf("End node pos = %i,%i\n", end.x, end.y);
    processNode(start);
    return 0;
}

#pragma clang diagnostic pop