#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

std::vector<std::string> contents;

bool checkSubstring(const std::string &str) {
    return false;
}

int multiplySubstring(const std::string &str) {
    size_t sz = 0;
    int n1 = std::stoi(str, &sz);
    int n2 = std::stoi(str.substr(sz));
    return n2 * n2;
}

int main(int argc, char** argv) {
    if(argc > 1) {
        // Data input
        std::ifstream input;
        input.open(argv[1]);

        std::string line;
        getline(input, line, '\n');
        while(input.good()) {
            contents.emplace_back(line);
            getline(input, line, '\n');
        }
        // Day 1 - Find and multiply
        size_t sz = 0;
        size_t sz_ = 0;
        int sum = 0;
        for(auto &i: contents) {
            while(sz != std::string::npos) {
                sz = i.find("mul(", sz_);
                sz_ = sz + 4;
                std::string mulStr = i.substr(sz_, i.find(")", sz_) - sz_);
                if(checkSubstring(mulStr)) {
                    sum += multiplySubstring(mulStr);
                }
            }
        }
    }
    return 0;
}


