#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    if(argc > 1) {
        int sum = 0;
        int sum_point = 0;
        int dial = 50;
        int dial_old = 50;
        const char* filename = argv[1];
        FILE *p_file = fopen(filename, "r");
        size_t len = 255;
        char buffer[len];
        while(fgets(buffer, len, p_file)) {
            int s;
            switch(buffer[0]) {
                case 'L':
                    s = -1 * atoi(&buffer[0] + 1);
                    break;
                case 'R':
                    s = atoi(&buffer[0] + 1);
                    break;
                default:
                    printf("Unknown option\n");
            }
            dial_old = dial;
            dial += s;
            // Normalise the number to 0-100
            bool performed = false;
            if(dial < 0) {
                dial = 100 + dial;
                printf("passed 0\n");
                if(dial_old != 0) {
                    sum_point++;
                }
                performed = true;
            }
            if(dial > 99) {
                printf("passed 0\n");
                dial = dial % 100;
                if(dial_old != 0) {
                    sum_point++;
                }
                performed = true;
            }
            if(dial == 0) {
                sum++;
                if(!performed) {
                    sum_point++;
                }
            }
            printf("Moving %d, dial is now %d\n", s, dial);
        }
        fclose(p_file);
        printf("Password = %d\n", sum);
        printf("Password including points = %d\n", sum_point);
    } else {
        printf("input filename required.\n");
    }
}
