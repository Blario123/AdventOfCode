#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

std::ifstream input;
std::vector<std::string> lines;

struct Pos {
    int x = 0;
    int y = 0;
};

struct Node {
    Pos pos;
    char c = 0;
    int depth = 0;
    std::vector<Node> children;
};

void processNode(Node &n) {
    // Search around the node for possible children
    if(n.c == 'S') {
        n.c = 'a' - 1;
    }
    for(int i = n.pos.y - 1; i < 2; i++) {
        if(i < 0 && i >= lines.size()) {
            continue;
        }
        for(int j = n.pos.x - 1; j < 2; j++) {
            if(j < 0 && j >= lines[0].size()) {
                continue;
            }
            if(lines[i][j] == n.c + 1) {
                Node newNode;
                newNode.pos.x = j;
                newNode.pos.y = i;
                newNode.c = lines[i][j];
                newNode.depth = n.depth + 1;
                n.children.push_back(newNode);
            }
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
    printf("End node pos = %i,%i\n", end.x, end.y);
    processNode(start);
    return 0;
}
