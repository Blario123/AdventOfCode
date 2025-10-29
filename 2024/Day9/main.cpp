#include <cstdio>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <cstdint>

class Block {
public:
    int id;
    int size;
    int space;
};

std::vector<Block> blocks;
std::vector<std::string> filesystem;

void printBlocks(std::vector<Block>* b) {
    for(int i = 0; i < b->size(); i++) {
        Block* bl = &b->at(i);
        printf("Block %d\nSize = %d, Space = %d\n", bl->id, bl->size, bl->space);
    }
}

void createFileSystem(std::vector<Block>* b) {
    filesystem.clear();
    for(int i = 0; i < b->size(); i++) {
        Block* bl = &b->at(i);
        for(int j = 0; j < bl->size; j++) {
            filesystem.emplace_back(std::to_string(bl->id));
        }
        for(int j = 0; j < bl->space; j++) {
            filesystem.emplace_back(".");
        }
    }
}

std::string printBlocksAsLine(std::vector<Block>* b) {
    std::string temp;
    for(int i = 0; i < b->size(); i++) {
        Block* bl = &b->at(i);
        for(int j = 0; j < bl->size; j++) {
            temp.append(std::to_string(bl->id));
        }
        for(int j = 0; j < bl->space; j++) {
            temp.push_back('.');
        }
    }
    return temp;
}

void printFileSystem() {
    for(const auto& i: filesystem) {
        printf("%s", i.c_str());
    }
    printf("\n");
}

void processLine(std::string l) {
    int id = 0;
    for(int i = 0; i < l.size(); i += 2) {
        Block b;
        b.id = id++;
        b.size = l[i] - '0';
        b.space = 0;
        if(l[i + 1] != '\0') {
            b.space = l[i+1] - '0';
        }
        blocks.emplace_back(b);
    }
}

void defragFileSystem() {
    // Get element from back, if number, find first instance of '.' and swap.
    for(;;) {
        // Get back element
        std::string* backElement;
        int backElementIndex;
        for(int i = filesystem.size() - 1; i >= 0; i--) {
            if(filesystem[i] != ".") {
            backElement = &filesystem[i];
            backElementIndex = i;
            break;
            }
        }
        // Find first '.'
        std::string* frontElement;
        int frontElementIndex;
        for(int i = 0; i < filesystem.size(); i++) {
            if(filesystem[i] == ".") {
                frontElement = &filesystem[i];
            frontElementIndex = i;
            break;
            }
        }
        if(frontElementIndex > backElementIndex) {
            break;
        }
        // If the indexes are valid, proceed
        *frontElement = *backElement;
        *backElement = ".";
    }
}

void defragBlocks() {
    // Get block from back, check space after each element
    // If it fits, set size to 0 of first element, set size of
    // inserted element to first element prev. size - inserted block size
    for(int i = blocks.size() - 1; i >= 0; i--) {
        const Block b = blocks[i];
        for(int j = 0; j < i; j++) {
            // No need to check the same block.
            if(i != j) {
                Block c = blocks[j];
                if(b.size <= c.space) {
                    blocks.insert(blocks.begin() + j + 1, b);
                    blocks[j + 1].space = blocks[j].space - blocks[j + 1].size;
                    blocks[j].space = 0;
                    if(j > 0) {
                        blocks[i].space += b.size + b.space;
                    }
                    blocks.erase(blocks.begin() + i + 1);
                    i++;
                    break;
                }
            }
        }
    }
}

uint64_t calculateChecksum() {
    uint64_t sum = 0;
    for(int i = 0; i < filesystem.size(); i++) {
        if(filesystem[i] == ".") {
            continue;
        }
        //printf("%d * %d = %d, sum = %llu\n
        sum += i * std::stoi(filesystem[i]);
    }
    return sum;
}

int main(int argc, char** argv) {
    if(argc > 1) {
        // Data input
        std::ifstream input;
        input.open(argv[1]);
        std::string line;
        getline(input, line, '\n');
        processLine(line);
        createFileSystem(&blocks);
        defragFileSystem();
        uint64_t sum = calculateChecksum();
        printf("Checksum = %llu\n", sum);
        defragBlocks();
        createFileSystem(&blocks);
        sum = calculateChecksum();
        printf("Contiguous checksum = %llu\n", sum);
    }
    return 0;
}

