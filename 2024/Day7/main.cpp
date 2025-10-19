#include <cstdio>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <cstdint>

std::vector<unsigned long long> split(std::string str) {
    std::vector<unsigned long long> retVec;
    std::string s;
    for(int i = 0; i < str.size(); i++) {
        char j = str[i];
        if(j == ' ' || j == '\0') {
            retVec.emplace_back(std::stoull(s));
            s = "";
        } else {
            s.push_back(j);
        }
    }
    if(!s.empty()) {
        retVec.emplace_back(std::stoull(s));
    }
    return retVec;
}

uint64_t checkLine(unsigned long long goal, const std::vector<unsigned long long> &v) {
    int possPermutations = std::pow(2, v.size() - 1);
    // Cheaper to check the addition and multiplication permutations immediately.
    unsigned long long addSum = v[0];
    unsigned long long mulSum = v[0];
    for(size_t i = 1; i < v.size(); i++) {
        addSum += v[i];
        mulSum *= v[i];
    }
    if(addSum == goal || mulSum == goal) {
        return goal;
    }
    // If neither worked, determine a mix of symbols
    int tried = 1;
    while(tried != possPermutations - 1) {
        unsigned long long mixSum = v[0];
        // 0b0 == +
        // 0b1 == *
        int mask = 0b1;
        //std::string binRep = "";
        for(int i = 1; i < v.size(); i++) {
            if(tried & mask) {
                mixSum *= v[i];
            } else {
                mixSum += v[i];
            }
            //binRep.append((tried & mask) ? "*" : "+");
            mask = mask << 1;
        }
        if(mixSum == goal) {
            return goal;
        }
        tried++;
    }
    return -1; 
}

std::vector<std::vector<std::string>> cartesianProduct(std::vector<std::string> p, size_t len) {
    std::vector<std::vector<std::string>> temp = {{}};
    for(int i = 0; i < len; i++) {
        std::vector<std::vector<std::string>> newTemp;
        for(const std::vector<std::string>& product: temp) {
            for(const std::string& element: p) {
                std::vector<std::string> tempCopy = product;
                tempCopy.push_back(element);
                newTemp.push_back(tempCopy);
            }
        }
        temp = newTemp;
    }
    return temp;
}

std::string vecToString(std::vector<std::string> s) {
    std::string i;
    for(auto &j: s) {
        i.append(j);
    }
    return i;
}

uint64_t checkLineConcat(unsigned long long goal, const std::vector<unsigned long long> &v) {
    // printf("Goal = %llu\n", goal);
    int possPermutations = std::pow(3, v.size() - 1);
    // Cheaper to check the addition and multiplication permutations immediately.
    unsigned long long addSum = v[0];
    unsigned long long mulSum = v[0];
    unsigned long long concatSum = v[0];
    for(size_t i = 1; i < v.size(); i++) {
        addSum += v[i];
        mulSum *= v[i];
        concatSum *= pow(10, std::to_string(v[i]).length());
        concatSum += v[i];
    }
    if(addSum == goal || mulSum == goal || concatSum == goal) {
        // printf("addSum = %llu, mulSum = %llu, concatSum = %llu\n", addSum, mulSum, concatSum);
        return goal;
    }
    // If neither worked, determine a mix of symbols
    std::vector<std::string> p = {"*", "+", "||"};


    const auto& ret = cartesianProduct(p, v.size() - 1);
    for(int i = 0; i < ret.size(); i++) {
        std::string opString = vecToString(ret[i]);
        // printf("Goal is %llu using %s\n", goal, opString.c_str());
        unsigned long long mixSum = v[0];
        for(int k = 0; k < ret[i].size(); k++) {
            std::string j = ret[i][k];
            int l = k + 1;
            if(j == "*") {
                // printf("%llu * %llu\n", mixSum, v[l]);
                mixSum *= v[l];
            } else if(j == "+") {
                // printf("%llu + %llu\n", mixSum, v[l]);
                mixSum += v[l];
            } else if(j == "||") {
                // printf("%llu || %llu\t", mixSum, v[l]);
                size_t shiftAmount = pow(10,std::to_string(v[l]).length());
                // printf("shifting by %lu\n", shiftAmount);
                // printf("mixSum = %llu", mixSum);
                mixSum *= shiftAmount;
                // printf("shifted = %llu", mixSum);
                mixSum += v[l];
                // printf("added = %llu\n", mixSum); 
            }
            // printf("sum now = %llu\n", mixSum);
        }
        if(mixSum == goal) {
            // printf("mixSum = %llu\n", mixSum);
            return goal;
        }
    }
    return -1; 
}

int main(int argc, char** argv) {
    if(argc > 1) {
        // Data input
        std::ifstream input;
        input.open(argv[1]);

        std::string line;
        getline(input, line, '\n');
        uint64_t sum1 = 0;
        uint64_t sum2 = 0;
        uint64_t sz = 0;
        while(input.good()) {
            sz = line.find(':');
            long long g = std::stoll(line.substr(0, sz++).c_str());
            size_t sz_ = sz + 1;
            const auto& v = split(line.substr(sz_, std::string::npos));
            uint64_t l1 = checkLine(g, v);
            uint64_t l2 = checkLineConcat(g, v);
            if(l1 != -1) {
                sum1 += l1; 
            }
            if(l2 != -1) {
                sum2 += l2;
            }
            getline(input, line, '\n');
        }
        printf("Count of working lines = %lu\n", sum1);
        printf("Count of new working lines = %lu\n", sum2);
    }
    return 0;
}


