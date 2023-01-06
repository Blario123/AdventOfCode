#include <iostream>
#include <fstream>
#include <vector>

std::ifstream input;

struct file;
struct dir;

struct file {
    unsigned long size = 0;
    std::string name;
};

struct dir {
    std::string name;
    dir *parent = nullptr;
    unsigned long size = 0;
    unsigned long hash;
    int fileLine = 0;
    std::vector<file> files;
    std::vector<dir> subDirs;
};

file createFile(const std::string &s) {
    file tempFile;
    unsigned long spacePos = s.find_first_of(' ');
    if(s.find("dir") == std::string::npos) {
        tempFile.size = (unsigned long) std::stoi(s.substr(0, spacePos++));
        tempFile.name = s.substr(spacePos, s.size() - spacePos);
    } else {
        tempFile.size = -1;
    }
    return tempFile;
}

std::vector<dir> directories;

void createTree(const std::vector<std::string> &list) {
    // Find the location in list of where the cd commands are.
    // This can be used for parent references later.
    std::vector<int> dirLocations;
    for(int i = 0; i < list.size(); i++) {
        if(list[i].find("$ cd") != std::string::npos && list[i].find("..") == std::string::npos) {
            dirLocations.emplace_back(i);
        }
    }
    // Construct the files from the previous locations.
    for(auto i: dirLocations) {
        dir newDir;
        std::string name = list[i].substr(5, list[i].size() - 5);
        newDir.name = name;
        newDir.fileLine = i;
        // Read files until it finds; a "$ cd" or the end of scope, and if j is beyond the scope of list.
        for(int j = i + 2; list[j].find("$ cd") == std::string::npos && j < list.size(); j++) {
            file toAdd = createFile(list[j]);
            if(toAdd.size != -1) {
                newDir.files.emplace_back(toAdd);
            }
        }
        directories.emplace_back(newDir);
    }
    // Calculate the hash of the directory,
    // The hash also needs the size of the directory.
    for(int i = 0; i < directories.size(); i++) {
        // Size calculation
        unsigned long sizeTotal = 0;
        for(const auto& j: directories[i].files) {
            sizeTotal += j.size;
        }
        directories[i].size = sizeTotal;
        // Hash calculation
        std::size_t nameHash = std::hash<std::string>{}(directories[i].name);
        std::size_t sizeHash = std::hash<unsigned long>{}(directories[i].size);
        directories[i].hash = std::hash<std::size_t>{}(nameHash & (sizeHash) ^ (i << 1));
    }
}

int main(int argc, char *argv[]) {
#if 0
    if(argc > 1) {
        markerLength = std::atoi(argv[1]);
    } else {
        std::cout << "Usage: " << argv[0] << " [Length of marker]" << std::endl;
        return 0;
    }
#endif
    input.open("Day7.txt");

    if(!input.is_open()) {
        return -1;
    }
    std::vector<std::string> inputList;
    std::string temp;
    while(getline(input, temp)) {
        inputList.emplace_back(temp);
    }

    createTree(inputList);

    unsigned long total = 0;

    std::cout << std::endl << "Total = " << total << std::endl;
    return 0;
}

// Is not higher than 1869999
// Is not lower than 1069572
