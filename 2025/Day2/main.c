#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

void substr(char* buf, char* start, char* end) {
    // Zero the buffer
    memset(buf, 0x0, 1024);
    // Create another string pointer to the same start point
    char* _start = start;
    // If the strchr value is NULL, fin the last digit of the original buffer
    if(end == NULL) {
        while(*start != '\0') {
            start++;
        }
        // Decrement to remove the null-end
        end = start - 1;
    }
    int index = 0;
    // Form the sub-string buffer from the start to the end point
    while(_start != end) {
        if(*_start != ',') {
            buf[index++] = *_start;
        }
        _start++;
    }
}

bool isRepeat(unsigned long long pal) {
    // Create a local buffer
    char str[1024];
    // Convert the ull input to c-string
    sprintf(str, "%llu", pal);
    // If the string is an odd length, ignore as cannot be invalid
    if((strlen(str) % 2) != 0) {
        return false;
    }
    // Create a second local buffer, half the size of the first ull buffer.
    char str_half[512];
    // Zero to ensure no erroneous characters
    memset(str_half, 0, 512);
    // Take the first half of the string
    for(int i = 0; i < (strlen(str) / 2); i++) {
        str_half[i] = str[i];
    }
    // Compare the first half, to the second
    // Add the halfway point to the pointer to start the second half without copying
    if(strcmp(str_half, str + (strlen(str) / 2)) == 0) {
        return true;
    }
    return false;
}

unsigned long long invalidId(char* buf) {
    // To correctly handle the input, all values must be ull.
    unsigned long long sum = 0;
    unsigned long long from = 0;
    unsigned long long to = 0;
    int index = 0;
    char num[strlen(buf)];
    memset(num, 0, strlen(buf));
    while(*buf != '\0') {
        if(*buf == '-') {
            from = atoll(num);
            memset(num, 0, strlen(buf));
            index = 0;
        } else {
            num[index++] = *buf;
        }
        buf++;
    }
    to = atoll(num);
    // Iterate within the range of the values
    for(unsigned long long i = from; i <= to; i++) {
        if(isRepeat(i)) {
            sum += i;
        }
    }
    return sum;
}

int main(int argc, char** argv) {
    if(argc > 1) {
        unsigned long long sum = 0;
        const char* filename = argv[1];
        FILE *p_file = fopen(filename, "r");
        size_t len = 1024;
        char buffer[len];
		char* p_buf = buffer;
        while(fgets(buffer, len, p_file)) {
			// Get substring of buffer
			// Find next occurance of ','
			while(p_buf != NULL) {
                char* p_buf_old = p_buf;
				p_buf = strchr(p_buf + 1, ',');
                char buf[len];
                substr(buf, p_buf_old, p_buf);
                sum += invalidId(buf);
			}
        }
        printf("Sum of invalid IDs = %llu\n", sum);
        fclose(p_file);
    } else {
        printf("input filename required.\n");
    }
}
