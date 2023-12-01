#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

int count = 0;
char line[1000] = {0};

int main(int argc, char** argv) {
    if(argc > 1) {
        // Load file handle to input as given by command-line argument
        FILE* input = fopen(argv[1], "r");
        // Check if the file loaded properly, if not, exit
        if(input == NULL) {
            printf("Could not open file.\n");
            return 1;
        }
        // Get the current char of the input
        char c = fgetc(input);
        // Set line position to 0 to set the line array to the relevant character.
        size_t pos = 0;
        // If the character is equal to EOF (-1) no more characters will be read.
        while(c != EOF) {
            // Set the position of the character in the input to the same in line.
            line[pos++] = c;
            // If c is a newline, parse the input and then clear the line array.
            if(c == '\n') {
                // Shift pos back 1 to represent the current data.
                pos--;
                // Parse the contents of line.
                int value = 0;
                for(size_t i = 0; i < pos; i++) {
                    char a = line[i];
                    if(a >= '0' && a <= '9') {
                        value += (atoi(&a) * 10); 
                        break;
                    }
                }
                for(size_t i = pos - 1; i >= 0; i--) {
                    char a = line[i];
                    if(a >= '0' && a <= '9') {
                        value += atoi(&a); 
                        break;
                    }
                }
                count += value;
                // Set the line position back to 0;
                pos = 0;

                memset(&line, 0x00, 1000);    
            }
            // Read the next character into c.
            c = fgetc(input);
        }
        printf("%d\n", count);
    } else {
        printf("No file given.\n");
        return 1;
    }
    return 0;
}
