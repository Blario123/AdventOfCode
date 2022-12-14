#include <iostream>
#include <fstream>
#include <vector>

int correctChar(char i) {
    if(i > 'Z') {
        return (int) i - 96;
    } else {
        return (int) i - 38;
    }
}

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

    return correctChar(duplicate);
}

int findDuplicateGroup(const std::vector<std::string> &group) {
    std::vector<char> duplicates;
    // Generate duplicates between group[0] and group[1]
    for(auto i: group[0]) {
        if(group[1].find(i) != std::string::npos) {
            duplicates.emplace_back(i);
        }
    }
    // Verify duplicates on group[2]
    char duplicate;
    for(auto i: duplicates) {
        if(group[2].find(i) != std::string::npos) {
            duplicate = i;
            break;
        }
    }
    return correctChar(duplicate);
}

int main(int argc, char *argv[]) {
    std::ifstream file;
    file.open("Day3.txt");

    if(!file.is_open()) {
        return -1;
    }
    std::vector<std::string> stringList;
    std::string temp;
    int totalCount;
    while(getline(file, temp)) {
        stringList.emplace_back(temp);
        totalCount += findDuplicate(temp);
    }
    std::cout << "Total count: " << totalCount << std::endl;
    
    // Part 2
    int totalCount2;
    int groupCount = 0;
    std::vector<std::string> tempVector;
    for(int i = 0; i < stringList.size(); i++) {
        if(groupCount == 3) {
            groupCount = 0;
            totalCount2 += findDuplicateGroup(tempVector);
            tempVector.resize(0);
        }
        tempVector.emplace_back(stringList[i]);
        groupCount++;        
    }
    totalCount2 += findDuplicateGroup(tempVector);
    std::cout << "Total group count: " << totalCount2 << std::endl;

    return 0;
}
