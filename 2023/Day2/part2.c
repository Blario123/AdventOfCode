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
                            switch(line[i + 1]) {
                                case 'r':
                                    if(temp > r.r) {
                                        r.r = temp;
                                    }
                                    break;
                                case 'g':
                                    if(temp > r.g) {
                                        r.g = temp;
                                    }
                                    break;
                                case 'b':
                                    if(temp > r.b) {
                                        r.b = temp;
                                    }
                                    break;
                            }
                            temp = 0;
                        }
                    }
                }
                int t = r.r * r.g * r.b;
                count += t;
                pos = 0;
                memset(&line, 0x00, 1000);
            }
            c = fgetc(input);
        }
        printf("Sum of the power of the sets: %d\n", count);
    } else {
        printf("No file given.\n");
        return 1;
    }
    return 0;
}
