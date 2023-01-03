#include <iostream>
#include <fstream>
#include <vector>
#include <list>

std::ifstream input;

struct file;
struct dir;

std::vector<dir> directories;

dir *tempDir = nullptr;
dir *prevDir = nullptr;

void processLine(const std::string&);
file processItem(const std::string&, dir &);
long calculateTotal(dir &);
int main(int, char**);

struct file {
    long size = 0;
    std::string name;
};

struct dir {
    dir *parent = nullptr;
    std::vector<file> files;
    std::vector<dir*> subDirs;
    unsigned long hash;
    std::string name;
    long size;
    void processSize() {
        long s = 0;
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
        auto sizeHash = std::hash<long>{}(size);
        unsigned long finalHash;
        if(parent != nullptr) {
            auto parentHash = std::hash<std::string>{}(parent->name);
            finalHash = std::hash<std::size_t>{}(nameHash ^ (parentHash << 1) ^ (sizeHash << 1) ^ i);
        } else {
            finalHash = std::hash<std::size_t>{}(nameHash & (sizeHash << 1) ^ i);
        }
        hash = finalHash;
    }
    void processSubdirs() {
//        std::cout << name << ".processSubdirs()" << std::endl;
        for(auto &i: directories) {
            if(i.name == "/") {
                continue;
            }
            if(i.parent->hash == hash) {
//                std::cout << i.name << ".parent = " << (std::string) i.parent->name << std::endl;
                subDirs.push_back(&i);
            }
        }
    }
};

file processItem(const std::string &s, dir &d) {
//    std::cout << s << std::endl;
    file tempFile;
    int spacePos;
    spacePos = s.find_first_of(" ");
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
    if(s.find("$ ls") != std::string::npos) {
        long index;
//        std::cout << s << std::endl;
        std::string temp;
        for(dir const &i: directories) {
            if(i.name == tempDir->name) {
                index = std::addressof(i) - std::addressof(directories[0]);
                break;
            }
        }
        while(getline(input, temp)) {
            if(temp.find("$ cd") != std::string::npos) {
                processLine(temp);
                break;
            } else {
                file toAdd = processItem(temp, directories[index]);
                if(toAdd.size != -1) {
                    directories[index].files.emplace_back(toAdd);
                }
            }
        }
    } else if(s.find("$ cd") != std::string::npos) {
//        std::cout << s << std::endl;
        std::string n = s.substr(5, s.size() - 5);
        if(n == "..") {
            tempDir = (dir*) tempDir->parent;
        } else {
            if(tempDir != nullptr) {
                prevDir = tempDir;
            }
            tempDir = new dir;
            tempDir->name = n;
            if(n != "/") {
                tempDir->parent = prevDir;
            }
            directories.push_back(*tempDir);
        }
    }
}

long calculateTotal(dir &d) {
//    std::cout << d.name << " calculateTotal" << std::endl;
    long total = 0;
    if(d.size <= 100000) {
        total += d.size;
    } else {
        return -1;
    }
    for(auto i: d.subDirs) {
//        std::cout << i->subDirs.size() << std::endl;
        long t = calculateTotal(*i);
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
        i.processSize();
        long index;
        index = std::addressof(i) - std::addressof(directories[0]);
        i.processHash(index);
    }
    for(auto &i: directories) {
        i.processSubdirs();
    }

    long total = 0;
    for(auto &i: directories) {
        long tempL = calculateTotal(i);
        if(tempL > 0) {
            total += tempL;
        }
    }

    std::cout << std::endl << "Total = " << total << std::endl;
    return 0;
}

// Is not higher than 1869999
// Is not lower than 1069572
