#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

char line[1000];

struct Round {
    int r;
    int g;
    int b;
};

int count;

int main(int argc, char** argv) {
    if(argc > 1) {
        FILE* input = fopen(argv[1], "r");
        if(input == NULL) {
            printf("Could not open file.\n");
            return 1;
        }
        size_t pos;
        int game = 0;
        char c = fgetc(input);
        while(c != EOF) {
            line[pos++] = c;
            if(c == '\n') {
                game++;
                printf("game: %d\n", game);
                pos--;
                char* colon = strstr(line, ":");
                struct Round r = {0, 0, 0};
                int temp;
                bool isValid = true;
                for(int i = colon - line + 2; i < pos; i++) {
                    char cur = line[i];
                    if(cur >= '0' && cur <= '9') {
                        if(temp > 0) {
                            temp *= 10;
                            temp += atoi(&cur);
                        } else {
                            temp = atoi(&cur); 
                        }
                    }
                    if(cur == ' ') {
                        if(temp > 0) {
                            printf("%c\n", line[i + 1]);
                            switch(line[i + 1]) {
                                case 'r':
                                    r.r = temp;
                                    break;
                                case 'g':
                                    r.g = temp;
                                    break;
                                case 'b':
                                    r.b = temp;
                                    break;
                            }
                            temp = 0;
                        }
                    }
                    if(cur == ';' || i == pos - 1) {
                        printf("r: %d, g: %d, b: %d\n", r.r, r.g, r.b);
                        if(r.r > 12 || r.g > 13 || r.b > 14) {
                            printf("invalid\n");
                            isValid = false;
                        }
                    }
                }
                if(isValid) {
                    printf("Game %d is valid.\n", game);
                    count += game;
                }
                printf("\n");
                pos = 0;
                memset(&line, 0x00, 1000);
            }
            c = fgetc(input);
        }
        printf("%d\n", count);
    } else {
        printf("No file given.\n");
        return 1;
    }
    return 0;
}
