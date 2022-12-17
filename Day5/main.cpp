#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

class Stack {
    public:
        explicit Stack() = default;
        void addItem(std::vector<char> &item, bool isMove = false) {
            if(isMove) {
                std::reverse(item.begin(), item.end());
            }
            stack.insert(stack.begin(), item.begin(), item.end());
        }
        std::vector<char> removeItem(int n) {
            std::vector<char> tempVec;
            for(int i = 0; i < n; i++) {
                tempVec.emplace_back(stack[0]);
                stack.erase(stack.begin());
            }
            return tempVec;
        }
        friend std::ostream& operator<<(std::ostream &stream, const Stack &s) {
            for(auto i: s.stack) {
                stream << i;
            }
            return stream;
        }
        char finalItem() {
            return stack[0];
        }
    private:
        std::vector<char> stack;
};

struct move {
    int n;
    int from;
    int to;
    friend std::ostream& operator<<(std::ostream &stream, const move &m) {
        stream << "Move " << m.n << " From " << m.from << " To " << m.to;
        return stream;
    }
};

std::vector<Stack*> columns;
std::vector<move> moveList;
std::vector<std::string> stackList;
bool is9000 = true;

void processColumn(const std::string &line, std::vector<Stack*> &vector) {
    // Process the line and create a Stack element
    // for every digit.
    for(auto i: line) {
        if(i != ' ') {
            auto *stack = new Stack;
            vector.emplace_back(stack);
        }
    }
}

std::string removeSubstr(std::string s, const std::string &r) {
    std::size_t rLoc = s.find(r);
    if(rLoc != std::string::npos) {
        s.erase(rLoc, r.length());
    }
    return s;
}

int findDigit(std::string &line, const char &c) {
    int digit;
    std::size_t loc = line.find_first_of(c);
    if(loc != std::string::npos) {
        digit = std::stoi(line.substr(0, loc));
        line.erase(0, loc + 1);
    }
    return digit;
}

void processMove(std::string line, std::vector<move> &vector) {
    move tempMove{};
    // Remove all the unnecessary text from line
    line = removeSubstr(line, "move ");
    line = removeSubstr(line, "from ");
    line = removeSubstr(line, "to ");
    // Find first number in line.
    tempMove.n =  findDigit(line, ' ');
    tempMove.from = findDigit(line, ' ');
    line.append(" ");
    tempMove.to = findDigit(line, ' ');
    moveList.emplace_back(tempMove);
}

void processLine(const std::string &line) {
    // Move commands all contain the word move in them.
    // All lines that contain move can be processed.
    if(line.find("move") != std::string::npos) {
        processMove(line, moveList);
    // All item lines contain a [ or ] can be pushed
    // onto stackList for later processing.
    } else if(line.find('[') != std::string::npos) {
        stackList.emplace_back(line);
    // Everything else, column line, can be processed individually.
    } else {
        processColumn(line, columns);
    }
}

void run() {
    for(auto i: moveList) {
        std::vector<char> c = columns[i.from - 1]->removeItem(i.n);
        if(is9000) {
            columns[i.to - 1]->addItem(c, true);
        } else {
            columns[i.to - 1]->addItem(c);
        }
    }
}

void listCranes() {
    std::cout << "Available cranes:" << std::endl;
    std::cout << "\t - 9000 (CrateMover 9000)" << std::endl;
    std::cout << "\t - 9001 (CrateMover 9001)" << std::endl; 
}

int main(int argc, char *argv[]) {
    if(argc == 1) {
        std::cout << "No crane version was given. Cannot run without a crane input." << std::endl;
        listCranes();
        return 0;
    } else if(argc >= 2) {
        if(std::atoi(argv[1]) != 9000 && std::atoi(argv[1]) != 9001) {
            std::cout << "This crane option is invalid. Please try again using one of the valid cranes." << std::endl;
            listCranes();
            return -1;
        }
        if(std::atoi(argv[1]) == 9001) {
            is9000 = false;
        }
    }
    std::ifstream file;
    file.open("Day5.txt");

    if(!file.is_open()) {
        return -1;
    }
    std::string temp;
    bool isColumnRow = false;
    while(getline(file, temp)) {
        if(temp.empty()) {
            // If string is empty, skip.
            continue;
        }
        processLine(temp);
    }
    // After separating the lines into their respective vectors
    // they can be processed. First is filling in the Stack from the stackList.
    for(int i = 0; i < columns.size(); i++) {
        std::vector<char> stackListTemp;
        for(int j = 0; j < stackList.size(); j++) {
            char l = stackList[j][1 + (i*4)];
            if(l != ' ') {
                stackListTemp.emplace_back(l);    
            }
        }
        columns[i]->addItem(stackListTemp);
        stackListTemp.resize(0);
    }

    run();

    std::cout << "Final output: ";
    for(auto i: columns) {
        std::cout << (char) i->finalItem();
    }
    std::cout << std::endl;
    
    return 0;
}
