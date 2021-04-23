#include "funktions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CRDS 52
#include <time.h>

typedef struct Card{
    char data[2];
    struct Card* next;
    struct Card* previous;
} Card;

Card cards[CRDS];

struct Card* c1Head = NULL;
struct Card* c2Head = NULL;
struct Card* c3Head = NULL;
struct Card* c4Head = NULL;
struct Card* c5Head = NULL;
struct Card* c6Head = NULL;
struct Card* c7Head = NULL;

struct Card* c1Tail = NULL;
struct Card* c2Tail = NULL;
struct Card* c3Tail = NULL;
struct Card* c4Tail = NULL;
struct Card* c5Tail = NULL;
struct Card* c6Tail = NULL;
struct Card* c7Tail = NULL;

int createDeck();
void shuffle(Card *deck, int n);

int main() {
    createDeck();
    shuffle(cards,CRDS);
    shuffle(cards,CRDS);

    for (int i = 0; i < CRDS; i++){
        printf("%c",cards[i].data[0]);
        printf("%c\n",cards[i].data[1]);
    }
    return 0;
}

void addCard(int counter,char value, char type)
{
    cards[counter].data[0] = value;
    cards[counter].data[1] = type;
}

void push(Card* headOfLink, Card* tailOfLink, Card* cardToImport)
{
    if(headOfLink != NULL)
    {
        struct Card* newCard = malloc(sizeof(struct Card));
        newCard->next = NULL;
        newCard->previous = tailOfLink;
        newCard->data[0] = &cardToImport[0];
        newCard->data[1] = &cardToImport[1];
        tailOfLink->next = newCard;
        tailOfLink = newCard;
    } else
        {
            headOfLink = malloc(sizeof (struct Card));
            headOfLink->previous = NULL;
            headOfLink->next = NULL;
            headOfLink->data[0] = &cardToImport[0];
            headOfLink->data[1] = &cardToImport[1];
            tailOfLink = headOfLink;
        }
}

int createDeck()
{
    char line[CRDS];
    FILE * fpointer = fopen("..\\Cards.txt","r");

    int counter = 0;
    while (fgets(line, CRDS, fpointer) != NULL) {
        //printf("%c %c\n",line[0], line[1]);
        addCard(counter,line[0],line[1]);
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