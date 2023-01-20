#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <queue>

std::ifstream input;
std::vector<std::string> lines;
std::vector<std::vector<bool>> visitedTable;
int height(char &c);

struct Pos {
    int x = 0;
    int y = 0;
    Pos() = default;
    Pos(int pX, int pY) : x(pX), y(pY) {
    }
    inline bool operator==(const Pos &rhs) const {
        return x == rhs.x && y == rhs.y;
    }
};

std::vector<Pos> neighbours(Pos &p) {
    std::vector<Pos> n;
    std::vector<Pos> toCheck = {Pos(p.x - 1, p.y),
                                Pos(p.x + 1, p.y),
                                Pos(p.x, p.y - 1),
                                Pos(p.x, p.y + 1)};
    for(auto i: toCheck) {
        // Ensure the position is not out of bounds of the vectors
        if(i.x < 0 || i.x >= lines[0].size()) {
            continue;
        }
        if(i.y < 0 || i.y >= lines.size()) {
            continue;
        }
        // If the height is at most 1 higher and lower, add it to the list to return
        if(height(lines[i.y][i.x]) <= height(lines[p.y][p.x]) + 1) {
            n.emplace_back(i);
        }
    }
    return n;
}

struct QueueNode {
    int dist = 0;
    Pos pos = Pos(0,0);
    QueueNode() = default;
    QueueNode(int d, Pos &p) : dist(d), pos(p) {};
};

struct CompareDist {
    bool operator()(const QueueNode &n1, const QueueNode &n2) {
        return n1.dist > n2.dist;
    }
};

// Using Dijkstra's Algorithm
void processNode(Pos &s, Pos &e) {
    // s is the start Pos, e is the end Pos
    // Create a priority queue sorted by dist from the start node
    std::priority_queue<QueueNode, std::vector<QueueNode>, CompareDist> q;
    // Add the start node with a distance of 0 to the queue
    q.emplace(0, s);
    // The current node being processed. Used once the end has been found
    QueueNode qn;
    // While it hasn't found the end position, process neighbours
    while(true) {
        // Access the first element and remove it from the queue
        qn = q.top();
        q.pop();
        // If it has been visited already skip, if not, mark it as visited
        if(visitedTable[qn.pos.y][qn.pos.x]) {
            continue;
        }
        visitedTable[qn.pos.y][qn.pos.x] = true;
        // If it is the end, break the while loop
        if(qn.pos == e) {
            break;
        }
        // Add the neighbours to the queue with an increasing distance
        for(auto &i: neighbours(qn.pos)) {
            q.emplace(qn.dist + 1, i);
        }
    }
    printf("Found character %c after %i moves\n", lines[qn.pos.y][qn.pos.x], qn.dist);
}

void createVisitedTable(const std::vector<std::string> &v) {
    // Create a row of all falses for the size of the lines
    std::vector<bool> tempRow(v[0].size(), false);
    // Create a vector of the rows for the size of the lines vector
    std::vector<std::vector<bool>> tempTable(v.size(), tempRow);
    visitedTable = tempTable;
}

int height(char &c) {
    // If the character lies within 'a' and 'z' return the relative integer
    // If the character is 'S', return 0 as it is the start
    // If the character is 'E', return 25 as it is the end
    if(c >= 'a' && c <= 'z') {
        return c - 'a';
    } else if(c == 'S') {
        return 0;
    } else if(c == 'E') {
        return 25;
    } else {
        return -1;
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
    // Allocate the start and end positions for the searching.
    Pos start;
    Pos end;
    std::string temp;
    while(getline(input, temp)) {
        lines.emplace_back(temp);
        if(temp.find('S') != std::string::npos) {
            start.x = (int) temp.find('S');
            start.y = (int) lines.size() - 1;
        }
        if(temp.find('E') != std::string::npos) {
            end.x = (int) temp.find('E');
            end.y = (int) lines.size() - 1;
        }
    }
    createVisitedTable(lines);
    processNode(start, end);
    return 0;
}
