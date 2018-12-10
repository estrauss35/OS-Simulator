#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ConfigScanner.h"
#include "MetaScanner.h"
#include "Helpers.h"
#include "Scheduler.h"
#include "Sorting.h"
#include "simtimer.h"

// Grabbing the chunks of data out of the metafile and storing all the processes one big linked list
struct node metaDataScanner(char* filename, char* scheduleCode, char* logTo, char* logFile, int IOCycleTime, int quantumTime)
{
	printf("==========================\nBegin Simulation\n\n");
	char dataHolder[30];
	char currentValue;
	int startSaving = 0;

	

	FILE *metafile = fopen(filename, "r");

	struct node *head;
	head = malloc(sizeof(struct node));
	head->empty = 0;

	if(metafile == NULL)
	{
		printf("ERROR OPENING FILE\n");
	}
	else
	{
		while((currentValue = fgetc(metafile)) != EOF)
		{
			if(currentValue == ':')
			{
				startSaving = 1;
				currentValue = ' ';
				memset(dataHolder, 0, sizeof(dataHolder));
			}
			else if(startSaving == 1)
			{
				if(currentValue != ';')
				{
					append(dataHolder, currentValue);
				}
				else
				{
					if(head->empty == 0)
					{
						for(int index; index < strlen(dataHolder); index++)
						{
							append(head->data, dataHolder[index]);
						}
						dataSeparator(head);
						head->empty = 1;
						head->next = NULL;
					}
					else
					{
						push(head, dataHolder);
					}
					memset(dataHolder, 0, sizeof(dataHolder));
				}
			}
		}
		struct processInfo *process = NULL;
		process = malloc(sizeof(struct processInfo));
		*process = processList(head);

		bubbleSort(process);
		startSchedule(head, process, scheduleCode, logTo, logFile, IOCycleTime, quantumTime);
	}
	
	

	return *head;
}

// Separating the whole string of data into appropriate parts inside its node (mainly used in my "Push" method)
void dataSeparator(struct node *head)
{
	struct node *current = head;
	int checker = 0;
	for(int index = 0; index < strlen(current->data); index++)
	{
		if(current->data[index] == '(')
		{
			checker = 1;
		}
		else
		{
			if(checker == 0)
			{
				current->commandLetter = current->data[index];
			}
			else if(checker == 1)
			{
				if(current->data[index] == ')')
				{
					checker++;
				}
				else
				{
					if(current->data[index] != ' ')
					{
						append(current->commandTask, current->data[index]);
					}
				}
			}
			else if(checker == 2)
			{
				if(current->data[index] != ' ')
				{
					append(current->cycleTime, current->data[index]);
				}
			}
			current->cycleTimeNum = atoi(current->cycleTime);
			current->cycles = current->cycleTimeNum / 55;
		}
	}
}

// Separating my big linked list into the separate processes (between A(start) and A(end)) to sort for SJF
struct processInfo processList(struct node *head)
{
	struct processInfo *processes = NULL;
	processes = malloc(sizeof(struct processInfo));
	
	int procNum = 0;

	processes->empty = 0;

	int timeSum = 0;

	struct node *current = head;
	while(current->next != NULL)
	{
		if(current->commandLetter != 'S' && current->commandLetter != 'M')
		{
			timeSum += current->cycleTimeNum;
		}
		if(current->commandLetter == 'A')
		{
			
			if(strcmp(current->commandTask, "end") == 0)
			{
				if(processes->empty == 0)
				{
					processes->processNum = procNum;
					processes->processTime = timeSum;
					processes->empty = 1;
					processes->next = NULL;
					procNum++;
				}
				else
				{
					pushProcess(processes, procNum, timeSum);
					procNum++;
				}
				
				timeSum = 0;
			}
		}
		current = current->next;
	}
	return *processes;
}

// Print out all the meta data that is stored in my linked list
void printList(struct node *head)
{
	struct node *current = head;
	while(current->next != NULL)
	{
		printf("The data item component letter is: %c\nThe data item operation string is: %s\nThe data item cycle time is: %d\n\n", current->next->commandLetter, current->next->commandTask, current->next->cycleTimeNum);
		current = current->next;
	}
	printf("End of List.\n");
}

//	*** Most of Sim04 Code is here and in scheduler ***

