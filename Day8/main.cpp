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

bool checkLine(const std::vector<int> &v, int pos) {
    // Return true for hidden, false for visible
    bool isHidden = true;
    // If the position of the tree is surrounded by equal or taller trees
    // Return isHidden immediately
    if((v[pos - 1] >= v[pos]) && (v[pos + 1] >= v[pos])) {
        return isHidden;
    }
    for(int i = 0; i < pos; i++) {
        if(v[pos] > v[i]) {
            isHidden = false;
        } else {
            isHidden = true;
        }
    }
    if(isHidden) {
        if(v[pos + 1] < v[pos]) {
            for(int i = ++pos; i < v.size(); i++) {
                if(v[pos] > v[i]) {
                    isHidden = false;
                } else {
                    isHidden = true;
                }
            }
        }
    }
    std::cout << ((isHidden) ? "Hidden" : "Visible") << std::endl;
    return isHidden;
}

bool verifyTree(int tree, int y, int x) {
    std::cout << "Checking tree [" << x << "," << y << "] of value " << tree << std::endl;
    // Create temporary vectors to be processed to check for height differences
    std::vector<int> vertical, horizontal;
    horizontal = vector[y];
    for(auto i : vector) {
        vertical.push_back(i[x]);
    }
    if(!checkLine(vertical, y)) {
        return false; // If visible verically, return false
    } else {
        return checkLine(horizontal, x); // If visible, returns true, else returns false
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
    input.open("Day8.txt");
    if(!input.is_open()) {
        return -1;
    }
    std::string temp;
    while(getline(input, temp)) {
        createRow(temp);
    }
    // Calculate no. of all outside trees
    // Using the product of the size of vector by the size of the first element in vector.
    // Remove the inside product by removing 2 from each size.
    unsigned long visible = (vector.size() * vector[0].size()) - ((vector.size() - 2) * (vector[0].size() - 2));
    // Check all the inner trees
    for(int i = 1; i < vector.size() - 1; i++) {
        for(int j = 1; j < vector[i].size() - 1; j++) {
            if(verifyTree(vector[i][j], i, j)) {
                visible++;
            }
        }
        std::cout << std::endl;
    }
    std::cout << "Total visible trees = " << visible << std::endl;
    return 0;
}
