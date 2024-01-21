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

typedef struct {
    int x;
    int y;
} coord;

void printfMap(void) {
    for(int i = 0; i < y; i++) {
        printf("%s\n", map[i]);
    }
}

void parseLine(const char* l, size_t length) {
    // If the length of the line is 1, or the first character is a newline, return immediately as nothing is to be done.
    if(length == 1 || l[0] == '\n') {
        return;
    }
    x = length + 1;
    xMax = length + 2;
    map[y] = malloc(xMax * sizeof(char));
    memset(map[y], 0, xMax);
    memcpy(map[y], l, length);
    memmove(&map[y][1], &map[y][0], length * sizeof(char));
    map[y][0] = '.';
    map[y][x - 1] = '.';
    map[y][xMax] = '\0';
    printf("%s\n", map[y]);
    if(y == yMax - 1) {
        yMax += 2;
        map = realloc(map, yMax * sizeof(char*));
    }
    char* sPos = strstr(map[y], "S");
    if(sPos != NULL) {
        startY = y;
        startX = sPos - map[y];
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
        coord* borderPoints = malloc(16384 * sizeof(coord));
        int borderPointCount = 0;
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
            borderPoints[borderPointCount++] = (coord){x_, y_};
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
                memset(borderPoints, 0, 16384 * sizeof(coord));
                borderPointCount = 0;
            }
            if(current == 'S' && distance > 0) {
                end = true;
            }
        }
        for(int i = 0; i < borderPointCount; i++) {
            coord* c = &borderPoints[i];
            map[c->y][c->x] = 'B';
        }
        free(borderPoints);
        for(int i = 0; i < y; i++) {
            map[i][0] = 'X';
            map[i][x] = 'X';
            for(int j = 0; j < x; j++) {
                if(map[i][j] != 'B') {
                    map[i][j] = '.';
                    if(i == 0 || i == (y - 1)) {
                        map[i][j] = 'X';
                    }
                }
            }
        }
        bool finished = false;
        map[0][0] = 'X';
        while(!finished) {
            int changes = 0;
            for(int i = 0; i < y; i++) {
                int u, d;
                u = i == 0 ? 0 : -1;
                d = i == (y - 1) ? 0 : 1;
                for(int j = 0; j < x; j++) {
                    int l, r;
                    l = j == 0 ? 0 : -1;
                    r = j == (x - 1) ? 0 : 1;
                    // Ignore if the current char is 'X'. Check around any that are a '.'
                    if(map[i][j] == '.') {
                        bool isConnected = false;
                        for(int ii = u; ii < (d + 1); ii++) {
                            for(int jj = l; jj < (r + 1); jj++) {
                                if(map[i + ii][j + jj] == 'X') {
                                    isConnected = true;
                                    break;
                                }
                            }
                            if(isConnected) {
                                break;
                            }
                        }
                        if(isConnected) {
                            changes++;
                            map[i][j] = 'X';
                        }
                    }
                }
            }
            if(changes == 0) {
                finished = true;
                break;
            }
        }
        printfMap();
        int count = 0;
        for(int i = 0; i < y; i++) {
            for(int j = 0; j < x; j++) {
                if(map[i][j] == '.') {
                    count++;
                }
            }
        }
        // Answer lies between 211 and 411
        printf("Count = %d\n", count);
    } else {
        printf("No file given.\n");
        return 1;
    }
    return 0;
}
