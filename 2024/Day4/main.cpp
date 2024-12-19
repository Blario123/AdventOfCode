#include <cstdio>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

std::vector<std::string> wordSearch;
std::vector<std::pair<int,int>> xPositions;
std::vector<std::pair<int,int>> aPositions;

int checkXMAS(const std::pair<int,int> &p) {
    int count = 0;
    std::string wordToCheck;
    if(p.first < wordSearch[0].length() - 3) {
        wordToCheck = wordSearch[p.second].substr(p.first, 4);
        if(wordToCheck == "XMAS") {
				count++;
        }
        if(p.second < wordSearch.size() - 3) {
            for(int i = 0; i < 4; i++) {
                wordToCheck[i] = wordSearch[p.second + i][p.first + i];
            }
            if(wordToCheck == "XMAS") {
				count++;
            }
        }
        if(p.second >= 3) {
            for(int i = 0; i < 4; i++) {
                wordToCheck[i] = wordSearch[p.second - i][p.first + i];
            }
            if(wordToCheck == "XMAS") {
				count++;
            }
        }
    }
    if(p.first >= 3) {
        wordToCheck = wordSearch[p.second].substr(p.first - 3, 4);
        std::reverse(wordToCheck.begin(), wordToCheck.end());
        if(wordToCheck == "XMAS") {
            count++;
        }
        if(p.second < wordSearch.size() - 3) {
            for(int i = 0; i < 4; i++) {
                wordToCheck[i] = wordSearch[p.second + i][p.first - i];
            }
            if(wordToCheck == "XMAS") {
				count++;
            }
        }
        if(p.second >= 3) {
            for(int i = 0; i < 4; i++) {
                wordToCheck[i] = wordSearch[p.second - i][p.first - i];
            }
            if(wordToCheck == "XMAS") {
				count++;
            }
        }
    }
    if(p.second < wordSearch.size() - 3) {
        for(int i = 0; i < 4; i++) {
            wordToCheck[i] = wordSearch[p.second + i][p.first];
        }
        if(wordToCheck == "XMAS") {
            count++;
        }
    }
    if(p.second >= 3) {
        for(int i = 0; i < 4; i++) {
            wordToCheck[i] = wordSearch[p.second - i][p.first];
        }
        if(wordToCheck == "XMAS") {
            count++;
        }
    }
    return count;
}


int checkMAS(const std::pair<int,int> &p) {
    int count = 0;
    std::string wordToCheck;
    if(p.first > 0 && p.first < (wordSearch[0].size() - 1) && p.second > 0 && p.second < (wordSearch.size() - 1)) {
        wordToCheck = "...";
        for(int i = 0; i < 3; i++) {
            wordToCheck[i] = wordSearch[p.second + (i - 1)][p.first + (i - 1)];
        }
        if(wordToCheck == "MAS" || wordToCheck == "SAM") {
            for(int i = 0; i < 3; i++) {
                wordToCheck[i] = wordSearch[p.second + (i - 1)][p.first - (i - 1)];
            }
            if(wordToCheck == "MAS" || wordToCheck == "SAM") {
                count++;
            }
        }
    }
    return count;
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
            size_t szX = 0;
            while(szX != std::string::npos) {
                szX = line.find('X', szX);
                if(szX != std::string::npos) {
                    xPositions.emplace_back(std::pair<int,int>(szX, wordSearch.size() - 1));
                    szX++;
                }
            }
            size_t szA = 0;
            while(szA != std::string::npos) {
                szA = line.find('A', szA);
                if(szA != std::string::npos) {
                    aPositions.emplace_back(std::pair<int,int>(szA, wordSearch.size() - 1));
                    szA++;
                }
            }
            getline(input, line, '\n');
        }
        int sum = 0;
        // Day 1 - Finding all XMASs
        for(const auto &i: xPositions) {
            sum += checkXMAS(i);
        }
        printf("XMAS count = %d\n", sum);
        // Day 2 - Finding all MAS in a X pattern
        sum = 0;
        for(const auto &i: aPositions) {
            sum += checkMAS(i);
        }
        printf("X-MAS count = %d\n", sum);
    }
    return 0;
}


