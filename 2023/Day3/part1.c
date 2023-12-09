#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>



char lines[1000][1000] = {{0}};
int length;

struct Number {
    int n;
    int x;
    int y;
    int length;
};

struct Number numbers[3000] = {0};
int numberCount = 0;

int main(int argc, char** argv) {
    if(argc > 1) {
        FILE* input = fopen(argv[1], "r");
        if(input == NULL) {
            printf("Could not open file.\n");
            return 1;
        }
        char c = fgetc(input);
        int x = 0;
        int y = 0;
        int temp = 0;
        struct Number tempNum = {0, 0, 0, 0};
        while(c != EOF) {
            lines[y][x++] = c; 
            if(c == '\n') {
                for(int i = 0; i < x; i++) {
                    char cur = lines[y][i];
                    if(cur >= '0' && cur <= '9') {
                        if(temp > 0) {
                            temp *= 10;
                            temp += atoi(&cur);
                        } else {
                            tempNum.x = i;
                            tempNum.y = y;
                            temp = atoi(&cur);
                        }
                    } else {
                        if(temp > 0) {
                            tempNum.n = temp;
                            tempNum.length = i - tempNum.x;
                            numbers[numberCount++] = tempNum;
                            tempNum.n = 0;
                            tempNum.x = 0;
                            tempNum.y = 0;
                            tempNum.length = 0;
                            temp = 0;
                        }
                    }
                }
                x = 0;
                y++;
            }
            c = fgetc(input);
        }
        int count = 0;
        for(int i = 0; i < numberCount; i++) {
            int lower = 1;
            int upper = 1;
            int left = 1;
            int right = 1;
            struct Number* n = &numbers[i];
            if(n->x == 0) {
                left = 0;
            }
            if(n->x == strlen(lines[0])) {
                right = 0;
            }
            if(n->y == 0) {
                upper = 0;
            }
            if(n->y == y) {
                lower = 0;
            }
            bool found = false;
            for(int j = n->x - left; j < n->x + n->length + right; j++) {
                for(int k = n->y - upper; k < n->y + lower + 1; k++) {
                    char c = lines[k][j];
                    if(!found && c != 0x00 && c != '\n' && c != '.' && (c < '0' || c > '9')) {
                        count += n->n;
                        found = true;
                    }
                }
            }
        }
        printf("Part number sum = %d\n", count);
    } else {
        printf("No file given.\n");
        return 1;
    }
    return 0;
}
