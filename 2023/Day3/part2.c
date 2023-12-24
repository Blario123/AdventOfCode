#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

char lines[1000][1000] = {{0}};
int length;

struct Number {
    int n;
    int x;
    int y;
    int length;
};

struct Gear {
    int x;
    int y;
};

struct Number numbers[3000] = {0};
int numberCount = 0;

struct Gear gears[500] = {0};
int gearCount = 0;

int main(int argc, char** argv) {
    if(argc > 1) {
        FILE* input = fopen(argv[1], "r");
        if(input == NULL) {
            printf("Could not open file.\n");
            return 1;
        }
        char c = fgetc(input);
        int x = 0;
        int y = 0;
        int temp = 0;
        struct Number tempNum = {0, 0, 0, 0};
        while(c != EOF) {
            lines[y][x++] = c; 
            if(c == '\n') {
                for(int i = 0; i < x; i++) {
                    char cur = lines[y][i];
                    if(cur == '*') {
                        if(temp > 0) {
                            tempNum.n = temp;
                            tempNum.length = i - tempNum.x;
                            numbers[numberCount++] = tempNum;
                            tempNum.n = 0;
                            tempNum.x = 0;
                            tempNum.y = 0;
                            tempNum.length = 0;
                            temp = 0;
                        }
                        struct Gear tempGear = {i, y};
                        gears[gearCount++] = tempGear;
                    }else if(cur >= '0' && cur <= '9') {
                        if(temp > 0) {
                            temp *= 10;
                            temp += atoi(&cur);
                        } else {
                            tempNum.x = i;
                            tempNum.y = y;
                            temp = atoi(&cur);
                        }
                    } else {
                        if(temp > 0) {
                            tempNum.n = temp;
                            tempNum.length = i - tempNum.x;
                            numbers[numberCount++] = tempNum;
                            tempNum.n = 0;
                            tempNum.x = 0;
                            tempNum.y = 0;
                            tempNum.length = 0;
                            temp = 0;
                        }
                    }
                }
                x = 0;
                y++;
            }
            c = fgetc(input);
        }
        unsigned long long count = 0;
        for(uint i = 0; i < gearCount; i++) {
            int lower = 1;
            int upper = 1;
            int left = 1;
            int right = 1;
            struct Gear* g = &gears[i];
            if(g->x == 0) {
                left = 0;
            }
            if(g->x == strlen(lines[0])) {
                right = 0;
            }
            if(g->y == 0) {
                upper = 0;
            }
            if(g->y == y) {
                lower = 0;
            }
            uint valid = 0;
            unsigned long long t = 1;
            bool numberFound = false;
            for(uint j = 0; j < numberCount; j++) {
                struct Number* n = &numbers[j];
                for(uint y_ = g->y - upper; y_ <= g->y + lower; y_++) {
                    for(uint x_ = g->x - left; x_ <= g->x + right; x_++) {
                        for(uint k = 0; k < n->length; k++) {
                            if(y_ == n->y && x_ == n->x + k) {
                                if(!numberFound) {
                                    t *= n->n;
                                    valid++;
                                    numberFound = true;
                                }
                            }
                        }
                    }
                }
                numberFound = false;
            }
            if(valid == 2) {
                count += t;
            }
        }
        printf("Part number sum = %llu\n", count);
    } else {
        printf("No file given.\n");
        return 1;
    }
    return 0;
}
