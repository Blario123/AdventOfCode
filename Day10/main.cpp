#include <iostream>
#include <fstream>
#include <vector>

std::ifstream input;
bool signalFrame = false;
int signalInt, X = 1;
std::vector<std::string> lines(6);

void drawSprite(int &c) {
    char toAdd;
    for(int i = X; i <= X + 2; i++) {
        int relPos = c % 40;
        if(relPos == 0) {
            relPos = 40;
        }
        if(i == relPos) {
            toAdd = '#';
            break;
        } else {
            toAdd = '.';
        }
    }
    lines[(c - 1) / 40].push_back(toAdd);
}

void checkSignalFrame(int &c) {
    if((c % 20) == 0 && (c % 40) != 0) {
        signalFrame = true;
        signalInt = X;
    }
    drawSprite(c);
}

void processCommand(const std::string &s, int &c) {
    if(s.substr(0, 4) == "noop") {
        c++;
        checkSignalFrame(c);
    } else {
        for(int i = 0; i < 2; i++) {
            c++;
            checkSignalFrame(c);
        }
        X += std::stoi(s.substr(5, s.size() - 5));
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
    input.open("Day10.txt");
    if(!input.is_open()) {
        return -1;
    }
    int cycleCount = 0;
    int signalCount = 1;
    int total = 0;
    std::string temp;
    while(getline(input, temp)) {
        processCommand(temp, cycleCount);
        if(signalFrame) {
            signalFrame = false;
            total += (signalCount * 20) * signalInt;
            signalCount += 2;
        }
    }
    std::cout << "Total = " << total << std::endl;
    for(const auto &i: lines) {
        std::cout << i << std::endl;
    }
    return 0;
}
