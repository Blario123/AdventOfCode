#include <iostream>
#include <fstream>
#include <list>
#include <string>

int main(int argc, char *argv[]) {
    std::ifstream input;
    input.open("Day1.txt");
    
    int data;
    std::list<int> elvesTotalCalories;
    std::string temp;
    
    if(!input.is_open()) {
        return -1;
    }
    
    while(getline(input, temp)) {
        if(temp == "") {
            std::cout << data << std::endl;
            elvesTotalCalories.push_back(data);
            data = 0;
        } else {
            data += atoi(temp.c_str());
        }
    }
    
    elvesTotalCalories.sort(std::greater<int>());
    
    std::cout << "Calorie count:" << elvesTotalCalories.front() << std::endl;
    int count = 0;
    int topThree = 0;
    for(auto i: elvesTotalCalories) {
        if(count < 3) {
            count++;
            topThree += i;
        }
    }
    std::cout << "Top three calorie count:" << topThree << std::endl;
    return 0;
}
