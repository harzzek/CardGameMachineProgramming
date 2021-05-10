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
    char desc;
    struct Bulk* next;
    struct Bulk* previous;
    struct Card* cHead;
    struct Card* cTail;
};

struct Card deck[CRDS];

// Node operations
void pushBulk(char name[]);
int pushCardToDeck(struct Bulk* bulk, char *value);
struct Bulk* findBulk(int column);
struct Bulk* findFoundation(int column);
struct Card* findTail(struct Card* card);
struct Card* findCard(struct Bulk* bulk, char *cardValue);
struct Card* popCard(struct Bulk* bulk, struct Card* card);
void initGame();

int canCardMove(struct Card* cardToMove, struct Bulk* toBulk);

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
void playPhase();
int giveValue(char value);
int numberOfChars(char* string);

//View
char* playConsole(char* lastInput, int messageBoo);
void display();
char* startConsole(char *lastInput, int messageBoo);

/**
 * Global variables
 */
struct Bulk* bulkHead;
struct Bulk* bulkTail;

struct Bulk* foundationhead;
struct Bulk* foundationTail;

int gameLoaded;

int main() {
    gameLoaded = 0;
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
            newBulk->desc = 'c';
            bulkTail->next = newBulk;
            bulkTail = newBulk;
        } else {
            bulkHead = malloc(sizeof(struct Bulk));
            bulkHead->previous = NULL;
            bulkHead->next = NULL;
            bulkHead->cTail = NULL;
            bulkHead->desc = 'c';
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
            newBulk->desc = 'f';
            foundationTail->next = newBulk;
            foundationTail = newBulk;
        } else {
            foundationhead = malloc(sizeof(struct Bulk));
            foundationhead->previous = NULL;
            foundationhead->next = NULL;
            foundationhead->cTail = NULL;
            foundationhead->desc = 'f';
            foundationhead->cHead = NULL;
            foundationTail = foundationhead;
        }
    }
}

int pushCardToDeck(struct Bulk* bulk, char *value)
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

