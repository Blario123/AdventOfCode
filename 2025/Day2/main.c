#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

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

uint invalidId(char* buf) {
    return 0;
}

int main(int argc, char** argv) {
    if(argc > 1) {
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
                printf("%s\n", buf);
			}
        }
        fclose(p_file);
    } else {
        printf("input filename required.\n");
    }
}
