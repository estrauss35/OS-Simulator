#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "MetaScanner.h"

// Function to get the length of a string
int length(char* myString)
{
	char *string = myString;
	int counter = 0;
	int index = 0;
	while(string[index] != '\0')
	{
		index++;
	}
	return (counter);
}

// Quick function I made to easily add new chars to the char arrays
void append(char* myString, char letterToAppend)	
{
	int strLen = strlen(myString);
	myString[strLen] = letterToAppend;
	myString[strLen + 1] = '\0';
}

// Function to compare strings
int compare( char *str1, char *str2 )
{
	while( *str1 && ( *str1 == *str2 ) )
	{
		str1++;
		str2++;
	}
	return *( unsigned char * )str1 - *( unsigned char * )str2;
}

// Adding a new node in my linked list
void push(struct node *head, char* val) 
{
	struct node *current = head;
	struct node *newNode = (struct node*)malloc(sizeof(struct node));

	for(int index = 0; index < strlen(val); index++)
	{
		append(newNode->data, val[index]);
	}
	newNode->empty = 1;
	dataSeparator(newNode);
	if(head->next == NULL)
	{
		head->next = newNode;
	}
	else
	{
		while(current->next != NULL)
		{
			current = current->next;
		}
		current->next = newNode;
	}
}

void pushProcess(struct processInfo *head, int procNum, int procTime)
{
	struct processInfo *current = head;
	struct processInfo *newProcess = malloc(sizeof(struct processInfo));

	newProcess->processNum = procNum;
	newProcess->processTime = procTime;

	if(head->next == NULL)
	{
		head->next = newProcess;
	}
	else
	{
		while(current->next != NULL)
		{
			current = current->next;
		}
		current->next = newProcess;
	}
}
