#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

std::ifstream input;
struct Monkey;
std::vector<Monkey> monkeys;
bool worry = true;

typedef long long llong;

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
    std::vector<llong> items;
    Operation operation{};
    int test = 0;
    int inspected = 0;
    std::vector<int> throwTo;
    void process(int s) {
        for(long long &item : items) {
            // Increase the inspected count of the monkey
            inspected++;
            long long mN;
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
            // Process the division if the program is accounting for worry level changing after
            // Inspection of an item.
            if(worry) {
                // Divide the number by 3 to account for the worry level updating
                item /= 3;
            } else {
                // Mod the item using the s (shared modulus) of the monkeys items.
                item = item % s;
            }
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

unsigned long calculateMB() {
    // Allocate the first monkeys inspected number to t1
    unsigned long t1 = monkeys[0].inspected, t2 = 0;
    for(int i = 1; i < monkeys.size(); i++) {
        // Iterate through monkeys and if the inspected number is
        // Greater than t1, t2 becomes t1 and t1 becomes inspected
        // This is the new top 2 highest
        if(monkeys[i].inspected >= t1) {
            t2 = t1;
            t1 = monkeys[i].inspected;
        } else {
            if(monkeys[i].inspected >= t2) {
                t2 = monkeys[i].inspected;
            }
        }
    }
    // Return the product of the top two inspected counts
    return t1 * t2;
}

void run(int n) {
    // Calculate a shared modulus value for allowing larger numbers to be processed
    int sMod = 1;
    for(auto &i: monkeys) {
        sMod *= i.test;
    }
    // For n times, iterate through all the monkeys processing their moves
    for(int i = 0; i < n; i++) {
        for(Monkey &j: monkeys) {
            j.process(sMod);
        }
    }
}

int main(int argc, char *argv[]) {
    // Template for command line arguments
    int iterations;
    if(argc > 1) {
        iterations = std::atoi(argv[1]);
        if(std::string(argv[2]) == "false") {
            worry = false;
        }
    } else {
        std::cout << "Usage: " << argv[0] << " [Iterations] [Worry]->(true/false)" << std::endl;
        return 0;
    }
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
    run(iterations);
    std::cout << "Monkey business = " << calculateMB() << std::endl;
    return 0;
}
