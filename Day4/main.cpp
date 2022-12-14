#include <iostream>
#include <fstream>

struct section{
    int min;
    int max;
};

bool isContained(section elf1, section elf2, bool r = true) {
    // Run first pass with elf1 and elf2, return true if it is contained within.
    if(elf1.min >= elf2.min && elf1.max <= elf2.max) {
        return true;
    }
    // If elf2 does not contain elf1, swap them around, but avoid recursion for successive fails.
    if(r) {
        return isContained(elf2, elf1, false);
    }
    // No match for elf1:elf2 or elf2:elf1, return false.
    return false;
}

bool isOverlapped(section elf1, section elf2, bool r = true) {
    if(elf1.min <= elf2.max && elf1.min >= elf2.min) {
        return true;
    }
    if(elf1.max >= elf2.min && elf1.max <= elf2.max) {
        return true;
    }
    if(r) {
        return (isOverlapped(elf2, elf1, false));
    }
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
    int containCount;
    int overlapCount;
    while(getline(file, temp)) {
        std::string elf1Section = temp.substr(0, temp.find(','));
        std::string elf2Section = temp.substr(temp.find(',') + 1, temp.size());
        section elf1 = createSection(elf1Section);
        section elf2 = createSection(elf2Section);
        if(isContained(elf1, elf2)) {
            containCount++;
        }
        if(isOverlapped(elf1, elf2)) {
            overlapCount++;
        }
    }
    
    std::cout << "Contained pair count: " << containCount << std::endl;
    std::cout << "Overlapping pair count: " << overlapCount << std::endl;
    
    return 0;
}
