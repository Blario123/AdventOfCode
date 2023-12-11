#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define min(a,b) ({__typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a < _b ? _a : _b;})

char line[1000] = {0x00};
uint seeds[100] = {0};
bool seedsOk[100] = {0};
uint seedCount = 0;

struct Map {
    uint destRangeStart;
    uint sourceRangeStart;
    uint rangeLength;
};

struct Map maps[1000] = {{0, 0, 0}};
uint mapCount = 0;

uint mapLengths[8] = {0};
uint lengthCount = 0;

int main(int argc, char** argv) {
    if(argc > 1) {
        FILE* input = fopen(argv[1], "r");
        if(input == NULL) {
            printf("Could not open file.\n");
            return 1;
        }
        size_t pos = 0;
        bool doNext = false;
        uint pass = 0;
        char c = fgetc(input);
        uint length = 0;
        memset(&seedsOk, false, 100);
        while(c != EOF) {
            line[pos++] = c;
            struct Map tempMap = {0, 0, 0};
            if(c == '\n') {
                uint temp = 0;
                pos--;
                char* l = strstr(line, "s:");
                if(l != NULL) {
                    for(size_t i = l - line + 2; i < pos; i++) {
                        char cur = line[i];
                        if(cur >= '0' && cur <= '9') {
                            if(temp > 0) {
                                temp *= 10;
                                temp += atoi(&cur);
                            } else {
                                temp += atoi(&cur);
                            }
                        }
                        if(temp > 0 && (cur == ' ' || i == (pos - 1))) {
                            seeds[seedCount++] = temp;
                            temp = 0;
                        }
                    }
                } else {
                    temp = 0;
                    l = strstr(line, ":");
                    if(l == NULL) {
                        size_t lineLen = strlen(line);
                        for(size_t i = 0; i < lineLen; i++) {
                            char cur = line[i];
                            if(cur >= '0' && cur <= '9') {
                                if(temp > 0) {
                                    temp *= 10;
                                    temp += atoi(&cur);
                                } else {
                                    temp = atoi(&cur);
                                }
                            }
                            if(cur == '\n' && i == 0) {    
                                mapLengths[lengthCount++] = length;
                                length = 0;
                            }
                            if(cur == ' ' || (i == (lineLen - 1) && i != 0)) {
                                switch(pass++) {
                                    case 0:
                                        tempMap.destRangeStart = temp;
                                        break;
                                    case 1:
                                        tempMap.sourceRangeStart = temp;
                                        break;
                                    case 2:
                                        tempMap.rangeLength = temp;
                                        maps[mapCount++] = tempMap;
                                        length++;
                                        pass = 0;
                                        tempMap.destRangeStart = 0;
                                        tempMap.sourceRangeStart = 0;
                                        tempMap.rangeLength = 0;
                                        break;
                                }
                                temp = 0;
                            }
                        }
                    }
                }
                pos = 0;
                memset(&line, 0x00, 1000);
            }
            c = fgetc(input);
        }
        mapLengths[lengthCount++] = length;
        uint j = 0;
        bool mapOk = false;
        for(uint i = 1; i < lengthCount; i++) {
            memset(&seedsOk, false, 100);
            for(uint ii = 0; ii < mapLengths[i]; ii++) {
                struct Map* m = &maps[j + ii];
                for(uint jj = 0; jj < seedCount; jj++) {
                    if(seedsOk[jj] == false && seeds[jj] >= m->sourceRangeStart && seeds[jj] <= (m->sourceRangeStart + m->rangeLength - 1)) {
                        seedsOk[jj] = true;
                        seeds[jj] -= (m->sourceRangeStart - m->destRangeStart);
                    }
                }
            }
            j += mapLengths[i];
        }
        uint lowest = seeds[0];
        for(uint i = 1; i < seedCount; i++) {
            if(seeds[i] < lowest) {
                lowest = seeds[i];
            }
        }
        printf("Starting location = %d\n", lowest);
    } else {
        printf("No file given.\n");
        return 1;
    }
    return 0;
}