int pushCard(struct Bulk* toBulk, struct Card* cardToPush){

    int boo = 0;

    struct Card* card = cardToPush;

    if(toBulk->cHead != NULL)
    {
        card->previous = toBulk->cTail;
        toBulk->cTail->next = card;
        card = findTail(card);
        toBulk->cTail = card;

    } else {
        toBulk->cHead = card;
        card = findTail(card);
        toBulk->cTail = card;
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
    initGame();
    struct Bulk* bulk = bulkHead;

    pushCardToDeck(bulk, deck[0].data);

    bulk = bulk->next;

    for(int i = 1; i < CRDS; i++)
    {
        if(i < 7) pushCardToDeck(bulk, deck[i].data);
        else if(i < 14) pushCardToDeck(bulk->next, deck[i].data);
        else if(i < 22) pushCardToDeck(bulk->next->next, deck[i].data);
        else if(i < 31) pushCardToDeck(bulk->next->next->next, deck[i].data);
        else if(i < 41) pushCardToDeck(bulk->next->next->next->next, deck[i].data);
        else if(i < 52) pushCardToDeck(bulk->next->next->next->next->next, deck[i].data);
    }
}

void loadDefaultColumns()
{
    initGame();
    int i = 0;
    struct Bulk* bulk = bulkHead;

    while(i != CRDS) {

        pushCardToDeck(bulk, deck[i].data);
        i++;
        if(i != CRDS) pushCardToDeck(bulk->next, deck[i].data);
        else break;
        i++;
        if(i != CRDS) pushCardToDeck(bulk->next->next, deck[i].data);
        else break;
        i++;
        if(i != CRDS) pushCardToDeck(bulk->next->next->next, deck[i].data);
        else break;
        i++;
        if(i != CRDS) pushCardToDeck(bulk->next->next->next->next, deck[i].data);
        else break;
        i++;
        if(i != CRDS) pushCardToDeck(bulk->next->next->next->next->next, deck[i].data);
        else break;
        i++;
        if(i != CRDS) pushCardToDeck(bulk->next->next->next->next->next->next, deck[i].data);
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

int canCardMove(struct Card* cardToMove, struct Bulk* toBulk){
    int canMove = 0;
    struct Card* tail = toBulk->cTail;

    if(toBulk->cHead != NULL || toBulk == NULL)
    {
        if(toBulk->desc == 'c') {
            if (cardToMove->value+1 == tail->value && cardToMove->data[1] != tail->data[1]) {
                canMove = 1;
            }
        } else if(toBulk->desc == 'f')
        {
            if (cardToMove->value-1 == tail->value && cardToMove->data[1] == tail->data[1]
                && cardToMove->next == NULL) {
                canMove = 1;
            }
        }
    } else {
        if(toBulk->desc == 'c') {
            if (cardToMove->value == 13) {
                canMove = 1;
            }
        } else if(toBulk->desc == 'f')
        {
            if (cardToMove->value == 1 && cardToMove->next == NULL) {
                canMove = 1;
            }
        }
    }
    return canMove;

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

    while(startupPhase == 0)
    {

        display();

        input = startConsole(input,errorMsg);

        if(input[0] == 'L' && input[1] == 'D')
        {
            gameLoaded = 1;
            loadDeck();
            loadDefaultColumns();
            makeAllInvisible();
            errorMsg = 0;
        } else if(input[0] == 'S' && input[1] == 'W')
        {
            makeAllVisible();
            errorMsg = 0;
        } else if(input[0] == 'S' && input[1] == 'R')
        {
            shuffle(deck,CRDS);
            loadDefaultColumns();
            errorMsg = 0;
        } else if(input[0] == 'Q' && input[0] == 'Q')
        {
            startupPhase = 1;
            quitProgram();
            errorMsg = 0;
        } else if(input[0] =='P')
        {
            errorMsg = 0;
            loadGameColumns();
            makeAllVisible();
            loadGameInvisibility();
            startupPhase = 1;
        } else errorMsg = 1;
    }
    playPhase();
}

// C3:5H
void playPhase()
{
    char* input;
    int errorMsg;
    int finished = 0;

    while(finished == 0)
    {

        display();

        input = playConsole(input,errorMsg);

        if(input[0] == 'Q' && input[1] == 'Q')
        {
            quitProgram();
            errorMsg = 0;
        } else if((input[0] == 'c'|| input[0] == 'C') && input[2] == ':')
        {
            // Get the input from which the card found from
            // That is bulk and card
            int columnNumber = input[1] - '0';

            struct Bulk* fromBulk = findBulk(columnNumber);

            char *cardvalue = malloc(3);
            cardvalue[0] = input[3];
            cardvalue[1] = input[4];
            cardvalue[2] = '\0';

            struct Card* cardToPush = findCard(fromBulk, cardvalue);
            free(cardvalue);
            if(cardToPush != NULL) {

                errorMsg = 0;
                input = startConsole(input, errorMsg);
                columnNumber = input[1] - '0';

                if(input[0] != 'f' || input[0] != 'c') {

                    struct Bulk *toBulk = NULL;

                    if (input[0] == 'c')
                        toBulk = findBulk(columnNumber);
                    else toBulk = findFoundation(columnNumber);

                    if (canCardMove(cardToPush, toBulk)) {
                        if (cardToPush->previous != NULL && cardToPush->previous->visible == 0) {
                            cardToPush->previous->visible = 1;
                        }
                        cardToPush = popCard(fromBulk, cardToPush);
                        pushCard(toBulk, cardToPush);
                    } else errorMsg = 1;
                    free(input);
                } else errorMsg = 1;
            }

        } else if(input[0] == 'Q')
        {
            initGame();
            startPhase();
        } else if(input[0] == 'F'|| input[0] == 'f')
        {
            int columnNumber = input[1] - '0';
            struct Bulk* fromBulk = findFoundation(columnNumber);

            struct Card* cardToPush = findTail(fromBulk->cHead);

            if(cardToPush != NULL)
            {
                errorMsg = 0;
                input = startConsole(input, errorMsg);
                columnNumber = input[1] - '0';

                if(input[0] == 'c')
                {
                    struct Bulk *toBulk;
                    toBulk = findBulk(columnNumber);

                    if (canCardMove(cardToPush, toBulk)) {
                        if (cardToPush->previous != NULL && cardToPush->previous->visible == 0) {
                            cardToPush->previous->visible = 1;
                        }
                        cardToPush = popCard(fromBulk, cardToPush);
                        pushCard(toBulk, cardToPush);
                    } else errorMsg = 1;

                } else errorMsg = 1;
            } else errorMsg = 1;

        } else errorMsg = 1;



        finished = gameOver();
    }

}

void printColumnRow(struct Bulk* bulk, int row)
{
    if(bulk->cHead != NULL) {
        struct Card *card = bulk->cHead;
        int nullBoo = 1;

        for (int i = 0; i < row; ++i) {
            if (card->next != NULL) {
                card = card->next;

            } else {
                nullBoo = 0;
                break;
            }
        }
        int isVisible = card->visible;

        if (nullBoo == 1) {
            if (isVisible == 1)
                printf("%s\t", card->data);
            else printf("[]\t");
        } else printf(" \t");
    } else printf(" \t");
}

int gameOver()
{
    int gameOver = 0;
    int counter = 0;

    if(foundationhead->cHead != NULL) {
        struct Bulk *foundation = foundationhead;

        for (int i = 0; i < 4; i++) {
            if (foundation->cTail->value == 13)
            {
                counter++;
                foundation = foundation->next;
            }
        }

        if(counter == 4){
            gameOver = 1;
        }
    }
    return gameOver;
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

char* startConsole(char* lastInput, int messageBoo)
{
    char input[5];

    printf("LAST Command: %s\n", lastInput);
    if(messageBoo == 0)
        printf("Message: OK\n");
    else if(messageBoo == 1)
        printf("Message: Error\n");
    else printf("Message: \n");
    printf("INPUT > ");
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
    if(messageBoo == 0)
        printf("Message: OK\n");
    else if(messageBoo == 1)
        printf("Message: Error\n");
    else printf("Message: \n");
    printf("INPUT > ");
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

struct Bulk* findBulk(int column){
    struct Bulk* bulk = bulkHead;

    for (int i = 0; i < column-1; ++i) {
        bulk = bulk->next;
    }

    return bulk;
}

struct Bulk* findFoundation(int column){
    struct Bulk* bulk = foundationhead;

    for (int i = 0; i < column-1; ++i) {
        bulk = bulk->next;
    }

    return bulk;
}

struct Card* findCard(struct Bulk* bulk, char *cardValue){
    struct Card* card = bulk->cHead;
    int numberOfCards = numberOfCardsInBulk(bulk);
    char* cardValues = cardValue;

    for(int i = 0; i < numberOfCards; i++)
    {
        if(cardValues[0] == card->data[0] && cardValues[1] == card->data[1])
        {
            break;
        } else card = card->next;
    }

    return card;
}

struct Card* findTail(struct Card* card){
    struct Card* cardHead = card;

    while(cardHead->next != NULL)
    {
        cardHead = cardHead->next;
    }

    return cardHead;

}

struct Card* popCard(struct Bulk* bulk, struct Card* card){

    //The actual pop
    if(card->previous != NULL)
    {
        bulk->cTail = card->previous;
        card->previous = NULL;
        bulk->cTail->next = NULL;
    } else {
        bulk->cTail = NULL;
        bulk->cHead = NULL;
    }

    return card;
}

void display()
{
    printf("c1\tc2\tc3\tc4\tc5\tc6\tc7\n\n");

    struct Bulk* bulk = bulkHead;
    int displaySize = 0;
    int numOfColumns = numberOfBulks();

    if(gameLoaded == 1)
    {
        displaySize = 7;
    }

    int size;
    for(int i = 0; i < numOfColumns; i++)
    {
        size = numberOfCardsInBulk(bulk);
        if(displaySize < size)
            displaySize = size;
        bulk = bulk->next;
    }

    struct Bulk* found = foundationhead;
    int foundCardsNum;

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
            foundCardsNum = numberOfCardsInBulk(found)-1;
            printColumnRow(found, foundCardsNum);
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