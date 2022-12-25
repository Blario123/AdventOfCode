#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

int markerLength = 14;

int getMarkerLocation(const std::string &s) {
    std::vector<char> chars;
    for(auto i: s) {
        chars.emplace_back(i);
    }
    for(int i = 0; i < chars.size(); i++) {
        std::vector<char> tempVec;
        for(int j = 0; j < markerLength; j++) {
            tempVec.emplace_back(chars[i + j]);
        }
        bool goodVec = true;
        for(int j = 0; j < tempVec.size(); j++) {
            if(std::count(tempVec.begin(), tempVec.end(), tempVec[j]) > 1) {
                goodVec = false;
            }
        }
        if(goodVec) {
            return i + markerLength;
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if(argc > 1) {
        markerLength = std::atoi(argv[1]);
    } else {
        std::cout << "Usage: " << argv[0] << " [Length of marker]" << std::endl;
        return 0;
    }
    std::ifstream file;
    file.open("Day6.txt");

    if(!file.is_open()) {
        return -1;
    }
    std::string temp;
    while(getline(file, temp)) {
        std::cout << "Marker location: " << getMarkerLocation(temp) << std::endl;
    }
    return 0;
}
