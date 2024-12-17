#include <fstream>
#include <string>
#include <vector>

std::vector<std::string> contents;

bool checkSubstring(const std::string &str) {
    printf("Checking string \"%s\"\n", str.c_str());
    if(str.find(' ') != std::string::npos) {
        printf("Contains a space\n");
        return false;
    }
    for(char i = 'a'; i < 'z'; i++) {
        if(str.find(i) != std::string::npos) {
            printf("Contains illegal characters\n");
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
    printf("%d\n", commaCount);
    if(commaCount != std::string::npos && commaCount > 1) {
        printf("Too many commas\n");
        return false;
    }
    return true;
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
        int sum = 0;
        for(auto &i: contents) {
            size_t sz_ = 0;
            while(sz != std::string::npos) {
                sz = i.find("mul(", sz_);
                sz_ = sz + 4;
                std::string mulStr = i.substr(sz_, i.find(")", sz_) - sz_);
                if(checkSubstring(mulStr)) {
                    // sum += multiplySubstring(mulStr);
                } else {
                    printf("Check failed\n");
                }
            }
        }
    }
    return 0;
}


