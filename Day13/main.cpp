#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <type_traits>

std::ifstream input;

struct Packet {
    std::vector<int> data{};
    std::vector<Packet> subPackets{};
};

enum State {
    True,
    False,
    Unknown
};

std::vector<Packet> packets;

Packet createPacket(std::string &s) {
    Packet tPacket;
    // Remove leading and trailing '[' and ']'
    s = s.substr(1, s.size() - 2);
    std::size_t start;
    std::size_t end;
    std::string newS = s;
    // Reduce child lists to a -1 to denote the position when processed
    // If no '[' is found in the string, process all the integers
    if(std::count(s.begin(), s.end(), '[') > 0) {
        // Keep track of the depth of nested lists
        int toCloseCount = 0;
        std::size_t diff;
        // Iterate through the string, replacing a duplicate temporary string list with "-1"
        for(int i = 0; i < s.size(); i++) {
            // If a new '[' is added, set the start for the first instance, and increment the depth
            if(s[i] == '[') {
                if(toCloseCount == 0) {
                    start = i;
                }
                toCloseCount++;
            }
            // Decrement the counter on a closing bracket, eventually finding the end of the sublist
            // This string can then be processed through createPacket
            if(s[i] == ']') {
                end = i + 1;
                if(--toCloseCount == 0) {
                    // Create the substring of the nested list
                    // Add the packet of this substring and add it to Packet::subPackets
                    std::string subString = s.substr(start, end - start);
                    tPacket.subPackets.emplace_back(createPacket(subString));
                    diff = s.size() - newS.size();
                    newS.replace(start - diff, end - start, "-1");
                }
            }
        }
    }
    // s can become the temporary string with all sublists removed
    s = newS;
    end = 0;
    // Convert all the strings into integers and add them into Packet::data
    while((start = s.find_first_not_of(',', end)) != std::string::npos) {
        end = s.find(',', start);
        tPacket.data.emplace_back(std::stoi(s.substr(start, end - start)));
    }
    // Return the packet, if there is no data, it will still be added and detected using Packet::data.empty()
    return tPacket;
}

Packet convertToPacket(int i) {
    // When an integer is added compared to a packet, the integer needs to be converted to a packet
    // This is solely done by adding the passed integer to data and return the packet
    Packet p;
    p.data.emplace_back(i);
    return p;
}

State comparePackets(const Packet &p1, const Packet &p2) {
    // Determine the greater data.size() of the two packets to compare
    std::size_t size = (p1.data.size() > p2.data.size()) ? p1.data.size() : p2.data.size();
    // A pair of integers are used to house the counters for iterating through subPackets
    std::pair<int, int> counters = {0,0};
    for(int i = 0; i < size; i++) {
        // If p1 is empty before p2, they are in the correct order
        // If p2 is empty before p1, they are not in the correct order
        if(i == p1.data.size()) {
            return True;
        } else if(i == p2.data.size()) {
            return False;
        }
        // If either p1 or p2 has a -1 in the data[i] it indicates a subpacket
        if(p1.data[i] == -1 || p2.data[i] == -1) {
            std::pair<Packet, Packet> tp;
            // Process the packets as either a subpacket
            // or convert an integer to a packet using convertToPacket
            // Increment the counter of either p1 or p2 (first or second respectively)
            // to access the next subpacket
            if(p1.data[i] == -1) {
                tp.first = p1.subPackets[counters.first++];
            } else {
                tp.first = convertToPacket(p1.data[i]);
            }
            if(p2.data[i] == -1) {
                tp.second = p2.subPackets[counters.second++];
            } else {
                tp.second = convertToPacket(p2.data[i]);
            }
            // If the comparison returns True or False, return
            // If the comparison is indeterminate, continue the loop
            State result = comparePackets(tp.first, tp.second);
            if(result != Unknown) {
                return result;
            }
        } else {
            // If there is no subpacket to process, compare the two integers
            // If they are equal, continue
            if(p1.data[i] < p2.data[i]) {
                return True;
            } else if(p1.data[i] > p2.data[i]) {
                return False;
            }
        }
    }
    // Return an Unknown if no other condition has been met
    return Unknown;
}

void processLine(std::string &s) {
    // If the line is not empty, add a new packet on to packets
    if(!s.empty()) {
        packets.emplace_back(createPacket(s));
    }
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
    std::string temp;
    // currLine is the current position of the line being processed.
    // This is used for the total calculation
    int currLine = 1;
    int total = 0;
    while(getline(input, temp)) {
        processLine(temp);
        // Only compare the two vectors in a pair
        if(packets.size() == 2) {
            State result = comparePackets(packets[0], packets[1]);
            if(result == True) {
               total += currLine;
            }
            currLine++;
            // After processing the packets, clear the vector
            packets.resize(0);
        }
    }
    printf("Total = %i", total);
    return 0;
}
