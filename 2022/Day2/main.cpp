#include <iostream>
#include <fstream>

int getScore(char elf, char player) {
    int elfCorrected = (int) elf - 64;
    int playerCorrected = (int) player - 87;
    int score = 0;
    if(elfCorrected == playerCorrected) {
        // Draw = 3
        return (3 + playerCorrected); 
    }
    switch(elfCorrected) {
        case 1:
            if(playerCorrected == 2) {
                score = 6 + playerCorrected;
            } else {
                score = playerCorrected;
            }
            break;
        case 2:
            if(playerCorrected == 3) {
                score = 6 + playerCorrected;
            } else {
                score = playerCorrected;
            }
            break;
        case 3:
            if(playerCorrected == 1) {
                score = 6 + playerCorrected;
            } else {
                score = playerCorrected;
            }
            break;
    }
    return score;
}

int getMove(char elf, char result) {
    int elfCorrected = (int) elf - 64;
    int resultCorrected = (int) result - 87;
    int score;
    switch(resultCorrected) {
        case 1:
            // Lose
            if(elfCorrected == 1) {
                score = 3;
            } else if(elfCorrected == 2) {
                score = 1;
            } else {
                score = 2;
            }
            break;
        case 2:
            // Draw
            return (3 + elfCorrected);
        case 3:
            // Win
            if(elfCorrected == 1) {
                score = 6 + 2;
            } else if (elfCorrected == 2) {
                score = 6 + 3;
            } else {
                score = 6 + 1;
            }
            break;
    }
    return score;
}

int main(int argc, char *argv[]) {
    std::ifstream file;
    file.open("Day2.txt");
    if(!file.is_open()) {
        return -1;
    }
    std::string temp;
    int totalScore;
    int totalScorePart2;
    while(getline(file, temp)) {
        totalScore += getScore(temp.front(), temp.back());
        totalScorePart2 += getMove(temp.front(), temp.back());
    }
    std::cout << "Part 1 Score: " << totalScore << std::endl;
    std::cout << "Part 2 Score: " << totalScorePart2 << std::endl;
    return 0;
}
