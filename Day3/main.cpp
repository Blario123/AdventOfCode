#include <iostream>
#include <fstream>

int findDuplicate(const std::string &backpack) {
    std::string compartment1 = backpack.substr(0, backpack.size() / 2);
    std::string compartment2 = backpack.substr((backpack.size() / 2), backpack.size());
    char duplicate;
    for(auto i: compartment1) {
        if(compartment2.find(i) != std::string::npos) {
            duplicate = i;
            break;
        }
    }

    int duplicateCorrected;

    if(duplicate > 'Z') {
        duplicateCorrected = (int) duplicate - 96;
    } else {
        duplicateCorrected = (int) duplicate - 38;
    }
    std::cout << duplicateCorrected << " " << duplicate << std::endl;
    return duplicateCorrected;
}

int main(int argc, char *argv[]) {
    std::ifstream file;
    file.open("Day3.txt");

    if(!file.is_open()) {
        return -1;
    }
    
    std::string temp;
    int totalCount;
    while(getline(file, temp)) {
        totalCount += findDuplicate(temp);
    }
    std::cout << "Total count: " << totalCount << std::endl;
    return 0;
}
