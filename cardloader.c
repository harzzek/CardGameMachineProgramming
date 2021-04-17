//
// Created by harzz on 16/04/2021.
//
#include <stdio.h>
#include "funktions.h"

#define CRDS 52
char cardsArray[CRDS];
char buff[CRDS];

int loadCards() {
    FILE *fp;
    char *filename = "..\\Cards.txt";
    *cardsArray = (char) buff;

    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("Could not open file %s", filename);
        return 1;
    }

    while (fgets(buff, CRDS, fp) != NULL)
        printf("%s", buff);
    fclose(fp);
    return 0;
}

