#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <type_traits>

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
    std::size_t start;
    std::size_t end = 0;
    std::string newS = s;
    if(std::count(s.begin(), s.end(), '[') > 0) {
        int toCloseCount = 0;
        for(int i = 0; i < s.size(); i++) {
            if(s[i] == '[') {
                if(toCloseCount == 0) {
                    start = i;
                }
                toCloseCount++;
            }
            if(s[i] == ']') {
                std::size_t diff = 0;
                end = i + 1;
                if(--toCloseCount == 0) {
                    std::string subString = s.substr(start, end - start);
                    tPacket.subPackets.emplace_back(createPacket(subString));
                    diff = s.size() - newS.size();
                    newS.replace(start - diff, end - start, "-1");
                }
            }
        }
    }
    s = newS;
    end = 0;
    while((start = s.find_first_not_of(',', end)) != std::string::npos) {
        end = s.find(',', start);
        tPacket.data.emplace_back(std::stoi(s.substr(start, end - start)));
    }
    return tPacket;
}

Packet convertToPacket(int i) {
    Packet p;
    p.data.emplace_back(i);
    return p;
}

bool comparePackets(const Packet &p1, const Packet &p2) {
    std::size_t size = (p1.data.size() > p2.data.size()) ? p1.data.size() : p2.data.size();
    // If p1 is empty before p2, they are in the correct order
    if(p1.data.empty() && !p2.data.empty()) {
        return true;
    }
    int p1subPacketCounter = 0;
    int p2subPacketCounter = 0;
    for(int i = 0; i < size; i++) {
        if(i == p1.data.size()) {
            break;
        } else if(i == p2.data.size()) {
            break;
        }
        if(p1.data[i] == -1 && p2.data[i] == -1) {
            if(!comparePackets(p1.subPackets[p1subPacketCounter++], p2.subPackets[p2subPacketCounter])) {
                return false;
            }
            continue;
        } else if(p1.data[i] == -1 && p2.data[i] != -1) {
            comparePackets(p1.subPackets[p1subPacketCounter++], convertToPacket(p2.data[i]));
            continue;
        }
        // If the left hand int is less than the right hand int, they are in the correct order.
        if(p1.data[i] < p2.data[i]) {
            return true;
        }
    }
    return false;
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
       if(packets.size() == 2) {
           printf("%i\n", comparePackets(packets[0], packets[1]));
           // After processing the packets, clear the vector
           packets.resize(0);
       }
    }
    return 0;
}
