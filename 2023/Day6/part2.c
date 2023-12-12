#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

unsigned long long times[4] = {0};
unsigned long long distances[4] = {0};
int timeCount = 0;
int distCount = 0;

int main(int argc, char** argv) {
    if(argc > 1) {
        FILE* input = fopen(argv[1], "r");
        if(input == NULL) {
            printf("Could not open file.\n");
            return 1;
        }
        char c = fgetc(input);
        bool first = true;
        unsigned long long temp = 0;
        while(c != EOF) {
            if(first) {
                if(c == '\n') {
                    if(temp > 0) {
                        times[timeCount++] = temp;
                        temp = 0;
                    }
                    first = false;
                } else {
                    if(c >= '0' && c <= '9') {
                        if(temp > 0) {
                            temp *= 10;
                            temp += atoi(&c);
                        } else {
                            temp = atoi(&c);
                        }
                    }
                }
            } else {
                if(c >= '0' && c <= '9') {
                    if(temp > 0) {
                        temp *= 10;
                        temp += atoi(&c);
                    } else {
                        temp = atoi(&c);
                    }
                }
            }
            c = fgetc(input);
        }
        distances[distCount++] = temp;
        unsigned long long count = 0;
        for(int i = 0; i < timeCount; i++) {
            for(unsigned long long j = 0; j < times[i]; j++) {
                unsigned long long distTravelled = j * (times[i] - j);
                if(distTravelled > distances[i]) {
                    count++;
                }
            }
        }
        printf("Number of times record beaten = %llu\n", count);
    } else {
        printf("No file given.\n");
        return 1;
    }
    return 0;
}
