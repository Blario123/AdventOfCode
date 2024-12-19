#include <cstdio>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

std::vector<std::pair<int,int>> rules;
std::vector<std::vector<int>> incorrectPages;

void addRule(const std::string &str) {
    int n1 = std::stoi(str.substr(0, str.find('|')));
    int n2 = std::stoi(str.substr(str.find('|') + 1));
    rules.emplace_back(std::pair<int,int>(n1, n2));
}

std::vector<int> createPage(const std::string &str) {
    std::vector<int> v;
    size_t sz = 0;
    size_t sz_ = 0;
    while(sz != std::string::npos) {
        sz = str.find(',', sz_);
        v.emplace_back(std::stoi(str.substr(sz_, sz - sz_)));
        sz_ = sz + 1;
    }
    return v;
}

bool contains(const std::vector<int> &v, int i) {
    for(const auto &vi: v) {
        if(vi == i) {
            return true;
        }
    }
    return false;
}

size_t find(const std::vector<int> &v, int i) {
    for(size_t vi = 0; vi < v.size(); vi++) {
        if(v[vi] == i) {
            return vi;
        }
    }
    return -1;
}

void swap(std::vector<int> &v, int a, int b) {
    int t = v[find(v, a)];
    v[find(v, a)] = v[find(v, b)];
    v[find(v, b)] = t;
}

bool checkPage(const std::vector<int> v) {
    for(const auto &i: rules) {
        if(contains(v, i.first) && contains(v, i.second)) {
            if(find(v, i.first) > find(v, i.second)) {
                return false;
            }
        }
    }
    return true;
}

void sortPage(std::vector<int> &v) {
    while(!checkPage(v)) {
        for(const auto &i: rules) {
            if(contains(v, i.first) && contains(v, i.second)) {
                if(find(v, i.first) > find(v, i.second)) {
                    swap(v, i.first, i.second);           
                }
            }
        }
    }
}

int main(int argc, char** argv) {
    if(argc > 1) {
        // Data input
        std::ifstream input;
        input.open(argv[1]);

        std::string line;
        getline(input, line, '\n');
        int sum = 0;
        // Day 1 - (built in to processing) Find good pages with rules provided
        while(input.good()) {
            if(line.find('|') != std::string::npos) {
                addRule(line);
            }
            if(line.find(',') != std::string::npos) {
                std::vector<int> v = createPage(line);
                if(checkPage(v)) {
                    sum += v[floor((float) v.size() / 2.0f)];
                } else {
                    incorrectPages.emplace_back(v);
                }
            }
            getline(input, line, '\n');
        }
        printf("Sum of good pages center elements = %d\n", sum);
        // Day 2 - Find bad pages, sort and find middle element
        sum = 0;
        for(auto &i: incorrectPages) {
            sortPage(i);
            sum += i[floor((float) i.size() / 2.0f)];
        }
        printf("Sum of bad pages center elements = %d\n", sum);
    }
    return 0;
}


