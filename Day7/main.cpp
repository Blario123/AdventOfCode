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
    int fileLine = 0;
    std::vector<dir*> dirs;
    std::vector<file> files;
};

file createFile(const std::string &s) {
    file tempFile;
    unsigned long spacePos = s.find_first_of(' ');
    // Only allow lines not containing "dir", otherwise, set the size
    // to -1 to be detected as not a file
    if(s.find("dir") == std::string::npos) {
        // Extract the size and name from s by using the calculated space position
        tempFile.size = (unsigned long) std::stoi(s.substr(0, spacePos++));
        tempFile.name = s.substr(spacePos, s.size() - spacePos);
    } else {
        tempFile.size = -1;
    }
    return tempFile;
}

// The vector of all directories, to allow each
// element to reference other elements as children
std::vector<dir> directories;

void createTree(const std::vector<std::string> &list) {
    // Find the location in list of where the cd commands are.
    // This can be used for parent references later.
    std::vector<int> dirLocations, parentLocation, depthLocation;
    for(int i = 0; i < list.size(); i++) {
        // Skip over non "$ cd" commands
        if(list[i].find("$ cd") == std::string::npos) {
            continue;
        }
        // Skip over ".." commands, but remove the last entry to depthLocation
        if(list[i].find("..") != std::string::npos) {
            depthLocation.pop_back();
            continue;
        }
        // Ignore the first entry due to it not having a parent
        // Add the line number to parentLocation
        if(i != 0) {
            parentLocation.push_back(depthLocation.back());
        }
        // Add the line number of the "$ cd ___" command to dirLocations;
        dirLocations.push_back(i);
        // Add the previous working directory to depthLocation
        depthLocation.push_back(i);
    }
    // Construct the files from the previous locations.
    for(auto i: dirLocations) {
        dir newDir;
        // Extract the name of the dir from the dirLocation by
        // removing the first 5 characters "$ cd "
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
    // Allocate parent pointer from directories
    for(int i = 0; i < directories.size(); i++) {
        for(int j = 0; j < parentLocation.size(); j++) {
            if(directories[i].fileLine == parentLocation[j]) {
                directories[i].dirs.push_back(&directories[j + 1]);
            }
        }
    }
}

unsigned long calculateSize(const dir &d) {
    unsigned long tempSize = 0;
    // Calculate the size of the files in the dir
    for(const auto& i: d.files) {
        tempSize += i.size;
    }
    // Calculate the size of the subdirs recursively in the dir
    for(auto i: d.dirs) {
        tempSize += calculateSize(*i);
    }
    return tempSize;
}

int main(int argc, char *argv[]) {
    // Template for adding command line options
#if 0 // UNUSED
    if(argc > 1) {

    } else {
        std::cout << "Usage: " << argv[0] << "" << std::endl;
        return 0;
    }
#else
    // Cast argc and argv to void to remove warnings for unused variables
    (void) argc; // UNUSED
    (void) argv; // UNUSED
#endif
    // Load file and process the lines
    input.open("Day7.txt");

    if(!input.is_open()) {
        return -1;
    }
    std::vector<std::string> inputList;
    std::string temp;
    while(getline(input, temp)) {
        inputList.emplace_back(temp);
    }
    // Generate the directory tree/list to be processed
    createTree(inputList);

    // Add the size of a directory if the size is less than 100000
    unsigned long total = 0;
    for(const auto& i: directories) {
        unsigned long s = calculateSize(i);
        if(s <= 100000) {
            total += s;
        }
    }
    std::cout << std::endl << "Total = " << total << std::endl;
    return 0;
}
