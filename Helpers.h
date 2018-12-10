#ifndef HELPERS_H
#define HELPERS_H
#include "MetaScanner.h"

int length(char* myString);

void append(char* myString, char letterToAppend);

int compare( char *str1, char *str2 );

void push(struct node *head, char val[]);

void pushProcess(struct processInfo *head, int procNum, int procTime);

#endif
