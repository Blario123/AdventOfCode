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
    bool isHidden = false;
    // Check left of position
    for(int i = 0; i < pos; i++) {
        if(v[i] >= v[pos]) {
            isHidden = true;
            break;
        }
    }
    // If the tree is already visible from the left of the tree position
    // Return isHidden
    if(!isHidden) {
        return isHidden;
    }
    // If the tree is still hidden, carry on processing right of the tree position
    for(int i = pos + 1; i < v.size(); i++) {
        // If any tree is taller or the same, return true;
        if(v[i] >= v[pos]) {
            return true;
        }
    }
}

bool verifyTree(int y, int x) {
    // Create temporary vectors to be processed to check for height differences
    std::vector<int> vertical, horizontal;
    // Create a vector if the row using the current element of vector
    horizontal = vector[y];
    // Create a vert. vector using all the elements at position x through vectors elements
    for(auto i : vector) {
        vertical.push_back(i[x]);
    }
    // If visible verically, return false
    if(!checkLine(vertical, y)) {
        return false;
    }
    // If visible, returns true, else returns false
    return checkLine(horizontal, x);
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
            if(!verifyTree(i, j)) {
                visible++;
            }
        }
    }
    std::cout << "Total visible trees = " << visible << std::endl;
    return 0;
}
