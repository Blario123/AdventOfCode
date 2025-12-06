#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

int count_passes(int pos, int prev) {
	int a = 0;
	while(prev != pos) {
		if(pos < prev) {
			prev--;
		}
		if(pos > prev) {
			prev++;
		}
		if((prev % 100) == 0) {
			a++;
		}
	}
	return a;
}

int main(int argc, char** argv) {
    if(argc > 1) {
        int sum = 0;
        int sum_pass = 0;
        int dial = 50;
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
			int dial_old = dial;
            dial += s;
			sum_pass += count_passes(dial, dial_old);
            // Normalise the number to 0-100
            if(dial < 0) {
                dial = 100 + dial;
            }
			dial %= 100;
            if(dial == 0) {
                sum++;
            }
        }
        fclose(p_file);
        printf("Password = %d\n", sum);
        printf("Password = %d\n", sum_pass);
    } else {
        printf("input filename required.\n");
    }
}
