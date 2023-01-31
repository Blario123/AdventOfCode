#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

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

Packet createPacket(const std::string &s) {
    Packet tPacket;
    // Remove leading and trailing '[' and ']'
    std::string newS = s.substr(1, s.size() - 2);
    std::string tempS = newS;
    std::size_t start;
    std::size_t end;
    // Reduce child lists to a -1 to denote the position when processed
    // If no '[' is found in the string, process all the integers
    if(std::count(newS.begin(), newS.end(), '[') > 0) {
        // Keep track of the depth of nested lists
        int toCloseCount = 0;
        std::size_t diff;
        // Iterate through the string, replacing a duplicate temporary string list with "-1"
        for(int i = 0; i < newS.size(); i++) {
            // If a new '[' is added, set the start for the first instance, and increment the depth
            if(newS[i] == '[') {
                if(toCloseCount == 0) {
                    start = i;
                }
                toCloseCount++;
            }
            // Decrement the counter on a closing bracket, eventually finding the end of the sublist
            // This string can then be processed through createPacket
            if(newS[i] == ']') {
                end = i + 1;
                if(--toCloseCount == 0) {
                    // Create the substring of the nested list
                    // Add the packet of this substring and add it to Packet::subPackets
                    std::string subString = newS.substr(start, end - start);
                    tPacket.subPackets.emplace_back(createPacket(subString));
                    diff = newS.size() - tempS.size();
                    tempS.replace(start - diff, end - start, "-1");
                }
            }
        }
    }
    // s can become the temporary string with all sublists removed
    newS = tempS;
    end = 0;
    // Convert all the strings into integers and add them into Packet::data
    while((start = newS.find_first_not_of(',', end)) != std::string::npos) {
        end = newS.find(',', start);
        tPacket.data.emplace_back(std::stoi(newS.substr(start, end - start)));
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

void sortPackets(std::vector<Packet> &v) {
    bool sorted = false;
    while(!sorted) {
        std::size_t inPos = 0;
        std::vector<Packet> tV = v;
        for(int i = 0; i < v.size() - 1; i++) {
            State result = comparePackets(v[i], v[i + 1]);
            switch(result) {
                case True:
                    // Do not need to rearrange the elements
                    inPos++;
                    break;
                case False:
                    // The elements can be swapped
                    std::iter_swap(tV.begin() + i, tV.begin() + i + 1);
                    break;
                case Unknown:
                    break;
            }
        }
        if(inPos == v.size() - 1) {
            sorted = true;
        }
        v = tV;
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
    // Process the lines of the file, adding new Packet elements to packets
    std::string temp;
    while(getline(input, temp)) {
        processLine(temp);
    }
    // currLine is the current position of the line being processed
    // This is used for the total calculation
    int currLine = 1;
    int total = 0;
    // Only compare the two vectors in a pair
    for(int i = 0; i < packets.size(); i += 2) {
        State result = comparePackets(packets[i], packets[i + 1]);
        if(result == True) {
            total += currLine;
        }
        currLine++;
    }
    printf("Total = %i\n", total);
    // Add the divider packets
    packets.emplace_back(createPacket("[[2]]"));
    packets.emplace_back(createPacket("[[6]]"));
    // Sort the packets
    sortPackets(packets);
    std::vector<int> keyPos;
    for(int i = 0; i < packets.size(); i++) {
        if(packets[i].subPackets.size() != 1) {
            continue;
        }
        if(packets[i].subPackets[0].data.size() != 1) {
            continue;
        }
        int data = packets[i].subPackets[0].data[0];
        if(data == 2 || data == 6) {
            keyPos.push_back(i + 1);
        }
    }
    printf("Decoder key = %i", keyPos[0] * keyPos[1]);
    return 0;
}
