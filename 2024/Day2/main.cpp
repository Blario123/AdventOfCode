#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

std::vector<std::vector<int>> reports;

std::vector<int> processLine(std::string l) {
    std::vector<int> r;
    std::size_t sz = 0;
    std::size_t spaceCount = std::count(l.begin(), l.end(), ' ');
    for(int i = 0; i < spaceCount + 1; i++) {
        l = l.substr(sz);
        r.emplace_back(std::stoi(l, &sz));
    }
    return r;
}

bool checkReports(const std::vector<int> &r) {
    int inc = 0;
    int dec = 0;
    bool pass = false;
    for(int j = 0; j < r.size() - 1; j++) {
        int delta = r[j + 1] - r[j];             
        if(abs(delta) > 3 || delta == 0) {
            return false;
        }
        if(delta > 0) {
            inc++;
        } else if(delta < 0) {
            dec++;
        }
    }
    if(inc == 0 || dec == 0) {
        return true;
    }
    return false;
}

int main(int argc, char** argv) {
    if(argc > 1) {
        // Data input
        std::ifstream input;
        input.open(argv[1]);

        std::string line;
        getline(input, line, '\n');
        while(input.good()) {
            reports.emplace_back(processLine(line));
            getline(input, line, '\n');
        }
        // Day 1 - all (in/de)creasing and step
        int sum = 0;
        for(auto &i: reports) {
            if(checkReports(i)) {
                sum++;
            }
        }
        printf("Safe reports = %d\n", sum);
        // Day 2 - day 1, with problem dampener.
        sum = 0;
        for(auto &i: reports) {
            if(checkReports(i)) {
                sum++;
            } else { // If actual did not pass, begin removing individual reports
                for(int j = 0; j < i.size(); j++) {
                    std::vector<int> copy = i;
                    copy.erase(copy.begin() + j);
                    if(checkReports(copy)) {
                        sum++;
                        break;
                    }
                }
            }
        }
        printf("Safe reports = %d\n", sum);
        
    }
    return 0;
}


