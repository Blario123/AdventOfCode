#include <iostream>
#include <fstream>
#include <vector>

std::ifstream input;
struct Packet;

struct Packet {
    std::vector<int> data;
    std::vector<Packet> subPackets;
};

std::vector<Packet> packets;

Packet createPacket(const std::vector<std::string> &v) {

}

bool comparePackets() {

}

void processLine(const std::string &s) {
    if(s.empty()) {
        return;
    }
    std::vector<std::string> items;
    // Split the items into substrings of lists, or integers
    // Allocate list items as -1 to detect when processing
    std::size_t start;
    std::size_t end = 1;
    while((start = s.find_first_not_of(',', end)) != std::string::npos) {
        end = s.find(',', start);
        items.emplace_back(s.substr(start, end - start));
    }
    for(auto i: items) {
        if(i.find('[') != std::string::npos) {
            printf("New list");
        }
        printf("%s\n", i.c_str());
    }
    printf("\n");
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
