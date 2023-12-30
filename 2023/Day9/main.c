#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

struct Sequence {
    int* numbers;
    uint numbersSize;
    int** subsequences;
    uint subsequencesSize;
};

char line[1000];
size_t linePos = 0;

struct Sequence* sequences;
uint sequenceSize = 2;
uint sequenceCount = 0;

void parseLine(const char* l, size_t length) {
    // If the length of the line is 1, or the first character is a newline, return immediately as nothing is to be done.
    if(length == 1 || l[0] == '\n') {
        return;
    }
    struct Sequence tempSeq;
    uint count = 2;
    tempSeq.numbers = malloc(count);
    int tempNo;
    for(int i = 0; i < length; i++) {
        if(l[i] > '0' && l[i] < '9') {
            if(tempNo > 0) {
                tempNo *= 10;
                tempNo += atoi(&l[i]);
            } else {
                tempNo = atoi(&l[i]);
            }
        }
        if(tempNo > 0 && (l[i] == ' ' || i == length - 1)) {
            tempSeq.numbers[count++] = tempNo;
            // TODO enumerate through line and allocate required memory. 
            if(;;) {
            }
            tempNo = 0;
        }
    }
    sequences[sequenceCount] = tempSeq;
    if(sequenceCount == sequenceSize) {
        sequenceSize *= 2;
        sequences = realloc(sequences, sequenceSize * sizeof(struct Sequence));
    }
}

int main(int argc, char** argv) {
    if(argc > 1) {
        FILE* input = fopen(argv[1], "r");
        if(input == NULL) {
            printf("Could not open file.\n");
            return 1;
        }
        sequences = malloc(sequenceSize * sizeof(struct Sequence));
        char c = fgetc(input);
        while(c != EOF) {
            line[linePos++] = c;
            if(c == '\n') {
                parseLine(line, linePos);
                memset(line, 0, 1000 * sizeof(char));
                linePos = 0;
            }
            c = fgetc(input);
        }
    } else {
        printf("No file given.\n");
        return 1;
    }
    return 0;
}
