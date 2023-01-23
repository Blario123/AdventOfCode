#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

std::ifstream input;
struct Packet;

struct Packet {
    std::vector<int> data{};
    std::vector<Packet> subPackets{};
};

std::vector<Packet> packets;

Packet createPacket(std::string &s) {
    Packet tPacket;
    // Remove leading and trailing '[' and ']'
    s = s.substr(1, s.size() - 2);
    // Reduce all lists into -1
    if(std::count(s.begin(), s.end(), '[') > 0) {
        
    }
    std::size_t start;
    std::size_t end = 0;
    while((start = s.find_first_not_of(',', end)) != std::string::npos) {
        end = s.find(',', start);
        tPacket.data.emplace_back(std::stoi(s.substr(start, end - start)));
    }
    printf("%s\n", s.c_str());
    printf("Data size = %zu\n", tPacket.data.size());
    return tPacket;
}

bool comparePackets(const Packet &p1, const Packet &p2) {

}

void processLine(std::string &s) {
    if(s.empty()) {
        return;
    }
    packets.emplace_back(createPacket(s));
}

int main(int argc, char *argv[]) {
    // Template for command line arguments
#if 0
    if(argc > 1) {
    } else {
        std::cout << "Usage: " << argv[0] << " " << std::endl;
        return 0;
    }
#else
    (void) argc;
    (void) argv;
#endif
    // Load file and process the lines
    input.open("Day13.txt");
    if(!input.is_open()) {
        return -1;
    }
    // Allocate the start and end positions for the searching.
    std::string temp;
    while(getline(input, temp)) {
        processLine(temp);
//        if(packets.size() == 2) {
//            comparePackets();
            // After processing the packets, clear the vector
//            packets.resize(0);
//        }
    }
    return 0;
}
