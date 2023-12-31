#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

struct Sequence {
    int* numbers;
    uint numbersSize;
    int* lastnumbers;
};

char line[1000];
size_t linePos = 0;
unsigned long long score = 0;

void parseLine(const char* l, size_t length) {
    // If the length of the line is 1, or the first character is a newline, return immediately as nothing is to be done.
    if(length == 1 || l[0] == '\n') {
        return;
    }
    struct Sequence tempSeq;
    uint count = 0;
    tempSeq.numbersSize = 2;
    tempSeq.numbers = malloc(tempSeq.numbersSize * sizeof(int));
    int tempNo = 0;
    bool negative = false;
    for(int i = 0; i < length; i++) {
        if(l[i] == '-') {
            negative = true;
        }
        if(l[i] >= '0' && l[i] <= '9') {
            char c = l[i];
            if(tempNo > 0) {
                tempNo *= 10;
                tempNo += atoi(&c);
            } else {
                tempNo = atoi(&c);
            }
        }
        if(l[i] == ' ' || i == length - 1) {
            if(negative) {
                tempNo *= -1;
            }
            tempSeq.numbers[count++] = tempNo;
            if(count == tempSeq.numbersSize) {
                tempSeq.numbersSize += 2;
                tempSeq.numbers = realloc(tempSeq.numbers, tempSeq.numbersSize * sizeof(int));
            }
            negative = false;
            tempNo = 0;
        }
    }
    // Reset the numbersSize to the actual count, not the allocated size of the array.
    tempSeq.numbersSize = count;
    // Iterate through each sequence. In a while loop, create the next sequence until all elements of the next sequence equal 0. 
    uint depth = 0;
    
    tempSeq.lastnumbers = malloc((tempSeq.numbersSize - 1) * sizeof(int));
    memset(tempSeq.lastnumbers, 0, (tempSeq.numbersSize - 1) * sizeof(int));
    
    int* subsequence = malloc(tempSeq.numbersSize * sizeof(int));
    memcpy(subsequence, tempSeq.numbers, tempSeq.numbersSize * sizeof(int));
    
    tempSeq.lastnumbers[depth++] = tempSeq.numbers[tempSeq.numbersSize - 1];

    free(tempSeq.numbers);
    tempSeq.numbers = NULL;

    bool allZeros = false;
    int tempSize = tempSeq.numbersSize;
    while(!allZeros) {
        int* temp = malloc(tempSize * sizeof(int));
        memset(temp, 0, tempSize * sizeof(int));
        for(int j = 1; j < tempSize; j++) {
            int element1 = subsequence[j];
            int element2 = subsequence[j-1];
            int delta = element1 - element2;
            temp[j-1] = delta;
            if(j == tempSize - 1) {
                tempSeq.lastnumbers[depth++] = temp[j - 1];
            }
        }
        tempSize--;
        for(int j = 0; j < tempSize; j++) {
            if(temp[j] != 0) {
                allZeros = false;
                break;
            }
            allZeros = true;
        }
        memcpy(subsequence, temp, tempSize * sizeof(int));
        free(temp);
    }
    int carry = 0;
    for(int j = depth - 1; j >= 0; j--) {
        carry += tempSeq.lastnumbers[j];
    }
    score += carry;
    free(tempSeq.numbers);
    free(subsequence);
}

int main(int argc, char** argv) {
    if(argc > 1) {
        FILE* input = fopen(argv[1], "r");
        if(input == NULL) {
            printf("Could not open file.\n");
            return 1;
        }
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
        printf("Total sum of the histories = %llu\n", score);
    } else {
        printf("No file given.\n");
        return 1;
    }
    return 0;
}
