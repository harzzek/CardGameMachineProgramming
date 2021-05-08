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

struct Card deck[CRDS];

// Node operations
void pushBulk(char name[]);
void popBulk();
int pushCard(struct Bulk* toBulk, char value[]);
int popCard(struct Bulk* fromBulk);
void initGame();

void moveCard(struct Bulk* fromBulk, struct Bulk* toBulk);
void moveMultipleCards(struct Bulk* fromBulk, struct Bulk* toBulk);

//Logic
int loadDeck();
void shuffle(struct Card *deck, int n);
void loadGameColumns();
void printColumn(struct Bulk *head);
void loadDefaultColumns();
void loadInvisibility();
int getBulkLenght();
int getLenghtOfColumn(struct Bulk* bulk);
int gameOver();
void startPhase();

//View
void display();
char* startConsole(char *lastInput, int messageBoo);

/**
 * Global variables
 */
struct Bulk* bulkHead;
struct Bulk* bulkTail;

struct Bulk* foundationhead;
struct Bulk* foundationTail;

int main() {
    /*
    initGame();
    loadDeck();
    shuffle(deck,CRDS);
    loadGameColumns();
    loadInvisibility();
    display();
    */
    startPhase();



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

    foundationhead = NULL;
    foundationTail = NULL;
    for(int i = 0; i < 4; i++)
    {
        if(foundationhead != NULL) {
            struct Bulk *newBulk = malloc(sizeof(struct Bulk));
            newBulk->next = NULL;
            newBulk->previous = foundationTail;
            newBulk->cHead = NULL;
            newBulk->cTail = NULL;
            newBulk->desc[5] = "what\0";
            foundationTail->next = newBulk;
            foundationTail = newBulk;
        } else {
            foundationhead = malloc(sizeof(struct Bulk));
            foundationhead->previous = NULL;
            foundationhead->next = NULL;
            foundationhead->cTail = NULL;
            foundationhead->cHead = NULL;
            foundationTail = foundationhead;
        }
    }
}

int pushCard(struct Bulk* bulk, char value[])
{
    int boo = 0;
    if(bulk->cHead != NULL)
    {
        struct Card* newCard = malloc(sizeof(struct Card));
        newCard->next = NULL;
        newCard->previous = bulk->cTail;
        newCard->visible = 1;
        newCard->data[0] = value[0];
        newCard->data[1] = value[1];
        newCard->data[2] = '\0';
        bulk->cTail->next = newCard;
        bulk->cTail = newCard;
        boo = 1;
    } else {
        bulk->cHead = malloc(sizeof(struct Card));
        bulk->cHead->previous = NULL;
        bulk->cHead->next = NULL;
        bulk->cHead->visible = 1;
        bulk->cHead->data[0] = value[0];
        bulk->cHead->data[1] = value[1];
        bulk->cHead->data[2] = '\0';
        bulk->cTail = bulk->cHead;
        boo = 1;
    }
    return boo;
}

void addCard(int counter,char value, char type)
{
    deck[counter].data[0] = value;
    deck[counter].data[1] = type;
}

int loadDeck()
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

void shuffle(struct Card *deck, int n) //Shuffles array
{
    srand(time(NULL));

    for(int i = n-1; i > 0; i--)
    {
        int j = rand() % (i+1);
        swap(&deck[i], &deck[j]);
    }
}

void makeInvisible(struct Bulk* bulk,int numOfInvisible)
{
    struct Card* card = bulk->cHead;

    for(int i = 0; i < numOfInvisible; i++)
    {
        card->visible = 0;
        if(card->next != NULL) card = card->next;
        else break;
    }
}

void loadInvisibility()
{
    int bulkLength = getBulkLenght();
    struct Bulk* bulk = bulkHead->next;

    for(int i = 1; i < bulkLength; i++)
    {
        makeInvisible(bulk, i);
        bulk = bulk->next;
    }
}

void makeAllInvisible()
{
    struct Bulk* bulk = bulkHead;
    struct Card* card;

    for (int i = 0; i < getBulkLenght(); ++i) {
        card = bulk->cHead;
        for (int j = 0; j < getLenghtOfColumn(bulk); ++j) {
            card->visible = 0;
            card = card->next;
        }
        bulk = bulk->next;
    }
}

void makeAllVisible()
{
    struct Bulk* bulk = bulkHead;
    struct Card* card;

    for (int i = 0; i < getBulkLenght(); ++i) {
        card = bulk->cHead;
        for (int j = 0; j < getLenghtOfColumn(bulk); ++j) {
            card->visible = 1;
            card = card->next;
        }
        bulk = bulk->next;
    }
}

void loadGameColumns()
{
    struct Bulk* bulk = bulkHead;

    pushCard(bulk,deck[0].data);

    bulk = bulk->next;

    for(int i = 1; i < CRDS; i++)
    {
        if(i < 7) pushCard(bulk,deck[i].data);
        else if(i < 14) pushCard(bulk->next, deck[i].data);
        else if(i < 22) pushCard(bulk->next->next, deck[i].data);
        else if(i < 31) pushCard(bulk->next->next->next, deck[i].data);
        else if(i < 41) pushCard(bulk->next->next->next->next, deck[i].data);
        else if(i < 52) pushCard(bulk->next->next->next->next->next, deck[i].data);
    }
}

