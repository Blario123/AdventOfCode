#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

char line[1000];

struct Card {
    int n;
    int winners[10];
    int numbers[25];
    int numWin;
    int tries;
};

struct Card cards[194];

int main(int argc, char** argv) {
    if(argc > 1) {
        FILE* input = fopen(argv[1], "r");
        if(input == NULL) {
            printf("Could not open file.\n");
            return 1;
        }
        char c = fgetc(input);
        size_t pos = 0;
        int cardCount = 0;
        while(c != EOF) {
            line[pos++] = c;
            if(c == '\n') {
                pos--;
                struct Card tempCard = {cardCount++ + 1, {0}, {0}, 0, 1};
                int temp = 0;
                int winnersCount = 0;
                int numbersCount = 0;
                for(size_t i = strstr(line, ":") - line + 2; i < strstr(line, "|") - line; i++) {
                    char cur = line[i];
                    if(cur >= '0' && cur <= '9') {
                       if(temp > 0) {
                           temp *= 10;
                           temp += atoi(&cur);
                       } else {
                           temp = atoi(&cur);
                       }
                    }
                    if(cur == ' ' && temp > 0) {
                        tempCard.winners[winnersCount++] = temp;
                        temp = 0;
                    }
                }
                for(size_t i = strstr(line, "|") - line; i < pos; i++) {
                    char cur = line[i];
                    if(cur >= '0' && cur <= '9') {
                       if(temp > 0) {
                           temp *= 10;
                           temp += atoi(&cur);
                       } else {
                           temp = atoi(&cur);
                        }
                    }
                    if((cur == ' ' || i == pos - 1) && temp > 0) {
                        tempCard.numbers[numbersCount++] = temp;
                        temp = 0;
                    }
                }
                for(int i = 0; i < winnersCount; i++) {
                    for(int j = 0; j < numbersCount; j++) {
                        if(tempCard.winners[i] == tempCard.numbers[j]) {
                            tempCard.numWin++;
                        }
                    }
                }
                cards[cardCount - 1] = tempCard;
                pos = 0;
                memset(&line, 0x00, 1000);
            }
            c = fgetc(input);
        }
        int count = 0;
        for(int i = 0; i < cardCount; i++) {
            for(int try = 0; try < cards[i].tries; try++) {
                for(int j = 1; j <= cards[i].numWin; j++) {
                    //printf("Adding a try to card %d\n",  
                    if((i + j) < cardCount) {
                        cards[i + j].tries++;
                    }
                }
            }
            count += cards[i].tries;
        }
        printf("Total scratchcards: %d\n", count);
    } else {
        printf("No file given.\n");
        return 1;
    }
    return 0;
}
