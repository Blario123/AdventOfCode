#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
//  1010329272 -- LOW
//  5305296568 -- LOW
void substr(char* buf, char* start, char* end) {
    // Zero the buffer
    memset(buf, 0x0, 1024);
    char* _start = start;
    if(end == NULL) {
        // Find the end from the start
        while(*start != '\0') {
            start++;
        }
        end = start - 1;
    }
    int index = 0;
    while(_start != end) {
        if(*_start != ',') {
            buf[index++] = *_start;
        }
        _start++;
    }
}

bool isPalindromic(int pal) {
    char str[1024];
    sprintf(str, "%d", pal);
    if((strlen(str) % 2) != 0) {
        return false;
    }
    char str_half[512];
    for(int i = 0; i < (strlen(str) / 2); i++) {
        str_half[i] = str[i];
    }
    if(strcmp(str_half, str + (strlen(str) / 2)) == 0) {
        return true;
    }
    return false;
}

unsigned long long invalidId(char* buf) {
    unsigned long long sum = 0;
    int from = 0;
    int to = 0;
    int index = 0;
    char num[strlen(buf)];
    memset(num, 0, strlen(buf));
    while(*buf != '\0') {
        if(*buf == '-') {
            from = atoi(num);
            memset(num, 0, strlen(buf));
            index = 0;
        } else {
            num[index++] = *buf;
        }
        buf++;
    }
    to = atoi(num);
    for(int i = from; i <= to; i++) {
        if(isPalindromic(i)) {
            sum += i;
        }
    }
    printf("from = %d, to = %d\n", from, to);
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
        printf("%llu\n", sum);
        fclose(p_file);
    } else {
        printf("input filename required.\n");
    }
}
