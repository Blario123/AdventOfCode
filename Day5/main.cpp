#include <iostream>
#include <fstream>
#include <vector>

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

std::vector<Stack*> columns;
std::vector<std::string> moveList;
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

void processMove(std::string line) {

}

void processLine(std::string line) {
    // If temp contains the char 1, it can be assumed
    // it is either a column line, or a move line.
    if(line.find("move") != std::string::npos) {
        std::cout << "Found a move line " << line << std::endl;
        moveList.emplace_back(line);
    } else if(line.find('[') != std::string::npos) {
        stackList.emplace_back(line);
    } else {
        std::cout << "Found a column line " << line << std::endl;
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
    std::cout << "columns.size() = " << columns.size() << std::endl; 
    std::cout << "stackList.size() = " << stackList.size() << std::endl;
    std::cout << "moveList.size() = " << moveList.size() << std::endl;
    return 0;
}
