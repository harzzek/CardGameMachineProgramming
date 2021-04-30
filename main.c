#include "funktions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CRDS 52
#include <time.h>
#include<stdbool.h>

typedef struct Card{
    char data[2];
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
void createColumns();
void printColumn(Card *head);
void display();

int main() {
    createDeck();
    shuffle(cards,CRDS);
    shuffle(cards,CRDS);
    createColumns();

    display();
    //printColumn(c4Head);

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
        (**tailOfLink).next = newCard;
        *tailOfLink = newCard;
    } else
        {
            *headOfLink = malloc(sizeof(struct Card));
            (*headOfLink)->previous = NULL;
            (*headOfLink)->next = NULL;
            (*headOfLink)->data[0] = cardToImport->data[0];
            (*headOfLink)->data[1] = cardToImport->data[1];
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

void createColumns()
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


void display() {
    Card *head1 = c1Head;
    Card *head2 = c2Head;
    Card *head3 = c3Head;
    Card *head4 = c4Head;
    Card *head5 = c5Head;
    Card *head6 = c6Head;
    Card *head7 = c7Head;

    printf("%c%c\t%c%c\t%c%c\t%c%c\t%c%c\t%c%c\t%c%c\n",
           head1->data[0], head1->data[1],
           head2->data[0], head2->data[1],
           head3->data[0], head3->data[1],
           head4->data[0], head4->data[1],
           head5->data[0], head5->data[1],
           head6->data[0], head6->data[1],
           head7->data[0], head7->data[1]);
    head2 = head2->next;
    head3 = head3->next;
    head4 = head4->next;
    head5 = head5->next;
    head6 = head6->next;
    head7 = head7->next;

    for(int i = 1; i < 6; i++) {
        printf("\t%c%c\t%c%c\t%c%c\t%c%c\t%c%c\t%c%c\n",
               head2->data[0], head2->data[1],
               head3->data[0], head3->data[1],
               head4->data[0], head4->data[1],
               head5->data[0], head5->data[1],
               head6->data[0], head6->data[1],
               head7->data[0], head7->data[1]);
        head2 = head2->next;
        head3 = head3->next;
        head4 = head4->next;
        head5 = head5->next;
        head6 = head6->next;
        head7 = head7->next;
    }

        printf("\t\t%c%c\t%c%c\t%c%c\t%c%c\t%c%c\n",
               head3->data[0], head3->data[1],
               head4->data[0], head4->data[1],
               head5->data[0], head5->data[1],
               head6->data[0], head6->data[1],
               head7->data[0], head7->data[1]);
        head4 = head4->next;
        head5 = head5->next;
        head6 = head6->next;
        head7 = head7->next;

    printf("\t\t\t%c%c\t%c%c\t%c%c\t%c%c\n",
           head4->data[0], head4->data[1],
           head5->data[0], head5->data[1],
           head6->data[0], head6->data[1],
           head7->data[0], head7->data[1]);
    head5 = head5->next;
    head6 = head6->next;
    head7 = head7->next;

    printf("\t\t\t\t%c%c\t%c%c\t%c%c\n",
           head5->data[0], head5->data[1],
           head6->data[0], head6->data[1],
           head7->data[0], head7->data[1]);
    head6 = head6->next;
    head7 = head7->next;

    printf("\t\t\t\t\t%c%c\t%c%c\n",
           head6->data[0], head6->data[1],
           head7->data[0], head7->data[1]);
    head7 = head7->next;

    printf("\t\t\t\t\t\t%c%c\n",
           head7->data[0], head7->data[1]);





}


    /*

    for(int i = 1; i < 11; i++) {
        if (head1 != NULL)
            printf("%c%c\t", head1->data[0], head1->data[1]);
        else printf("\t");
        if (head2 != NULL)
            printf("%c%c\t", head2->data[0], head2->data[1]);
        else printf("\t");
        if (head3 != NULL)
            printf("%c%c\t", head3->data[0], head3->data[1]);
        else printf("\t");
        if (head4 != NULL)
            printf("%c%c\t", head4->data[0], head4->data[1]);
        else printf("\t");
        if (head5 != NULL)
            printf("%c%c\t", head5->data[0], head5->data[1]);
        else printf("\t");
        if (head6 != NULL)
            printf("%c%c\t", head6->data[0], head6->data[1]);
        else printf("\t");
        if (head7 != NULL)
            printf("%c%c\n", head7->data[0], head7->data[1]);
        head1 = head1->next;
        head2 = head2->next;
        head3 = head3->next;
        head4 = head4->next;
        head5 = head5->next;
        head6 = head6->next;
        head7 = head7->next;
    }
     */
void quitProgram()
{
    exit(1);
}

void startPhase()
{
    createDeck();
}

