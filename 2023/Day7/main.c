#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

                // printf("%d, %d, %d, %d, %d\n", tempHand.cardCount[0], tempHand.cardCount[1], tempHand.cardCount[2], tempHand.cardCount[3], tempHand.cardCount[4]);

int compare(const void* a, const void* b) {
    int int_a = *((int*)a);
    int int_b = *((int*)b);
    if(int_a == int_b) {
        return 0;
    } else if(int_a > int_b) {
        return -1;
    } else {
        return 1;
    }
}

char deck[13] = {'A', 'K', 'Q', 'J', 'T', '9', '8', '7', '6', '5', '4', '3', '2'};

struct Hand {
    char cards[5];
    uint cardCount[5];
    uint wager;
    int score;
};

struct Hand hands[1000] = {0};
uint handCount = 0;

int main(int argc, char** argv) {
    if(argc > 1) {
        FILE* input = fopen(argv[1], "r");
        if(input == NULL) {
            printf("Could not open file.\n");
            return 1;
        }
        char c = fgetc(input);
        struct Hand tempHand = {{0}, {0}, 0};
        bool preSpace = true;
        int cardsDealt = 0;
        int temp = 0;
        int cardsDealtCount = 0;
        while(c != EOF) {
            if(c == '\n') {
                tempHand.wager = temp;
                for(int i = 0; i < 13; i++) {
                    int t = 0;
                    for(int j = 0; j < 5; j++) {
                        if(tempHand.cards[j] == deck[i]) {
                            t++;
                        }
                    }
                    if(t > 0) {
                        tempHand.cardCount[cardsDealtCount++] = t;
                        t = 0;
                    }
                }
                qsort(tempHand.cardCount, 5, sizeof(int), compare);
                hands[handCount++] = tempHand;
                cardsDealt = 0;
                cardsDealtCount = 0;
                tempHand.wager = 0;
                temp = 0;
                preSpace = true;
                memset(tempHand.cards, 0x00, 5 * sizeof(int));
                memset(tempHand.cardCount, 0x00, 5 * sizeof(int));
            }
            if(c == ' ') {
                preSpace = false;
            }
            if((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z')) {
                if(preSpace) {
                    tempHand.cards[cardsDealt++] = c;
                } else {
                    if(temp > 0) {
                        temp *= 10;
                        temp += atoi(&c);
                    } else {
                        temp = atoi(&c);
                    }
                }
            }
            c = fgetc(input);
        }
        for(int i = 0; i < handCount; i++) {
            struct Hand* h = &hands[i];
            switch(h->cardCount[0]) {
                case 5:
                    h->score = 0;
                    break;
                case 4:
                    h->score = 1;
                    break;
                case 3:
                    if(h->cardCount[1] == 2) {
                        h->score = 2;
                    } else {
                        h->score = 3;
                    }
                    break;
                case 2:
                    if(h->cardCount[1] == 2) {
                        h->score = 4;
                    } else {
                        h->score = 5;
                    }
                    break;
                case 1:
                    h->score = 6;
                    break;
                default:
                    break;
            }
            int* ranking = malloc(handCount * sizeof(int));
            memset(ranking, 0, handCount * sizeof(int)); 
            free(ranking);
        }
    } else {
        printf("No file given.\n");
        return 1;
    }
    return 0;
}
