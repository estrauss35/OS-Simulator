#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "MetaScanner.h"
#include "Helpers.h"
#include "Sorting.h"

void bubbleSort(struct processInfo *head)
{
	struct processInfo *pointer1;
	struct processInfo *pointer2;

	for(pointer1 = head; pointer1->next != NULL; pointer1 = pointer1->next)
	{
		for(pointer2 = pointer1->next; pointer2 != NULL; pointer2 = pointer2->next)
		{
			if(pointer1->processTime > pointer2->processTime)
			{
				swap(pointer1, pointer2);
			}
		}
	}
}

void bubbleSortProcNum(struct processInfo *head)
{
	struct processInfo *pointer1;
	struct processInfo *pointer2;

	for(pointer1 = head; pointer1->next != NULL; pointer1 = pointer1->next)
	{
		for(pointer2 = pointer1->next; pointer2 != NULL; pointer2 = pointer2->next)
		{
			if(pointer1->processNum > pointer2->processNum)
			{
				swap(pointer1, pointer2);
			}
		}
	}
}

// Function to swap two nodes in my process List
void swap(struct processInfo *first, struct processInfo *second)
{
	int numTemp = first->processNum;
	int timeTemp = first->processTime;

	first->processNum = second->processNum;
	first->processTime = second->processTime;
	
	second->processNum = numTemp;
	second->processTime = timeTemp;
}

void printProcesses(struct processInfo *head)
{
	struct processInfo *current = head;

	while(current->next != NULL)
	{
		printf("%d\n%d\n\n", current->processNum, current->processTime);
		current = current->next;
	}
	printf("%d\n%d\n\n", current->processNum, current->processTime);
}
