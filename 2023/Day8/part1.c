#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <string.h>

char* path;
size_t pathLength = 0;
char line[1000];
size_t linePos = 0;

struct Element {
    char start[4];
    char opt1c[4];
    char opt2c[4];
    struct Element* opt1;
    struct Element* opt2;
};

struct Element* elements = NULL;
size_t elementsSize = 10;
size_t elementsCount = 0;

void parseLine(const char* l, size_t length) {
    // If the length of the line is 1, or the first character is a newline, return immediately as nothing is to be done.
    if(length == 1 || l[0] == '\n') {
        return;
    }
    if(strstr(l, "=") == NULL) {
        path = malloc(length - 1);
        memcpy(path, l, length - 1);
        pathLength = length - 1;
    } else {
        struct Element tempE;
        char tempWord[3] = {0};
        for(int i = 0; i < 3; i++) {
            tempWord[i] = l[i];
        }
        memcpy(tempE.start, tempWord, 3);
        for(int i = 0; i < 3; i++) {
            tempWord[i] = l[i + 1 + (strstr(l, "(")) - l];
        }
        memcpy(tempE.opt1c, tempWord, 3);
        for(int i = 0; i < 3; i++) {
            tempWord[i] = l[i + 2 + (strstr(l, ", ")) - l];
        }
        memcpy(tempE.opt2c, tempWord, 3);
        tempE.start[3] = '\0';
        tempE.opt1c[3] = '\0';
        tempE.opt2c[3] = '\0';
        tempE.opt1 = NULL;
        tempE.opt2 = NULL;
        if(elementsCount == elementsSize - 1) {
            elementsSize += 10;
            elements = realloc(elements, elementsSize * sizeof(struct Element));
        }
        elements[elementsCount++] = tempE;
    }
}

int main(int argc, char** argv) {
    if(argc > 1) {
        FILE* input = fopen(argv[1], "r");
        if(input == NULL) {
            printf("Could not open file.\n");
            return 1;
        }
        elements = malloc(10 * sizeof(struct Element));
        memset(elements, 0, 10 * sizeof(struct Element));
        char c = fgetc(input);
        
        while(c != EOF) {
            line[linePos++] = c;
            if(c == '\n') {
                parseLine(line, linePos); 
                memset(line, 0, 1000);
                linePos = 0;
            }
            c = fgetc(input);
        }
        struct Element* current;
        for(int i = 0; i < elementsCount; i++) {
            if(strcmp(elements[i].start, "AAA") == 0) {
                current = &elements[i];
            }
            for(int j = 0; j < elementsCount; j++) {
                if(strcmp(elements[j].start, elements[i].opt1c) == 0) {
                    elements[i].opt1 = &elements[j];
                }
                if(strcmp(elements[j].start, elements[i].opt2c) == 0) {
                    elements[i].opt2 = &elements[j];
                }
            }
        }
        uint pathPos = 0;
        uint count = 0;
        while(strcmp(current->start, "ZZZ") != 0) {
            if(pathPos == pathLength) {
                pathPos = 0;
            }
            switch(path[pathPos++]) {
                case 'L':
                    current = current->opt1;
                    break;
                case 'R':
                    current = current->opt2;
                    break;
            }
            count++;
        }
        printf("Number of moves to get to ZZZ = %d\n", count);
    } else {
        printf("No file given.\n");
        return 1;
    }
    
    free(elements);
    return 0;
}
