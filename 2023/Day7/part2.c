#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

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

char line[1000];
size_t linePos = 0;

char deck[13] = {'A', 'K', 'Q', 'T', '9', '8', '7', '6', '5', '4', '3', '2', 'J'};

int deckIndex(const char c) {
    for(int i = 0; i < 13; i++) {
        if(deck[i] == c) {
            return i;
        }
    }
    return -1;
}

struct Hand {
    char cards[5];
    uint cardCount[5];
    uint wager;
    int score;
    int jokerCount;
};

struct Hand hands[1100] = {0};
uint handCount = 0;

void parseLine(const char* l, size_t length) {
    struct Hand tempHand = {{0}, {0}, 0x00, 0, 0};
    bool preSpace = true;
    int cardsDealt = 0;
    int temp = 0;
    int cardsDealtCount = 0;
    if(length == 1 || l[0] == '\n') {
        return;
    }
    for(int i = 0; i < length; i++) {
        char c = l[i];
        if(c == '\n') {
        tempHand.wager = temp;
        for(int i = 0; i < 12; i++) {
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
        tempHand.cardCount[0] += tempHand.jokerCount;
        hands[handCount++] = tempHand;
        cardsDealt = 0;
        cardsDealtCount = 0;
        tempHand.wager = 0;
        tempHand.score = 0;
        tempHand.jokerCount = 0;
        temp = 0;
        preSpace = true;
        memset(tempHand.cards, 0, 5);
        memset(tempHand.cardCount, 0, 5);
        }
        if(c == ' ') {
            preSpace = false;
        }
        if(c != ' ' && c != '\n') {
            if(preSpace) {
                if(c == 'J') {
                    tempHand.jokerCount++;
                }
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
    }
}

int main(int argc, char** argv) {
    if(argc > 1) {
        FILE* input = fopen(argv[1], "r");
        if(input == NULL) {
            printf("Could not open file.\n");
            return 1;
        }
        char c = fgetc(input);
        while(c != EOF) {
            line[linePos++] = c;
            if(c == '\n') {
                parseLine(line, linePos);
                memset(line, 0, 1000 * sizeof(char));
                linePos = 0;
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
            }
        }
        bool sorted = false;
        int goes = 0;
        while(!sorted) {
            int correct = 0;
            for(int i = 1; i < handCount; i++) {
                bool toSwap = false;
                struct Hand* h1 = &hands[i-1];
                struct Hand* h2 = &hands[i];
                if(h1->score == h2->score) {
                    for(int j = 0; j < 5; j++) {
                        int card1 = deckIndex(h1->cards[j]);
                        int card2 = deckIndex(h2->cards[j]);
                        if(card1 > card2) {
                            toSwap = true;
                            break;
                        }
                        if(card1 < card2) {
                            break;
                        }
                    }
                }
                if(h1->score > h2->score) {
                    toSwap = true;
                }
                if(toSwap) {
                    struct Hand temp = hands[i-1];
                    hands[i-1] = hands[i];
                    hands[i] = temp;
                } else {
                    correct++;
                }
            }
            if(correct == handCount - 1) {
                sorted = true;
            }
        }
        unsigned long long total = 0;
        for(int i = 0; i < handCount; i++) {
            total += ((handCount - i) * hands[i].wager);
        }
        printf("Total score = %llu\n", total);
    } else {
        printf("No file given.\n");
        return 1;
    }
    return 0;
}
