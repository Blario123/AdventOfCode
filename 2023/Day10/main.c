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
        direction startDir = 0;
        direction dir = 0;
        direction prevDir = startDir;
        char current = 'S';
        int x_ = startX;
        int y_ = startY;
        char previous = '.';
        int northCount = 0;
        int southCount = 0;
        int eastCount = 0;
        int westCount = 0;
        while(!end) {
            deadend = false;
            prevDir = dir;
            printf("Attempting to move ");
            switch(dir) {
                case North:
                    printf("North");
                    if(y_ == 0) {
                        deadend = true;
                        break;
                    }
                    northCount++;
                    --y_;
                    break;
                case East:
                    printf("East");
                    if(x_ == xMax) {
                        deadend = true;
                        break;
                    }
                    eastCount++;
                    ++x_;
                    break;
                case South:
                    printf("South");
                    if(y_ == yMax) {
                        deadend = true;
                        break;
                    }
                    southCount++;
                    ++y_;
                    break;
                case West:
                    printf("West");
                    if(x_ == 0) {
                        deadend = true;
                        break;
                    }
                    westCount++;
                    --x_;
                    break;
            }
            if(prevDir > 4) {
                return -1;
            }
            current = map[y_][x_];
            printf(" to %d,%d\n", x_, y_);
            printf("current = %c prevDir = %d\n", current, prevDir);
            switch(current) {
                case '-':
                    if(prevDir == East || prevDir == West) {
                        dir = prevDir;
                    } else {
                        deadend = true;
                    }
                    break;
                case '7':
                    if(prevDir == North) {
                        dir = West;
                        northCount--;
                    } else if(prevDir == East) {
                        dir = South;
                        eastCount--;
                    } else {
                        deadend = true;
                    }
                    break;
                case '|':
                    if(prevDir == North || prevDir == South) {
                        dir = prevDir;
                    } else {
                        deadend = true;
                    }
                    break;
                case 'J':
                    if(prevDir == East) {
                        dir = North;
                        eastCount--;
                    } else if(prevDir == South) {
                        dir = West;
                        westCount--;
                    } else {
                        deadend = true;
                    }
                    break;
                case 'L':
                    if(prevDir == South) {
                        dir = East;
                        southCount--;
                    } else if(prevDir == West) {
                        dir = North;
                        westCount--;
                    } else {
                        deadend = true;
                    }
                    break;
                case 'F':
                    if(prevDir == West) {
                        dir = South;
                        westCount--;
                    } else if(prevDir == North) {
                        dir = East;
                        northCount--;
                    } else {
                        deadend = true;
                    }
                    break;
                case '.':
                    deadend = true;
                    break;
            }
            distance++;
            if(deadend) {
                printf("Deadend reached.\n");
                x_ = startX;
                y_ = startY;
                northCount = 0;
                southCount = 0;
                eastCount = 0;
                westCount = 0;
                current = 'S';
                previous = '.';
                distance = 0;
                deadend = false;
                startDir++;
                dir = startDir;
            }
            if(current == 'S' && distance > 0) {
                end = true;
            }
        }
        printf("N = %d, S = %d, E = %d, W = %d\n", northCount, southCount, eastCount, westCount);
        int deltaX = eastCount - westCount;
        int deltaY = northCount - southCount;
        printf("deltaX = %d, deltaY = %d\n", deltaX, deltaY);
        printf("distance = %d, max path = %d\n", distance, distance / 2);
    } else {
        printf("No file given.\n");
        return 1;
    }
    return 0;
}