// Kicking off the threading
// Master threads are in each if statement (the while loop)
void startSchedule(struct node *head, struct processInfo *processes, char* scheduleCode, char* logTo, char* logFile, int IOCycleTime, int quantumTime)
{
	struct node *current = head;
	struct processInfo *process = processes;
	int qTime = quantumTime;

	int *timePointer = (int *)malloc(sizeof(int));
    	char timeString[ 10 ];

	int currentTime = 10;

	if(strcmp(logTo, "File") == 0 || strcmp(logTo, "Both") == 0)
	{
		FILE *scheduleLog = fopen(logFile, "w"); // Clearing the contents of the file if it already exists
		fclose(scheduleLog);
	}

	int progNum = -1;
	// Scheduling Protocol for FCFS-N
	if(strcmp(scheduleCode, "FCFS-N") == 0)
	{
		accessTimer( ZERO_TIMER, timeString);
		while(current->next != NULL)
		{
			accessTimer( LAP_TIMER, timeString );
			*timePointer = current->cycleTimeNum;
			*timePointer = (*timePointer) * IOCycleTime;
			progNum = opCount(current, progNum);
			scheduleThreads(current, process, scheduleCode, *timePointer, progNum, logTo, logFile, timeString, quantumTime, IOCycleTime);
			accessTimer( LAP_TIMER, timeString );
			current = current->next;
		}
		scheduleThreads(current, process, scheduleCode, *timePointer, progNum, logTo, logFile, timeString, quantumTime, IOCycleTime);
		printf("TIME: %s, System Stop\n", timeString);
	}
	else if(strcmp(scheduleCode, "FCFS-P") == 0)
	{
		bubbleSortProcNum(process);
		accessTimer( ZERO_TIMER, timeString);
		while(current->next != NULL)
		{
			accessTimer( LAP_TIMER, timeString );
			*timePointer = current->cycleTimeNum;
			*timePointer = (*timePointer) * IOCycleTime;
			if(timePointer > 0)	//Checking if there are any cycles left to complete in the current operation
			{
				printf("TIME: %s, %s selects process %d with time: %d\n", timeString, scheduleCode, process->processNum, process->processTime);
				accessTimer( LAP_TIMER, timeString );
				printf("TIME: %s, Process %d set in Running State\n", timeString, process->processNum);
				progNum = opCount(current, progNum);
				scheduleThreads(current, process, scheduleCode, *timePointer, progNum, logTo, logFile, timeString, quantumTime, IOCycleTime);
			}
			if(process->processTime <=0)
			{
				process = process->next;
			}
			accessTimer( LAP_TIMER, timeString );
			current = current->next;
		}
	}
	// Scheduling Protocol for SJF-N
	else if(strcmp(scheduleCode, "SJF-N") == 0)
	{
		int posCheck = 0;
		accessTimer( ZERO_TIMER, timeString );
		while(process->next != NULL)
		{
			while(current->next != NULL)
			{
				accessTimer( LAP_TIMER, timeString );
				*timePointer = current->cycleTimeNum;
				*timePointer = (*timePointer) * IOCycleTime;
				if(posCheck == process->processNum)
				{
					scheduleThreads(current, process, scheduleCode, *timePointer, process->processNum, logTo, logFile, timeString, quantumTime, IOCycleTime);
					current = current->next;
					accessTimer( LAP_TIMER, timeString );
				}
				else
				{
					current = current->next;
				}
				if(current->commandLetter == 'A' && strcmp(current->commandTask, "end") == 0)
				{
					if(posCheck == process->processNum)
					{
						scheduleThreads(current, process, scheduleCode, *timePointer, process->processNum, logTo, logFile, timeString, quantumTime, IOCycleTime);
						accessTimer( LAP_TIMER, timeString );
						break;
					}
					else
					{
						posCheck += 1;
						current = current->next;
					}
				}
			}
			posCheck = 0;
			current = head->next;
			process = process->next;	
		}
		while(current->next != NULL)
		{
			scheduleThreads(current, process, scheduleCode, *timePointer, process->processNum, logTo, logFile, timeString, quantumTime, IOCycleTime);
			current = current->next;
			if(current->commandLetter == 'A' && strcmp(current->commandTask, "end") == 0)
			{
				scheduleThreads(current, process, scheduleCode, *timePointer, process->processNum, logTo, logFile, timeString, quantumTime, IOCycleTime);
				break;
			}
		}
		printf("TIME: %s, System Stop\n", timeString);
	}
	else if(strcmp(scheduleCode, "SJF-P") == 0)
	{
		int posCheck = 0;
		accessTimer( ZERO_TIMER, timeString );
		while(process->next != NULL)
		{
			while(current->next != NULL)
			{
				accessTimer( LAP_TIMER, timeString );
				*timePointer = current->cycleTimeNum;
				*timePointer = (*timePointer) * IOCycleTime;
				if(timePointer > 0)
				{
					if(posCheck == process->processNum)
					{
						scheduleThreads(current, process, scheduleCode, currentTime, process->processNum, logTo, logFile, timeString, quantumTime, IOCycleTime);
						current = current->next;
						accessTimer( LAP_TIMER, timeString );
					}
					else
					{
						current = current->next;
					}
					if(current->commandLetter == 'A' && strcmp(current->commandTask, "end") == 0)
					{
						if(posCheck == process->processNum)
						{
							scheduleThreads(current, process, scheduleCode, currentTime, process->processNum, logTo, logFile, timeString, quantumTime, IOCycleTime);
							accessTimer( LAP_TIMER, timeString );
							break;
						}
						else
						{
							posCheck += 1;
							current = current->next;
						}
					}
				}
				if(process->processTime <=0)
				{
					process = process->next;
				}
				
			}
			posCheck = 0;
			current = head->next;
			process = process->next;	
		}
		while(current->next != NULL)
		{
			scheduleThreads(current, process, scheduleCode, currentTime, process->processNum, logTo, logFile, timeString, quantumTime, IOCycleTime);
			current = current->next;
			if(current->commandLetter == 'A' && strcmp(current->commandTask, "end") == 0)
			{
				scheduleThreads(current, process, scheduleCode, currentTime, process->processNum, logTo, logFile, timeString, quantumTime, IOCycleTime);
				break;
			}
		}
		printf("TIME: %s, System Stop\n", timeString);
	}
	else if(strcmp(scheduleCode, "RR-P") == 0)
	{
		qTime = 3;
		int cycleTracker = 0;
		while(qTime > 0)
		{
			timePointer -= IOCycleTime;
			qTime -= 1;
			cycleTracker += 1;
		}
		printf("Completed %d Cycles, %d Cycles Remaining", cycleTracker, (*timePointer) / IOCycleTime);
	}
}

