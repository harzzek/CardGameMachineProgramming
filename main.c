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

/**
 * Methods for Node operations
 */

void pushBulk(char name[]);
int pushCardToDeck(struct Bulk* bulk, char *value);
struct Bulk* findBulk(int column);
struct Bulk* findFoundation(int column);
struct Card* findTail(struct Card* card);
struct Card* findCard(struct Bulk* bulk, char *cardValue);
struct Card* popCard(struct Bulk* bulk, struct Card* card);
void initGame();
int canCardMove(struct Card* cardToMove, struct Bulk* toBulk);

/**
 * Methods for the logic of the game
 */
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

/**
 * Methods for creating the view
 */
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

/**
 * The main method
 */
int main() {
    gameLoaded = 0;
    startPhase();
    return 0;
}

/**
 * A method for initializing all the global variables
 */

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

/**
 * A push operation for pushing a card to the top of the bulk
 * @param bulk a Bulk pointer
 * @param value a char
 * @return boo
 */

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

/**
 * A push operation for a card
 * @param toBulk a Bulk pointer
 * @param cardToPush a Card pointer
 * @return boo
 */

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

/**
 * A method that contains the second and first char of a card
 * @param counter a int
 * @param value a char
 * @param type
 */

void addCard(int counter,char value, char type)
{
    deck[counter].data[0] = value;
    deck[counter].data[1] = type;
}

/**
 * A method for loading the Cards.txt file into the game
 */
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

/**
 * A method for swapping positions of two cards in the deck
 * @param a a Card pointer
 * @param b a Card pointer
 */

void swap( struct Card *a, struct Card *b){
    struct Card temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * A method for shuffling the deck randomly
 * @param deck a Card pointer
 * @param n a int
 */

void shuffle(struct Card *deck, int n) //Shuffles array
{
    srand(time(NULL));

    for(int i = n-1; i > 0; i--)
    {
        int j = rand() % (i+1);
        swap(&deck[i], &deck[j]);
    }
}

/**
 * A method for making the amount of cards that need to be invisible, invisible
 * @param bulk a Bulk pointer
 * @param numOfInvisible a int
 */

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

/**
 *
 */

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

/**
 * A method for making all cards invisible
 */

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

/**
 * A method for showing all cards in the deck
 */

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

/**
 * A method for loading in columns in the PLAY phase
 */

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

/**
 * A method for loading in the columns in the STARTUP phase
 */

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

/**
 * A method for printing out a column
 * @param head a bulk pointer
 */

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

/**
 * A method for determining whether a move is possible to make
 * @param cardToMove a Card pointer
 * @param toBulk a Bulk pointer
 * @return canMove a int
 */

int canCardMove(struct Card* cardToMove, struct Bulk* toBulk){
    int canMove = 0;
    struct Card* tail = toBulk->cTail;

    if(toBulk->cHead != NULL)
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

/**
 * A method for exiting the program
 */

void quitProgram()
{
    exit(1);
}

/**
 * A method for commands are possible in the STARTUP phase
 */

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
/**
 * A method for what commands are possible in the PLAY phase.
 */
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
        } else if((input[0] == 'c'|| input[0] == 'C' || input[0] == 'f' || input[0] == 'F') && input[2] == ':')
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

                struct Bulk *toBulk;
                if(input[0] == 'c')
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
            }

        } else if(input[0] == 'Q')
        {
            initGame();
            startPhase();
        } else errorMsg = 1;



        finished = gameOver();
    }

}

/**
 * A method for printing each row.
 * @param bulk a Bulk pointer
 * @param row a int
 */

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

/**
 * A method for determining if the player has won
 * @return gameOver a int
 */

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

/**
 * A method for the amount of cards in the Bulk
 * @param bulk a Bulk pointer
 * @return count a int
 */

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

/**
 * A method that contains the amount of Bulks
 * @return count a int
 */

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

/**
 * A method for the last input, and the message in the console in the STARTUP phase
 * @param lastInput a char pointer
 * @param messageBoo a int
 * @return str a char pointer
 */

char* startConsole(char* lastInput, int messageBoo)
{
    char input[2];

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

/**
 * A method for the last input, and the message in the console in the PLAY phase
 * @param lastInput a char pointer
 * @param messageBoo a int
 * @return str a char pointer
 */

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

/**
 * A find method for Bulk
 * @param column a int
 * @return bulk a Bulk pointer.
 */

struct Bulk* findBulk(int column){
    struct Bulk* bulk = bulkHead;

    for (int i = 0; i < column-1; ++i) {
        bulk = bulk->next;
    }

    return bulk;
}

/**
 * A find method for the foundations
 * @param column a int
 * @return bulk a Bulk pointer
 */

struct Bulk* findFoundation(int column){
    struct Bulk* bulk = foundationhead;

    for (int i = 0; i < column-1; ++i) {
        bulk = bulk->next;
    }

    return bulk;
}

/**
 * A find method for cards
 * @param bulk a Bulk pointer
 * @param cardValue a char pointer
 * @return card a Card pointer
 */

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

/**
 * A find method for the tail of a card
 * @param card a Card pointer
 * @return cardHead a Card pointer
 */

struct Card* findTail(struct Card* card){
    struct Card* cardHead = card;

    while(cardHead->next != NULL)
    {
        cardHead = cardHead->next;
    }

    return cardHead;

}

/**
 * A pop operation for the removing the top card in the deck
 * @param bulk a Bulk pointer
 * @param card a Card pointer
 * @return card a Card pointer
 */

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

/**
 * A method that controls how the cards are displayed in the console
 */

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

/**
 * A method that gives each gives each card value a int value from 1 to 13.
 * @param value a char
 * @return value a char
 */

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