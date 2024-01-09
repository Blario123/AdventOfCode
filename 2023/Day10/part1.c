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
        while(!end) {
            deadend = false;
            prevDir = dir;
            switch(dir) {
                case North:
                    if(y_ == 0) {
                        deadend = true;
                        break;
                    }
                    --y_;
                    break;
                case East:
                    if(x_ == xMax) {
                        deadend = true;
                        break;
                    }
                    ++x_;
                    break;
                case South:
                    if(y_ == yMax) {
                        deadend = true;
                        break;
                    }
                    ++y_;
                    break;
                case West:
                    if(x_ == 0) {
                        deadend = true;
                        break;
                    }
                    --x_;
                    break;
            }
            if(prevDir > 4) {
                return -1;
            }
            current = map[y_][x_];
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
                    } else if(prevDir == East) {
                        dir = South;
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
                    } else if(prevDir == South) {
                        dir = West;
                    } else {
                        deadend = true;
                    }
                    break;
                case 'L':
                    if(prevDir == South) {
                        dir = East;
                    } else if(prevDir == West) {
                        dir = North;
                    } else {
                        deadend = true;
                    }
                    break;
                case 'F':
                    if(prevDir == West) {
                        dir = South;
                    } else if(prevDir == North) {
                        dir = East;
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
                x_ = startX;
                y_ = startY;
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
        printf("Max path length is %d\n", distance / 2);
    } else {
        printf("No file given.\n");
        return 1;
    }
    return 0;
}
