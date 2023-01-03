#include <iostream>
#include <fstream>
#include <vector>
#include <list>

std::ifstream input;

struct file;
struct dir;

std::vector<dir> directories;

dir *tempDir;
dir *prevDir;
std::size_t index = 0;
int count = 0;

void processLine(const std::string&);
file processItem(const std::string&);
unsigned long calculateTotal(dir &, int);
int main(int, char**);

struct file {
    unsigned long size = 0;
    std::string name;
};

struct dir {
    std::string name;
    dir *parent = nullptr;
    unsigned long size = 0;
    unsigned long hash = 0;
    std::vector<file> files;
    std::vector<dir> subDirs;
    void processSize() {
        unsigned long s = 0;
        for(const auto& i: files) {
            s += i.size;
        }
        size = s;
    }
    void processHash(long i) {
        // The hash is calculated from the name, parent name if available,
        // the size, and the index of the directory within directories.
        // The index is used to remove duplicates as some dirs,
        // have the same name, size and parent.
        auto nameHash = std::hash<std::string>{}(name);
        auto sizeHash = std::hash<unsigned long>{}(size);
        if(parent != nullptr) {
            auto parentHash = std::hash<std::string>{}(parent->name);
            hash = std::hash<std::size_t>{}(nameHash ^ (parentHash << 1) ^ (sizeHash << 1) ^ i);
        } else {
            hash = std::hash<std::size_t>{}(nameHash & (sizeHash << 1) ^ i);
        }
    }
    void processSubdirs() {
        for(auto i: directories) {
            if(i.name != "/") {
                if(i.parent->hash == hash) {
                    std::cout << i.name << ".parent = " << (std::string) i.parent->name << std::endl;
                    subDirs.push_back(i);
                }
            }
        }
    }
};

file processItem(const std::string &s) {
//    std::cout << s << std::endl;
    file tempFile;
    unsigned long spacePos;
    spacePos = s.find_first_of(' ');
    if(s.find("dir") == std::string::npos) {
        tempFile.size = (long) std::stoi(s.substr(0, spacePos++));
        tempFile.name = s.substr(spacePos, s.size() - spacePos);
    } else {
        tempFile.size = -1;
    }
    return tempFile;
}

void processLine(const std::string &s) {
    // Two possible commands; ls and cd.
    // Process them accordingly.
    std::string n;
    if(s.find("$ ls") != std::string::npos) {
//        std::cout << s << std::endl;
        std::string temp;
        while(getline(input, temp)) {
            if(temp.find("$ cd") != std::string::npos) {
                processLine(temp);
                break;
            } else {
                file toAdd = processItem(temp);
                if(toAdd.size != -1) {
                    tempDir->files.emplace_back(toAdd);
                }
            }
        }
    } else if(s.find("$ cd") != std::string::npos) {
        if(tempDir != nullptr) {
            // Finished processing files to be added
            tempDir->processSize();
            tempDir->processHash(count++);
            directories.at(index) = *tempDir;
        }
//        std::cout << s << std::endl;
        n = s.substr(5, s.size() - 5);
        if(n == "..") {
            tempDir = tempDir->parent;
        } else {
            if(tempDir != nullptr) {
                prevDir = tempDir;
            }
            tempDir = new dir;
            tempDir->name = n;
            if(n != "/") {
                tempDir->parent = prevDir;
            }
            directories.emplace_back(*tempDir);
            index = directories.size() - 1;
        }
    }
}

unsigned long calculateTotal(dir &d, int depth) {
//    std::cout << d.name << " calculateTotal, depth = " << depth << std::endl;
    unsigned long total = 0;
    if(d.size <= 100000) {
        total += d.size;
    } else {
        return -1;
    }
//    std::cout << d.name << " subDirs.size = " << d.subDirs.size() << std::endl;
    for(auto &i: d.subDirs) {
//        std::cout << i->subDirs.size() << std::endl;
        unsigned long t = calculateTotal(i, ++depth);
        if(t <= 100000) {
            total += t;
        }
    }
    return total;
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
    std::string temp;
    while(getline(input, temp)) {
        processLine(temp);                
    }
    for(auto &i: directories) {
        i.processSubdirs();
    }
    unsigned long total = 0;
    for(auto i: directories) {
        total += calculateTotal(i, 0);
    }
    std::cout << std::endl << "Total = " << total << std::endl;
    return 0;
}

// Is not higher than 1869999
// Is not lower than 1069572
