#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

std::vector<int> left;
std::vector<int> right;

typedef struct {
    bool done;
    int count;
} check_t;

check_t isDone(int check, std::vector<std::pair<int, int>> &vec) {
    for(auto &i: vec) {
        if(i.first == check) {
            return check_t{true, i.second};
        }
    }
    return check_t{false, 0};
}

int main(int argc, char** argv) {
    if(argc > 1) {
        // Data input
        std::ifstream input;
        input.open(argv[1]);

        std::string line;
        getline(input, line, '\n');
        std::size_t sz;
        while(input.good()) {
            int n1 = std::stoi(line, &sz);
            int n2 = std::stoi(line.substr(sz));
            left.emplace_back(n1);
            right.emplace_back(n2);
            getline(input, line, '\n');
        }
        // Day 1 - sort and delta
        auto left1 = left;
        auto right1 = right;
        std::stable_sort(left1.begin(), left1.end());
        std::stable_sort(right1.begin(), right1.end());
        int sum = 0;
        for(int i = 0; i < left.size(); i++) {
            sum += abs(left1[i] - right1[i]);
        }
        printf("Total Distance = %d\n", sum);
        // Day 2 - duplicates
        sum = 0;
        std::vector<std::pair<int, int>> done;
        for(int i = 0; i < left.size(); i++) {
            check_t c = isDone(left[i], done);
            if(c.done) {
                sum += left[i] * c.count;
            } else {
                int iCount = 0;
                for(int j = 0; j < right.size(); j++) {
                    if(left[i] == right[j]) {
                        iCount++;
                    }
                }
                done.emplace_back(std::pair<int, int>(left[i], iCount));
                sum += left[i] * iCount;
            }
        }
        printf("Total Similarity = %d\n", sum);
    }
    return 0;
}


