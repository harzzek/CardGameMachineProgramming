#include "funktions.h"
#include <stdio.h>
#include <stdlib.h>
#define CRDS 52
#include <time.h>
#include <stdbool.h>
#include <string.h>

typedef struct Card{
    char data[3];
    bool visible;
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

struct Card* f1tail = NULL;
struct Card* f2tail = NULL;
struct Card* f3tail = NULL;
struct Card* f4tail = NULL;

struct Card* c1Tail = NULL;
struct Card* c2Tail = NULL;
struct Card* c3Tail = NULL;
struct Card* c4Tail = NULL;
struct Card* c5Tail = NULL;
struct Card* c6Tail = NULL;
struct Card* c7Tail = NULL;

int createDeck();
void shuffle(Card *deck, int n);
void push(Card** headOfLink, Card** tailOfLink, Card* cardToImport);
void createGameColumns();
void printColumn(Card *head);
void display();
void deckToColumns();

int main() {
    createDeck();
    deckToColumns();

    display();

    char command[2] = "SW";

    scanf("%s",command);

    switch(command[0] + command[1]){
        case 'S'+'W':
            printf("%c%c", command[0], command[1]);
            break;
        case 'L'+'D':
            printf("%c%c", command[0], command[1]);
            break;
        default:
            break;

    }


/*
    for (int i = 0; i < CRDS; i++){

        printf("%c", cards[i].data[0]);
        printf("%c\n",cards[i].data[1]);
    }
    */
    return 0;
}

void addCard(int counter,char value, char type)
{
    cards[counter].data[0] = value;
    cards[counter].data[1] = type;
}

void push(Card** headOfLink, Card** tailOfLink, Card* cardToImport)
{
    if(*headOfLink != NULL)
    {
        struct Card* newCard = malloc(sizeof(struct Card));
        newCard->next = NULL;
        newCard->previous = *tailOfLink;
        newCard->data[0] = cardToImport->data[0];
        newCard->data[1] = cardToImport->data[1];
        newCard->data[2] = '\0';
        (**tailOfLink).next = newCard;
        *tailOfLink = newCard;
    } else
        {
            *headOfLink = malloc(sizeof(struct Card));
            (*headOfLink)->previous = NULL;
            (*headOfLink)->next = NULL;
            (*headOfLink)->data[0] = cardToImport->data[0];
            (*headOfLink)->data[1] = cardToImport->data[1];
            (*headOfLink)->data[2] = '\0';
            *tailOfLink = *headOfLink;
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

void createGameColumns()
{
    for(int i = 0; i < CRDS; i++)
    {
        if(i < 7)
        {
            if(i == 0) push(&c1Head, &c1Tail, &cards[i]);
            if(i == 1) push(&c2Head, &c2Tail, &cards[i]);
            if(i == 2) push(&c3Head, &c3Tail, &cards[i]);
            if(i == 3) push(&c4Head, &c4Tail, &cards[i]);
            if(i == 4) push(&c5Head, &c5Tail, &cards[i]);
            if(i == 5) push(&c6Head, &c6Tail, &cards[i]);
            if(i == 6) push(&c7Head, &c7Tail, &cards[i]);
        } else if(i < 12)
        {
            push(&c2Head, &c2Tail, &cards[i]);
        } else if(i < 18)
        {
            push(&c3Head, &c3Tail, &cards[i]);
        } else if(i < 25)
        {
            push(&c4Head, &c4Tail, &cards[i]);
        } else if(i < 33)
        {
            push(&c5Head, &c5Tail, &cards[i]);
        } else if (i < 42)
        {
            push(&c6Head, &c6Tail, &cards[i]);
        } else if (i < 52)
        {
            push(&c7Head, &c7Tail, &cards[i]);
        }
    }
}

void deckToColumns()
{
    int i = 0;

    while(i != CRDS) {

        if(i != CRDS) push(&c1Head, &c1Tail, &cards[i]);
        i++;
        if(i != CRDS) push(&c2Head, &c2Tail, &cards[i]);
        i++;
        if(i != CRDS) push(&c3Head, &c3Tail, &cards[i]);
        i++;
        if(i != CRDS) push(&c4Head, &c4Tail, &cards[i]);
        i++;
        if(i != CRDS) push(&c5Head, &c5Tail, &cards[i]);
        i++;
        if(i != CRDS) push(&c6Head, &c6Tail, &cards[i]);
        i++;
        if(i != CRDS) push(&c7Head, &c7Tail, &cards[i]);
        i++;
    }

}


void makeColumnInvisible(Card* columnHead, int numOfInvisible)
{
    Card* card = columnHead;

    for(int i = 0; i < numOfInvisible; i++)
    {
        card->visible = false;
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
    createDeck();
}

char * modifyData(Card* card)
{
    char* newData;
    if(card == NULL)
    {
        newData = (char *) "  \0";
    } else
        {
        newData = card->data;
        }

    return newData;
}

void printRow(int row)
{
    Card* c1 = c1Head;
    Card* c2 = c2Head;
    Card* c3 = c3Head;
    Card* c4 = c4Head;
    Card* c5 = c5Head;
    Card* c6 = c6Head;
    Card* c7 = c7Head;

    Card* foundation;

    for(int i = 0 ; i < row; i++ )
    {
        c1 = c1->next;
        c2 = c2->next;
        c3 = c3->next;
        c4 = c4->next;
        c5 = c5->next;
        c6 = c6->next;
        c7 = c7->next;
    }

    if(row == 0) foundation = f1tail;
    if(row == 2) foundation = f2tail;
    if(row == 4) foundation = f3tail;
    if(row == 6) foundation = f4tail;

    if(row == 0 || row == 2 || row == 4 || row == 6)
        printf("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n",modifyData(c1),modifyData(c2),modifyData(c3),modifyData(c4),modifyData(c5),modifyData(c6),modifyData(c7), modifyData(foundation));
     else printf("%s\t%s\t%s\t%s\t%s\t%s\t%s\n",modifyData(c1),modifyData(c2),modifyData(c3),modifyData(c4),modifyData(c5),modifyData(c6),modifyData(c7));
}

void display()
{
    printf("c1\tc2\tc3\tc4\tc5\tc6\tc7\n\n");
    printRow(0);
    printRow(1);
    printRow(2);
    printRow(3);
    printRow(4);
    printRow(5);
    printRow(6);
    printRow(7);
    printRow(8);
    printRow(9);
    printRow(10);
    printRow(11);
    printRow(12);

}