void loadDefaultColumns()
{
    initGame();
    int i = 0;
    struct Bulk* bulk = bulkHead;

    while(i != CRDS) {

        pushCard(bulk, deck[i].data);
        i++;
        if(i != CRDS) pushCard(bulk->next, deck[i].data);
        else break;
        i++;
        if(i != CRDS) pushCard(bulk->next->next, deck[i].data);
        else break;
        i++;
        if(i != CRDS) pushCard(bulk->next->next->next, deck[i].data);
        else break;
        i++;
        if(i != CRDS) pushCard(bulk->next->next->next->next, deck[i].data);
        else break;
        i++;
        if(i != CRDS) pushCard(bulk->next->next->next->next->next, deck[i].data);
        else break;
        i++;
        if(i != CRDS) pushCard(bulk->next->next->next->next->next->next, deck[i].data);
        else break;
        i++;
    }
}

void printColumn(struct Bulk *head)
{
    struct Card *card = head->cHead;

    printf("%c", card->data[0]);
    printf("%c\n",card->data[1]);


    while(card->next != NULL){
        card = card->next;
        printf("%c", card->data[0]);
        printf("%c\n", card->data[1]);
    }

}

void quitProgram()
{
    exit(1);
}

void startPhase()
{
    int startupPhase = 0;
    char* input;
    int errorMsg;
    initGame();

    while(startupPhase == 0)
    {

        display();

        input = startConsole(input,errorMsg);

        if(input[0] == 'L' && input[1] == 'D')
        {
            loadDeck();
            loadDefaultColumns();
            makeAllInvisible();
            errorMsg = 1;
        } else if(input[0] == 'S' && input[1] == 'W')
        {
            makeAllVisible();
            errorMsg = 1;
        } else if(input[0] == 'S' && input[1] == 'R')
        {
            shuffle(deck,CRDS);
            loadDefaultColumns();
            errorMsg = 1;
        } else if(input[0] == 'Q' && input[0] == 'Q')
        {
            quitProgram();
            errorMsg = 1;
        } else if(input[0] =='P')
        {
            startupPhase = 1;
            errorMsg = 1;
        } else errorMsg = 0;
    }
}


void printColumnRow(struct Bulk* bulk, int row)
{
    struct Card* column = bulk->cHead;
    int nullBoo = 1;

    for (int i = 0; i < row; ++i) {
        if(column->next != NULL) {
            column = column->next;

        } else {
            nullBoo = 0;
            break;
        }
    }
    int isVisible = column->visible;

    if(nullBoo == 1) {
        if(isVisible == 1)
        printf("%s\t", column->data);
        else printf("[]\t");
    } else printf(" \t");
}

int getLenghtOfColumn(struct Bulk* bulk){
    int count = 0;
    struct Card* current = bulk->cHead;
    while (current != NULL)
    {
        count++;
        current = current->next;
    }

    return count;
}

int getBulkLenght(){
    int count = 0;
    struct Bulk* bulk = bulkHead;
    while (bulk != NULL)
    {
        count++;
        bulk = bulk->next;
    }

    return count;
}

char* startConsole(char* lastInput, int messageBoo)
{
    char input[2];

    printf("LAST Command: %s\n", lastInput);
    if(messageBoo == 1)
        printf("Message: OK\n");
    else if(messageBoo == 0)
        printf("Message: Error\n");
    else printf("Message: \n");
    printf("INPUT >\n");
    scanf("%s",input);

    char *str = malloc(3);

    str[0] = input[0];
    str[1] = input[1];
    str[2] = '\0';

    return str;
}

char* playConsole()
{

}

void display()
{
    printf("c1\tc2\tc3\tc4\tc5\tc6\tc7\n\n");

    struct Bulk* bulk = bulkHead;
    int displaySize = 0;
    int numOfColumns = getBulkLenght();

    for(int i = 0; i < numOfColumns; i++)
    {
        int size = getLenghtOfColumn(bulk);
        if(displaySize < size)
            displaySize = size;
        bulk = bulk->next;
    }

    struct Bulk* found = foundationhead;

    for(int i = 0; i < displaySize; i++) {
        printColumnRow(bulkHead, i);
        printColumnRow(bulkHead->next, i);
        printColumnRow(bulkHead->next->next, i);
        printColumnRow(bulkHead->next->next->next, i);
        printColumnRow(bulkTail->previous->previous, i);
        printColumnRow(bulkTail->previous, i);
        printColumnRow(bulkTail, i);

        if(i == 0 || i == 2
        || i == 4 || i == 6) {
            printf("F%d ", i/2+1);
            //printColumnRow(found, 0);
            found = found->next;
        }

        printf("\n");
    }


}