#include <iostream>
#include <fstream>
#include <vector>

std::ifstream input;

std::vector<std::vector<int>> vector;

void createRow(const std::string &s) {
    std::vector<int> t;
    for(auto i: s) {
        t.push_back(std::stoi(&i));
    }
    vector.push_back(t);
}

bool verifyTree(int tree, int y, int x) {
    std::cout << "Checking tree " << tree << std::endl;
    // Check cardinal directions
    return false;
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
    input.open("Day8.txt");
    if(!input.is_open()) {
        return -1;
    }
    std::string temp;
    while(getline(input, temp)) {
        createRow(temp);
    }
    // Calculate no. of all outside trees
    unsigned long visible = (vector.size() * vector[0].size()) - ((vector.size() - 2) * (vector[0].size() - 2));
    // Check all the inner trees
    for(int i = 1; i < vector.size() - 1; i++) {
        for(int j = 1; j < vector[i].size() - 1; j++) {
            std::cout << verifyTree(vector[i][j], i, j);
        }
        std::cout << std::endl;
    }
    std::cout << "Total visible trees = " << visible << std::endl;
    return 0;
}
