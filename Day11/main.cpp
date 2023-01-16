#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

std::ifstream input;

enum Modifier {
    Add,
    Minus,
    Multiply,
    Divide
};

struct Operation {
    Modifier modifier;
    int n;
};

struct Monkey {
    std::vector<int> items;
    Operation operation{};
    int test = 0;
    std::vector<int> throwTo;
};

std::vector<Monkey> monkeys;

Monkey newMonkey() {
    Monkey tempMonkey;
    std::string temp;
    while(getline(input, temp)) {
        if(temp.empty()) {
            break;
        }
        std::cout << temp << std::endl;
        // Starting list
        if(temp.find("Starting") != std::string::npos) {
            std::size_t colonPos = temp.find_first_of(':');
            std::string startingList = temp.substr(colonPos + 1, temp.size() - colonPos + 1);
            std::size_t start;
            std::size_t end = 0;
            while((start = startingList.find_first_not_of(',', end)) != std::string::npos) {
                end = startingList.find(", ", start);
                int stressNo = std::stoi(startingList.substr(start, end - start));
                std::cout << stressNo << std::endl;
                tempMonkey.items.push_back(std::stoi(startingList.substr(start, end - start)));
            }
        }
        // Operation
        if(temp.find("old") != std::string::npos) {
            Operation tempOp{};
            std::size_t pos = temp.find("old") + 1;
            std::string modString = temp.substr(pos, 1);
            if(modString == "+") {
                tempOp.modifier = Add;
            } else if(modString == "-") {
                tempOp.modifier = Minus;
            } else if(modString == "*") {
                tempOp.modifier = Multiply;
            } else {
                tempOp.modifier = Divide;
            }
            pos += 4;
            tempOp.n = std::stoi(temp.substr(pos, temp.size() - pos));
            tempMonkey.operation = tempOp;
        }
        // Test
        if(temp.find("by") != std::string::npos) {
            std::size_t pos = temp.find("by") + 3;
            tempMonkey.test = std::stoi(temp.substr(pos, temp.size() - pos));
        }
        // True/False
    }
    return tempMonkey;
}

void processLine(const std::string &s) {
    if(s.find("Monkey") != std::string::npos) {
        monkeys.push_back(newMonkey());
    }
}

int main(int argc, char *argv[]) {
    // Template for command line arguments
#if 0
    if(argc > 1) {
         = atoi(argv[1]);
    } else {
        std::cout << "Usage: " << argv[0] << "" << std::endl;
        return 0;
    }
#else
    (void) argc;
    (void) argv;
#endif
    // Load file and process the lines
    input.open("Day11.txt");
    if(!input.is_open()) {
        return -1;
    }
    std::string temp;
    while(getline(input, temp)) {
        processLine(temp);
    }
    return 0;
}
