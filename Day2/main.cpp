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

int main(int argc, char *argv[]) {
    std::ifstream file;
    file.open("Day2.txt");
    if(!file.is_open()) {
        return -1;
    }
    std::string temp;
    int totalScore;
    while(getline(file, temp)) {
        totalScore += getScore(temp.front(), temp.back());
    }
    std::cout << totalScore << std::endl;
    return 0;
}
