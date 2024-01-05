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

int deckIndex(const char c) {
    for(int i = 0; i < 13; i++) {
        if(deck[i] == c) {
            // printf("Found %c at index %d\n", c, i);
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
};

struct Hand hands[1100] = {0};
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
                // If the score is the same, determine on first, if not second, if not third... card.
                // printf("Hand %d's score is %d. First card is %c\n", i-1, h1->score, h1->cards[0]);
                // printf("Hand %d's score is %d. First card is %c\n", i, h2->score, h2->cards[0]);
                if(h1->score == h2->score) {
                    for(int j = 0; j < 5; j++) {
                        // printf("Score matches. Checking card[%d]\t%c,%c\n", i, h1->cards[j], h2->cards[j]);
                        if(deckIndex(h1->cards[j]) > deckIndex(h2->cards[j])) {
                            toSwap = true;
                            break;
                        } else if(deckIndex(h1->cards[j]) < deckIndex(h2->cards[j])) {
                            break;
                        }
                        // printf("Cards matched. Moving onto card number %d\n", j + 1);
                    }
                }
                if(h1->score > h2->score) {
                    toSwap = true;
                }
                if(toSwap) {
                    // printf("Swapping %d and %d\n", i-1, i);
                    struct Hand temp = hands[i-1];
                    hands[i-1] = hands[i];
                    hands[i] = temp;
                } else {
                    correct++;
                }
            }
            printf("\r%.3f%% Correct", (float) 100 * correct/(handCount-2));
            if(correct == handCount - 2) {
                sorted = true;
            }
        }
        unsigned long long total = 0;
        for(int i = 0; i < handCount; i++) {
            printf("%d * %d\tWager multiplied = %llu\tTotal = %llu\n", handCount - i, hands[i].wager, (unsigned long long) ((handCount - i) * hands[i].wager), total);
            total += ((handCount - i) * hands[i].wager);
        }
        printf("Total score = %llu\n", total);
    } else {
        printf("No file given.\n");
        return 1;
    }
    return 0;
}
