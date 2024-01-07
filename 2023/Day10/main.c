#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

char** map;
int x = 0;
int y = 0;
int xMax = 0;
int yMax = 2;

int startX;
int startY;

char line[1024];
size_t linePos = 0;

typedef enum {
    North = 0,
    East,
    South,
    West
} direction;

void parseLine(const char* l, size_t length) {
    // If the length of the line is 1, or the first character is a newline, return immediately as nothing is to be done.
    if(length == 1 || l[0] == '\n') {
        return;
    }
    char* sPos = strstr(l, "S");
    if(sPos != NULL) {
        startY = y;
        startX = sPos - l;
    }
    x = length - 1;
    xMax = length - 1;
    map[y] = malloc(xMax * sizeof(char));
    memcpy(map[y], l, xMax);
    printf("Line is %s\n", map[y]);
    if(y == yMax - 1) {
        yMax += 2;
        map = realloc(map, yMax * sizeof(char*));
    }
    y++;
}

int main(int argc, char** argv) {
    if(argc > 1) {
        FILE* input = fopen(argv[1], "r");
        if(input == NULL) {
            printf("Could not open file.\n");
            return 1;
        }
        map = malloc(yMax * sizeof(char*));
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
        bool end = false;
        int distance = 0;
        bool deadend = false;
        direction dir = 0;
        char current = 'S';
        int x_ = startX;
        int y_ = startY;
        char previous;
        while(!end) {
            switch(dir) {
                case North:
                    current = map[--y_][x_];
                    break;
                case East:
                    current = map[y_][++x_];
                    break;
                case South:
                    current = map[++y_][x_];
                    break;
                case West:
                    current = map[y_][--x_];
                    break;
            }
            switch(current) {
                case '-':
                    if(previous == 'F' || previous == 'L') {
                        
                    } else if(previous == 'J' || previous == '7') {
                    
                    }
                    break;
                case '7':
                    break;
                case '|':
                    if(previous == 'F' || previous == '7') {
                    } else if(previous == 'J' || previous == 'L') {
                    }
                    break;
                case 'J':
                    break;
                case 'L':
                    break;
                case 'F':
                    break;
            }
            if(current == '.') {
                deadend = true;
            }
            if(deadend) {
                x_ = startX;
                y_ = startX;
                current = 'S';
                distance = 0;
                deadend = false;
                dir++;
            }
            if(current == 'S' && distance > 0) {
                end = true;
            }
        }
        printf("X = %d, Y = %d, %c\n", startX, startY, map[startY][startX]);
    } else {
        printf("No file given.\n");
        return 1;
    }
    return 0;
}
