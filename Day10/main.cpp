#include <iostream>
#include <fstream>

std::ifstream input;
bool signalFrame = false;
int signalInt = 0;
int X = 1;

void checkSignalFrame(int &c, int &x) {
    if((c % 20) == 0 && (c % 40) != 0) {
        signalFrame = true;
        signalInt = x;
    }
}


void processCommand(const std::string &s, int &c) {
    if(s.substr(0, 4) == "noop") {
        c++;
        checkSignalFrame(c, X);
    } else {
        for(int i = 0; i < 2; i++) {
            c++;
            checkSignalFrame(c, X);
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
    return 0;
}
