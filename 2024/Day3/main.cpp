#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

std::vector<std::string> contents;

bool checkSubstring(const std::string &str) {
    std::string toCheck = " abcdefghijklmnopqrstuvwxyz#~()[]{}!\"£$%^&*-_=+;:'@<.>/?\\|";
    for(auto &i: toCheck) {
        if(str.find(i) != std::string::npos) {
            return false;
        }
    }
    int commaCount = 0;
    size_t commaPos = 0;
    while(commaPos != std::string::npos) {
        commaCount++;
        commaPos = str.find(',', commaPos + 1);
    }
    commaCount -= 1;
    if(commaCount != std::string::npos && commaCount > 1) {
        return false;
    }
    return true;
}

int multiplySubstring(const std::string &str) {
    size_t sz = 0;
    int n1 = std::stoi(str.substr(0, str.find(",")), &sz);
    int n2 = std::stoi(str.substr(sz + 1));
    return n1 * n2;
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
        int sum = 0;
        for(auto &i: contents) {
            size_t sz = 0;
            size_t sz_ = 0;
            while(sz != std::string::npos) {
                sz = i.find("mul(", sz_);
                sz_ = sz + 4;
                std::string mulStr = i.substr(sz_, i.find(")", sz_) - sz_);
                if(checkSubstring(mulStr)) {
                    sum += multiplySubstring(mulStr);
                }
            }
        }
        printf("Corrupted memory = %d\n", sum);
        // Day 2 - Find and multiply + do() and dont() actions
        sum = 0;
        bool perform = true;
        for(auto &i: contents) {
            size_t sz = 0;
            size_t sz_ = 0;
            while(sz != std::string::npos) {
                int action = 1;
                // Determine if mul, do or dont is closer
                size_t mulPos = i.find("mul(", sz_);
                size_t doPos = i.find("do()", sz_);
                size_t dontPos = i.find("don't()", sz_);
                size_t smallest = mulPos;
                if(smallest > doPos) {
                    action = 2;
                    smallest = doPos;
                }
                if(smallest > dontPos) {
                    action = 3;
                    smallest = dontPos;
                }
                sz = smallest;
                sz_ = sz + 4;
                std::string mulStr;
                switch(action) {
                    case 1: // mul
                        if(perform) {
                            mulStr = i.substr(sz_, i.find(")", sz_) - sz_);
                            if(checkSubstring(mulStr)) {
                                sum += multiplySubstring(mulStr);
                            }
                        }
                        break;
                    case 2: // do
                        perform = true;
                        break;
                    case 3: // dont
                        perform = false;
                        sz_ += 3;
                        break;
                }
            }
        }
        printf("Filtered corrupted memory = %d\n", sum);
    }
    return 0;
}


