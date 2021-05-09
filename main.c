#include "funktions.h"
#include <stdio.h>
#include <stdlib.h>
#define CRDS 52
#include <time.h>

struct Card{
    char data[3];
    int visible;
    int value;
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
struct Card* popCard(struct Bulk* bulk, char cardValue[2]);
void initGame();

void moveCard(struct Card* cardToMove, struct Bulk* toBulk);

//Logic
int loadDeck();
void shuffle(struct Card *deck, int n);
void loadGameColumns();
void printColumn(struct Bulk *head);
void loadDefaultColumns();
void loadInvisibility();
int numberOfBulks();
int numberOfCardsInBulk(struct Bulk* bulk);
int gameOver();
void startPhase();
int giveValue(char value);
int numberOfChars(char* string);

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
        newCard->value = giveValue(value[0]);
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
        bulk->cHead->value = giveValue(value[0]);
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

void loadGameInvisibility()
{
    int bulkLength = numberOfBulks();
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

    for (int i = 0; i < numberOfBulks(); ++i) {
        card = bulk->cHead;
        for (int j = 0; j < numberOfCardsInBulk(bulk); ++j) {
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

    for (int i = 0; i < numberOfBulks(); ++i) {
        card = bulk->cHead;
        for (int j = 0; j < numberOfCardsInBulk(bulk); ++j) {
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

// C3:5H
void playPhase()
{
    char* input;
    int errorMsg;
    int finished;

    while(finished == 0)
    {

        display();

        input = startConsole(input,errorMsg);
        int sizeOfInput = numberOfChars(input);

        if(input[0] == 'Q' && input[0] == 'Q')
        {
            quitProgram();
            errorMsg = 1;
        } else if(sizeOfInput == 5)
        {
            int columnNumber = (int) input[1];
            struct Bulk* bulk= bulkHead;

            for (int i = 0; i < columnNumber; ++i) {
                bulk = bulk->next;
            }

            char cardvalue[3];
            cardvalue[0] = input[3];
            cardvalue[1] = input[4];
            cardvalue[2] = '\0';

            struct Card* card = popCard(bulk,cardvalue);

        }
    }

}

void moveCard(struct Card* cardToMove, struct Bulk* toBulk){

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

int numberOfCardsInBulk(struct Bulk* bulk){
    int count = 0;
    struct Card* current = bulk->cHead;
    while (current != NULL)
    {
        count++;
        current = current->next;
    }

    return count;
}

int numberOfBulks(){
    int count = 0;
    struct Bulk* bulk = bulkHead;
    while (bulk != NULL)
    {
        count++;
        bulk = bulk->next;
    }

    return count;
}

int numberOfChars(char* string)
{
    int count = 0;
    while(string[count] != NULL || string[count] != " ")
    {
        count++;
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

char* playConsole(char* lastInput, int messageBoo)
{
    char input[5];

    printf("LAST Command: %s\n", lastInput);
    if(messageBoo == 1)
        printf("Message: OK\n");
    else if(messageBoo == 0)
        printf("Message: Error\n");
    else printf("Message: \n");
    printf("INPUT >\n");
    scanf("%s",input);

    char *str = malloc(6);

    str[0] = input[0];
    str[1] = input[1];
    str[2] = input[2];
    str[3] = input[3];
    str[4] = input[4];
    str[5] = '\0';

    return str;
}

struct Card* popCard(struct Bulk* bulk, char cardValue[3]){

    struct Card* card = bulk->cHead;
    int numberOfCards = numberOfCardsInBulk(bulk);


    if(bulk->cTail != NULL)
    {
        for(int i = 0; i < numberOfCards; i++)
        {
            if(cardValue[0] == card->data[0] || cardValue[1] == card->data[1])
            {
                break;
            } else card = card->next;
        }

        if(card->previous != NULL)
        {
            bulk->cTail = card->previous;
            card->previous = NULL;
            bulk->cTail->next = NULL;
        } else {
            bulk->cTail = NULL;
            bulk->cHead = NULL;
        }
    }

    return card;
}

void display()
{
    printf("c1\tc2\tc3\tc4\tc5\tc6\tc7\n\n");

    struct Bulk* bulk = bulkHead;
    int displaySize = 0;
    int numOfColumns = numberOfBulks();

    for(int i = 0; i < numOfColumns; i++)
    {
        int size = numberOfCardsInBulk(bulk);
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

int giveValue(char value)
{
    switch(value)
    {
        case 'A':
            return 1;
        case '2':
            return 2;
        case '3':
            return 3;
        case '4':
            return 4;
        case '5':
            return 5;
        case '6':
            return 6;
        case '7':
            return 7;
        case '8':
            return 8;
        case '9':
            return 9;
        case 'T':
            return 10;
        case 'J':
            return 11;
        case 'Q':
            return 12;
        case 'K':
            return 13;
    }
}