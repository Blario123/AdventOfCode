#include <cstdio>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <cmath>

int checkLine(int goal, const std::vector<long> &v) {
    int possPermutations = std::pow(2, v.size() - 1);
    // Cheaper to check the addition and multiplication permutations immediately.
    int addSum = v[0];
    int mulSum = v[0];
    for(int i = 1; i < v.size(); i++) {
        addSum += v[i];
        mulSum *= v[i];
    }
    if(addSum == goal || mulSum == goal) {
        return goal;
    }
    // If neither worked, determine a mix of symbols
    int tried = 1;
    while(tried != possPermutations - 1) {
        int mixSum = v[0];
        // 0b0 == +
        // 0b1 == *
        int mask = 0b1;
        //std::string binRep = "";
        for(int i = 1; i < v.size(); i++) {
            if(tried & mask) {
                mixSum *= v[i];
            } else {
                mixSum += v[i];
            }
            //binRep.append((tried & mask) ? "*" : "+");
            mask = mask << 1;
        }
        if(mixSum == goal) {
            return goal;
        }
        tried++;
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
            std::vector<long> v;
            while(sz != std::string::npos) {
                printf("%s\n", line.substr(sz_, std::string::npos).c_str());
                sz = line.find(' ', sz_);
                if(sz == std::string::npos) {
                    printf("end of line found\n");
                }
                try {
                    v.emplace_back(std::stol(line.substr(sz_, sz).c_str()));
                } catch(const std::out_of_range& err) {
                    std::cerr << "out of range: " << err.what();
                }
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


