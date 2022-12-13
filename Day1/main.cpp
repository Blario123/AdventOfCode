#include <iostream>
#include <fstream>
#include <vector>
#include <string>

int main(int argc, char *argv[]) {
    std::ifstream input;
    input.open("Day1.txt");
    
    int data;
    std::vector<int> elvesTotalCalories;
    std::string temp;
    
    if(!input.is_open()) {
        return -1;
    }
    
    while(getline(input, temp)) {
        if(temp == "") {
            elvesTotalCalories.emplace_back(data);
            data = 0;
        } else {
            data += atoi(temp.c_str());
        }
    }
    
    data = 0;
    
    for(auto i: elvesTotalCalories) {
        if(i > data) {
            data = i;
        }
    }
    
    // std::cout << "Elf number:" << index << std::endl;
    std::cout << "Calorie count:" << data << std::endl;
    
    return 0;
}
