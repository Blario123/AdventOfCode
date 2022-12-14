#include <iostream>
#include <fstream>

struct section{
    int min;
    int max;
};

bool isContained(section elf1, section elf2, bool r = true) {
//    std::cout << "Elf 1: Min: " << elf1.min << " Max: " << elf1.max << " Elf 2: Min: " << elf2.min << " Max: " << elf2.max << std::endl;
    // Run first pass with elf1 and elf2, return true if it is contained within.
    if(elf1.min >= elf2.min && elf1.max <= elf2.max) {
//        std::cout << "Contained" << std::endl;
        return true;
    }
    // If elf2 does not contain elf1, swap them around, but avoid recursion for successive fails.
    if(r) {
//        std::cout << "Attempt with elf2 and elf1 swapped" << std::endl;
        return isContained(elf2, elf1, false);
    }
    // No match for elf1:elf2 or elf2:elf1, return false.
    return false;
}

section createSection(const std::string &s) {
    section r;
    r.min = stoi(s.substr(0, s.find('-')));
    r.max = stoi(s.substr(s.find('-') + 1, s.size()));
    return r;
}

int main(int argc, char *argv[]) {
    std::ifstream file;
    file.open("Day4.txt");

    if(!file.is_open()) {
        return -1;
    }
    std::string temp;
    int count;
    while(getline(file, temp)) {
        std::string elf1Section = temp.substr(0, temp.find(','));
        std::string elf2Section = temp.substr(temp.find(',') + 1, temp.size());
        if(isContained(createSection(elf1Section), createSection(elf2Section))) {
            count++;
        }
    }
    
    std::cout << "Contained pair count: " << count << std::endl;

    return 0;
}
