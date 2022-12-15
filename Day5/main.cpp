#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

class Stack {
    public:
        explicit Stack() {
        }
        void addItem(std::vector<char> item) {
            stack.insert(stack.begin(), item.begin(), item.end());
        }
    private:
        std::vector<char> stack;
};

struct move {
    int n;
    int from;
    int to;
};

std::vector<Stack*> columns;
std::vector<move> moveList;
std::vector<std::string> stackList;

void processColumn(std::string line, std::vector<Stack*> &vector) {
    // Process the line and create a Stack element
    // for every digit.
    for(auto i: line) {
        if(i != ' ') {
            Stack *stack = new Stack;
            vector.emplace_back(stack);
        }
    }
}

std::string removeSubstr(std::string s, std::string r) {
    std::size_t rLoc = s.find(r);
    if(rLoc != std::string::npos) {
        s.erase(rLoc, r.length());
    }
    return s;
}

void processMove(std::string line, std::vector<move> &vector) {
    move tempMove;
    std::cout << "processMove() : " << line << std::endl;
    // Remove all the unnecessary text from line
    line = removeSubstr(line, "move ");
    line = removeSubstr(line, "from ");
    line = removeSubstr(line, "to ");
    std::cout << line << std::endl;
    // Find first number in line.
 
}

void processLine(std::string line) {
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

int main(int argc, char *argv[]) {
    std::ifstream file;
    file.open("Day5.txt");

    if(!file.is_open()) {
        return -1;
    }
    std::string temp;
    bool isColumnRow = false;
    while(getline(file, temp)) {
        if(temp == "") {
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
            stackListTemp.emplace_back(stackList[j][1 + (i*4)]);
        }
        for(auto j: stackListTemp) {
            std::cout << j;
        }
        std::cout << std::endl;
        stackListTemp.resize(0);
    }
    std::cout << "columns.size() = " << columns.size() << std::endl; 
    std::cout << "stackList.size() = " << stackList.size() << std::endl;
    std::cout << "moveList.size() = " << moveList.size() << std::endl;
    return 0;
}
