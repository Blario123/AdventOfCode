#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

std::ifstream input;
struct Monkey;
std::vector<Monkey> monkeys;

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
    int inspected = 0;
    std::vector<int> throwTo;
    void process() {
        for(int & item : items) {
            // Increase the inspected count of the monkey
            inspected++;
            int mN;
            if(operation.n == 0) {
                mN = item;
            } else {
                mN = operation.n;
            }
            // Using the operation of the monkey, modify the items number accordingly
            switch(operation.modifier) {
                case Add:
                    item += mN;
                    break;
                case Minus:
                    item -= mN;
                    break;
                case Multiply:
                    item *= mN;
                    break;
                case Divide:
                    item /= mN;
                    break;
            }
            // Divide the number by 3 to account for the worry level updating
            item /= 3;
            // If the item is divisible by item, it will return 0 and
            // The item will be pushed to the respective monkey
            if((item % test) == 0) {
                monkeys[throwTo[0]].items.push_back(item);
            } else {
                monkeys[throwTo[1]].items.push_back(item);
            }
        }
        // After the monkey has finished processing
        // Instead of removing while processing, the monkeys
        // List will be cleared
        items.clear();
    }
};

Monkey newMonkey() {
    Monkey tempMonkey;
    std::string temp;
    while(getline(input, temp)) {
        // Skip reading an empty line, returns the monkey to be added to the list
        if(temp.empty()) {
            break;
        }
        // Starting List
        if(temp.find("Starting") != std::string::npos) {
            // Create a list of all the starting numbers the monkey has
            std::size_t colonPos = temp.find_first_of(':');
            std::string startingList = temp.substr(colonPos + 1, temp.size() - colonPos + 1);
            std::size_t s;
            std::size_t e = 0;
            // Find the position of ',' and split the string into integers for the list
            while((s = startingList.find_first_not_of(',', e)) != std::string::npos) {
                e = startingList.find(", ", s);
                int stressNo = std::stoi(startingList.substr(s, e - s));
                tempMonkey.items.push_back(stressNo);
            }
        }
        // Operation
        if(temp.find("old") != std::string::npos) {
            std::size_t pos = temp.find("old") + 4;
            std::string modString = temp.substr(pos, 1);
            // Assign the enum value depending on the modifier
            if(modString == "+") {
                tempMonkey.operation.modifier = Add;
            } else if(modString == "-") {
                tempMonkey.operation.modifier = Minus;
            } else if(modString == "*") {
                tempMonkey.operation.modifier = Multiply;
            } else {
                tempMonkey.operation.modifier = Divide;
            }
            // Increase the position by 2 to account for the new location to search
            pos += 2;
            std::string nStr = temp.substr(pos, temp.size() - pos);
            // If the string is "old" n will be allocated as 0 to be detected later
            // If the string is a number, it will be converted to an integer and assigned to n
            if(nStr == "old") {
                tempMonkey.operation.n = 0;
            } else {
                tempMonkey.operation.n = std::stoi(nStr);
            }
        }
        // Test
        if(temp.find("by") != std::string::npos) {
            // Find the test number of the monkey
            std::size_t pos = temp.find("by") + 3;
            tempMonkey.test = std::stoi(temp.substr(pos, temp.size() - pos));
        }
        // True/False
        if(temp.find("If") != std::string::npos) {
            // Push the monkey number of each line, True then False, to the throwTo
            // Vector to be used when processing the monkeys move
            std::size_t monkeyPos = temp.find("monkey") + 7;
            int monkeyNo = std::stoi(temp.substr(monkeyPos, temp.size() - monkeyPos));
            tempMonkey.throwTo.push_back(monkeyNo);
        }
    }
    return tempMonkey;
}

void processLine(const std::string &s) {
    if(s.find("Monkey") != std::string::npos) {
        // Push a new monkey on the vector monkeys
        monkeys.push_back(newMonkey());
    }
}

void run(int n) {
    // For n times, iterate through all the monkeys processing their moves
    for(int i = 0; i < n; i++) {
        for(Monkey &j: monkeys) {
            j.process();
        }
    }
}

long calculateMB() {
    // Allocate the first monkeys inspected number to t1
    long t1 = monkeys[0].inspected, t2 = 0;
    for(const auto &i: monkeys) {
        // Iterate through monkeys and if the inspected number is
        // Greater than t1, t2 becomes t1 and t1 becomes inspected
        // This is the new top 2 highest
        if(i.inspected > t1) {
            t2 = t1;
            t1 = i.inspected;
        }
    }
    // Return the product of the top two inspected counts
    return t1 * t2;
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
    // Run the monkey processing 20 times
    run(20);
    std::cout << "Monkey business = " << calculateMB() << std::endl;
    return 0;
}
