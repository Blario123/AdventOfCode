#include <cstdio>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

std::vector<std::string> wordSearch;
std::vector<std::pair<int,int>> xPositions;

bool checkWord(const std::pair<int,int> &p) {
    printf("Checking X at %d,%d\n", p.first, p.second);
    std::string wordToCheck;
    if(p.first < wordSearch[0].length() - 4) {
        wordToCheck = wordSearch[p.second].substr(p.first, 4);
        if(wordToCheck == "XMAS") {
            return true;
        }
    }
    if(p.first > 4) {
        wordToCheck = wordSearch[p.second].substr(p.first - 3, 4);
        std::reverse(wordToCheck.begin(), wordToCheck.end());
        if(wordToCheck == "XMAS") {
            return true;
        }
    }
    if(p.second < wordSearch.size() - 4) {
        for(int i = 0; i < 4; i++) {
            wordToCheck[i] = wordSearch[p.second + i][p.first];
        }
        if(wordToCheck == "XMAS") {
            return true;
        }
    }
    if(p.second > 3) {
        for(int i = 0; i < 4; i++) {
            wordToCheck[i] = wordSearch[p.second - i][p.first];
        }
        if(wordToCheck == "XMAS") {
            return true;
        }
    }
    return false;
}

int main(int argc, char** argv) {
    if(argc > 1) {
        // Data input
        std::ifstream input;
        input.open(argv[1]);

        std::string line;
        getline(input, line, '\n');
        while(input.good()) {
            wordSearch.emplace_back(line);
            size_t sz = 0;
            while(sz != std::string::npos) {
                sz = line.find('X', sz);
                if(sz != std::string::npos) {
                    xPositions.emplace_back(std::pair<int,int>(sz, wordSearch.size() - 1));
                    sz++;
                }
            }
            getline(input, line, '\n');
        }
        int sum = 0;
        for(auto &i: xPositions) {
            if(checkWord(i)) {
                sum++;
            }
        }
        printf("XMAS count = %d\n", sum);
    }
    return 0;
}


