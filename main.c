#include "funktions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CRDS 52
#include <time.h>


int counter = 0;

typedef struct Card{
    char data[2];
    struct Card* next;
} Card;

Card cards[CRDS];

Card c1;
Card c2;
Card c3;
Card c4;
Card c5;
Card c6;
Card c7;

int createDeck();
void shuffle(Card *deck, int n);

int main() {
    createDeck();
    shuffle(cards,CRDS);
    shuffle(cards,CRDS);

    int i;
    for (i = 0; i < CRDS; i++){
        printf("%c",cards[i].data[0]);
        printf("%c\n",cards[i].data[1]);
    }
    return 0;
}

void addCard(char value, char type)
{
    cards[counter].data[0] = value;
    cards[counter].data[1] = type;
}

int createDeck()
{
    char line[CRDS];
    FILE * fpointer = fopen("..\\Cards.txt","r");

    while (fgets(line, CRDS, fpointer) != NULL) {
        addCard(line[0],line[1]);
        counter++;
    }

    fclose(fpointer);
    return 0;
}

void swap( struct Card *a, struct Card *b){
    struct Card temp = *a;
    *a = *b;
    *b = temp;
}

void shuffle(Card *deck, int n) //Shuffles array
{
    srand(time(NULL));

    for(int i = n-1; i > 0; i--)
    {
        int j = rand() % (i+1);
        swap(&deck[i], &deck[j]);
    }
}

void createColumns() {
    c1 = cards[0];
    c2 = cards[1];
    c3 = cards[2];
    c4 = cards[3];
    c5 = cards[4];
    c6 = cards[5];
    c7 = cards[6];
    
    int i = 7;
    while(i < CRDS)
    {

    }
    
}