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
    // Determine the greater data.size() of the two packets to compare
    std::size_t size = (p1.data.size() > p2.data.size()) ? p1.data.size() : p2.data.size();
    // If p1 is empty before p2, they are in the correct order
    if(p1.data.empty() && !p2.data.empty()) {
        return true;
    }
    // A pair of ints are used to house the counters for iterating through subPackets
    std::pair<int, int> counters = {0,0};
    for(int i = 0; i < size; i++) {
        if(i == p1.data.size()) {
            printf("p1 size reached %i %zu\n", i, p2.data.size() - 1);
            if(i < p2.data.size()) {
                return false;
            }
            if(i == p2.data.size()) {
                return true;
            }
        } else if(i == p2.data.size()) {
            printf("p2 size reached %i %zu\n", i, p2.data.size() - 1);
            return false;
        }
        printf("Comparing %i to %i\n", p1.data[i], p2.data[i]);
        if(p1.data[i] == -1 || p2.data[i] == -1) {
            Packet tp1, tp2;
            if(p1.data[i] == -1) {
                tp1 = p1.subPackets[counters.first++];
            } else {
                tp1 = convertToPacket(p1.data[i]);
            }
            if(p2.data[i] == -1) {
                tp2 = p2.subPackets[counters.second++];
            } else {
                tp2 = convertToPacket(p2.data[i]);
            }
            if(comparePackets(tp1, tp2)) {
                return true;
            }
        } else {
            if(p1.data[i] < p2.data[i]) {
                return true;
            } else if(p1.data[i] > p2.data[i]){
                return false;
            } else {
                continue;
            }
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
    int count = 1;
    int total = 0;
    while(getline(input, temp)) {
        processLine(temp);
       if(packets.size() == 2) {
           bool result = comparePackets(packets[0], packets[1]);
           printf("%i\n", result);
           if(result) {
               total += count;
           }
           count++;
           // After processing the packets, clear the vector
           packets.resize(0);
       }
    }
    printf("Total = %i", total);
    return 0;
}
