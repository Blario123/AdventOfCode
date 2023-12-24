#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

uint times[4] = {0};
uint distances[4] = {0};
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
        int temp = 0;
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
                    if(c == ' ' && temp > 0) {
                        times[timeCount++] = temp;
                        temp = 0;
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
                if(c == ' ' && temp > 0) {
                    distances[distCount++] = temp;
                    temp = 0;
                }
            }
            c = fgetc(input);
        }
        distances[distCount++] = temp;
        uint value = 0;
        uint count = 0;
        for(int i = 0; i < timeCount; i++) {
            for(int j = 0; j < times[i]; j++) {
                uint distTravelled = j * (times[i] - j);
                if(distTravelled > distances[i]) {
                    count++;
                }
            }
            if(value > 0) {
                value *= count;
                count = 0;
            } else {
                value = count;
                count = 0;
            }
        }
        printf("Multiplied scores = %d\n", value);
    } else {
        printf("No file given.\n");
        return 1;
    }
    return 0;
}
