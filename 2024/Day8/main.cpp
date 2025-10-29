#include <cstdio>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>

std::vector<std::string> map;

class Channel {
public:
    char identifier;
    std::vector<std::pair<int, int>> locations;
    std::vector<std::pair<int, int>> antinodes;
};

std::vector<Channel> channels;

void processMap(void) {
    for(int i = 0; i < map.size(); i++) {
        for(int j = 0; j < map[i].size(); j++) {
            if(map[i][j] != '.') {
                // Check if the current channel exists in channels;
                bool channelFound = false;
                for(int k = 0; k < channels.size(); k++) {
                    Channel* c = &channels[k];
                    if(c->identifier == map[i][j]) {
                        // Channel exists
                        c->locations.emplace_back(std::pair(j, i));
                        channelFound = true;
                    }
                }
                if(!channelFound) {
                    Channel ch;
                    ch.identifier = map[i][j];
                    ch.locations.emplace_back(std::pair(j, i));
                    channels.emplace_back(ch);
                }
            }
        }
    }
}

void printChannels() {
    for(const auto& i: channels) {
        printf("Channel %c\n", i.identifier);
        for(const auto& j: i.locations) {
            printf("X = %d, Y = %d\n", j.first, j.second);
        }
    }
}

void printAntinodes(std::vector<Channel> v) {
    for(const auto& i: v) {
        printf("Channel %c antinodes:\n", i.identifier);
        for(const auto& j: i.antinodes) {
            printf("X = %d, Y = %d\n", j.first, j.second);
        }
    }
}

void createAntinodes(Channel* c) {
    for(int i = 0; i < c->locations.size(); i++) {
        for(int j = 0; j < c->locations.size(); j++) {
            // Skip the same element, delta will always be 0.
            if(j != i) {
                int xDelta = c->locations[i].first - c->locations[j].first; 
                int yDelta = c->locations[i].second - c->locations[j].second;
                std::pair<int, int> anode = {c->locations[i].first + xDelta, c->locations[i].second + yDelta};
                // If anode location is outside of map, skip
                if(anode.first < 0 || anode.first >= map[0].size() || anode.second < 0 || anode.second >= map.size()) {
                    continue;
                }
                // If the anode exists as a item within the channel do not add.
                std::vector<std::pair<int, int>>* locs = &c->locations;
                bool isLocation = false;
                for(auto &m: *locs) {
                    if(anode.first == m.first && anode.second == m.second) {
                        isLocation = true;
                    }
                }
                if(!isLocation) {
                    c->antinodes.emplace_back(anode);
                }
            }
        }
    }
}

void createContinuousAntinodes(Channel* c) {
    for(int i = 0; i < c->locations.size(); i++) {
        for(int j = 0; j < c->locations.size(); j++) {
            // Skip the same element, delta will always be 0.
            if(j != i) {
                int xDelta = c->locations[i].first - c->locations[j].first; 
                int yDelta = c->locations[i].second - c->locations[j].second;
                // Continue until edge of map found.
                std::pair<int, int> *prev = nullptr;
                while(true) {
                    std::pair<int, int> anode;
                    if(prev == nullptr) {
                        anode = {c->locations[i].first + xDelta, c->locations[i].second + yDelta};
                    } else {
                        anode = {prev->first + xDelta, prev->second + yDelta};
                    }
                    // If anode location is outside of map, break
                    if(anode.first < 0 || anode.first >= map[0].size() || anode.second < 0 || anode.second >= map.size()) {
                        break;
                    }
                    // If the anode exists as a item within the channel do not add.
                    std::vector<std::pair<int, int>>* locs = &c->locations;
                    bool isLocation = false;
                    // Check for all channels
                    for(const auto& ma: channels) {
                        for(auto &m: ma.locations) {
                            if(anode.first == m.first && anode.second == m.second) {
                                isLocation = true;
                            }
                        }
                    }
                    if(!isLocation) {
                        c->antinodes.emplace_back(anode);
                    }
                    prev = new std::pair<int, int>;
                    prev->first = anode.first;
                    prev->second = anode.second;
                }
            }
        }
    }
}

int calculateTotalAnodes(std::vector<Channel>* c) {
    std::vector<std::pair<int, int>> allAnodes;
    for(int i = 0; i < c->size(); i++) {
        Channel* ch = &c->at(i);
        for(int j = 0; j < ch->locations.size(); j++) {
            allAnodes.emplace_back(ch->locations[j]);
        }
        for(int j = 0; j < ch->antinodes.size(); j++) {
            std::pair<int, int> node = ch->antinodes[j];
            bool isInAllAnodes = false;
            for(int k = 0; k < allAnodes.size(); k++) {
                std::pair<int, int> aaNode = allAnodes[k];
                if(node.first == aaNode.first && node.second == aaNode.second) {
                    isInAllAnodes = true;
                }
            }
            if(!isInAllAnodes) {
                allAnodes.emplace_back(node);
            }
        }
    }
    return allAnodes.size();
}

int main(int argc, char** argv) {
    if(argc > 1) {
        // Data input
        std::ifstream input;
        input.open(argv[1]);

        std::string line;
        getline(input, line, '\n');
        while(input.good()) {
            map.emplace_back(line);
            getline(input, line, '\n');
        }
        processMap();
        std::vector<Channel> channelsCopy;
        std::copy(channels.begin(), channels.end(), std::back_inserter(channelsCopy));
        // printChannels();
        for(int i = 0; i < channels.size(); i++) {
            createAntinodes(&channels[i]);
            createContinuousAntinodes(&channelsCopy[i]);
        }
        // printAntinodes(channelsCopy);
        printf("Total number of antinodes = %d\n", calculateTotalAnodes(&channels));
        printf("Total number of continuous antinodes = %d\n", calculateTotalAnodes(&channelsCopy));
    }
    return 0;
}

