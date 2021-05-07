#include "funktions.h"
#include <stdio.h>
#include <stdlib.h>
#define CRDS 52
#include <time.h>

struct Card{
    char data[3];
    int visible;
    struct Card* next;
    struct Card* previous;
};

struct Bulk{
    char desc[10];
    struct Bulk* next;
    struct Bulk* previous;
    struct Card* cHead;
    struct Card* cTail;
};

// Node operations
void pushBulk(char name[]);
void popBulk();
int pushCard(struct Bulk* toBulk, char value);
int popCard(struct Bulk* fromBulk);
void initGame();

void moveCard(struct Bulk* fromBulk, struct Bulk* toBulk);
void moveMultipleCards(struct Bulk* fromBulk, struct Bulk* toBulk);

//Logic
int loadDeck();
void shuffle(struct Card *deck, int n);
void createGameColumns();
void printColumn(struct Card *head);
void deckToColumns();

//View
void display();
void console();

/**
 * Global variables
 */
struct Bulk* bulkHead;
struct Bulk* bulkTail;

int main() {

    return 0;
}

void initGame()
{
    bulkHead = NULL;
    bulkTail = NULL;
    for(int i = 0; i < 7; i++)
    {
        if(bulkHead != NULL) {
            struct Bulk *newBulk = malloc(sizeof(struct Bulk));
            newBulk->next = NULL;
            newBulk->previous = bulkTail;
            newBulk->cHead = NULL;
            newBulk->cTail = NULL;
            newBulk->desc[5] = "what\0";
            bulkTail->next = newBulk;
            bulkTail = newBulk;
        } else {
            bulkHead = malloc(sizeof(struct Bulk));
            bulkHead->previous = NULL;
            bulkHead->next = NULL;
            bulkHead->cTail = NULL;
            bulkHead->cHead = NULL;
            bulkTail = bulkHead;
        }

    }
}

int pushCard(struct Bulk* bulk, char value)
{
    if(bulk->cHead != NULL)
    {
        struct Card* newCard = malloc(sizeof(struct Card));
        newCard->next = NULL;
        newCard->previous = bulk->cTail;
        newCard->data = value;
    }
}

int loadDeck()
{
    char line[CRDS];
    FILE * fpointer = fopen("..\\Cards.txt","r");

    int counter = 0;
    while (fgets(line, CRDS, fpointer) != NULL) {
        //printf("%c %c\n",line[0], line[1]);

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

void createGameColumns()
{
    for(int i = 0; i < CRDS; i++)
    {
        if(i < 7)
        {
            if(i == 0) pushCard(&c1Head, &c1Tail, &cards[i]);
            if(i == 1) pushCard(&c2Head, &c2Tail, &cards[i]);
            if(i == 2) pushCard(&c3Head, &c3Tail, &cards[i]);
            if(i == 3) pushCard(&c4Head, &c4Tail, &cards[i]);
            if(i == 4) pushCard(&c5Head, &c5Tail, &cards[i]);
            if(i == 5) pushCard(&c6Head, &c6Tail, &cards[i]);
            if(i == 6) pushCard(&c7Head, &c7Tail, &cards[i]);
        } else if(i < 12)
        {
            pushCard(&c2Head, &c2Tail, &cards[i]);
        } else if(i < 18)
        {
            pushCard(&c3Head, &c3Tail, &cards[i]);
        } else if(i < 25)
        {
            pushCard(&c4Head, &c4Tail, &cards[i]);
        } else if(i < 33)
        {
            pushCard(&c5Head, &c5Tail, &cards[i]);
        } else if (i < 42)
        {
            pushCard(&c6Head, &c6Tail, &cards[i]);
        } else if (i < 52)
        {
            pushCard(&c7Head, &c7Tail, &cards[i]);
        }
    }
}

void deckToColumns()
{
    int i = 0;

    while(i != CRDS) {

        pushCard(&c1Head, &c1Tail, &cards[i]);
        i++;
        if(i != CRDS) pushCard(&c2Head, &c2Tail, &cards[i]);
        else break;
        i++;
        if(i != CRDS) pushCard(&c3Head, &c3Tail, &cards[i]);
        else break;
        i++;
        if(i != CRDS) pushCard(&c4Head, &c4Tail, &cards[i]);
        else break;
        i++;
        if(i != CRDS) pushCard(&c5Head, &c5Tail, &cards[i]);
        else break;
        i++;
        if(i != CRDS) pushCard(&c6Head, &c6Tail, &cards[i]);
        else break;
        i++;
        if(i != CRDS) pushCard(&c7Head, &c7Tail, &cards[i]);
        else break;
        i++;
    }

}


void makeColumnInvisible(Card* columnHead, int numOfInvisible)
{
    Card* card = columnHead;

    for(int i = 0; i < numOfInvisible; i++)
    {
        card->visible = 0;
        if(card->next != NULL) card = card->next;
        else break;
    }
}

void printColumn(Card *head)
{
    printf("%c", head->data[0]);
    printf("%c\n", head->data[1]);

    while(head->next != NULL){
        head = head->next;
        printf("%c", head->data[0]);
        printf("%c\n", head->data[1]);
    }

}

void quitProgram()
{
    exit(1);
}

void startPhase()
{
    loadDeck();
}


void printColumnRow(Card* column, int row)
{
    Card* columnT = column;

    for(int i = 0 ; i < row; i++ )
    {

        columnT = columnT->next;

    }

    if(columnT == NULL) {
        printf("%s\t", columnT->data);
    } else printf()

}

void display()
{
    printf("c1\tc2\tc3\tc4\tc5\tc6\tc7\n\n");
    for(int i = 0; i < 13; i++) {
        printColumnRow(c1Head, i);
        printColumnRow(c2Head, i);
        printColumnRow(c3Head, i);
        printColumnRow(c4Head, i);
        printColumnRow(c5Head, i);
        printColumnRow(c6Head, i);
        printColumnRow(c7Head, i);
        printf("\n");
    }


}

