#include <cstdio>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

int checkLine(int goal, const std::vector<int> &v) {
    // Check all additions
    int addSum = v[0];
    int mulSum = v[0];
    for(int i = 0; i < v.size(); i++) {
        addSum += v[i];
        mulSum *= v[i];
    }
    if(addSum == goal || mulSum == goal) {
        return goal;
    }
    // If neither worked, determine a mix of symbols
    for(int i = 0; i < v.size(); i++) {
        
    }
    return -1; 
}

int main(int argc, char** argv) {
    if(argc > 1) {
        // Data input
        std::ifstream input;
        input.open(argv[1]);

        std::string line;
        getline(input, line, '\n');
        int sum = 0;
        // Day 1 - (built in to processing) Check sum of line (+/*)
        size_t sz = 0;
        while(input.good()) {
            sz = line.find(':');
            int g = std::stoi(line.substr(0, sz++).c_str());
            size_t sz_ = sz + 1;
            std::vector<int> v;
            while(sz != std::string::npos) {
                sz = line.find(' ', sz_);
                v.emplace_back(std::stoi(line.substr(sz_, sz).c_str()));
                sz_ = sz + 1;
            }
            int l = checkLine(g, v);
            if(l != -1) {
                sum += l; 
            }
            getline(input, line, '\n');
        }
        printf("Count of working lines = %d\n", sum);
    }
    return 0;
}


