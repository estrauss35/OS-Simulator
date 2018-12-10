#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include "Scheduler.h"
#include "MetaScanner.h"
#include "simtimer.h"

// Does the proper scheduling according to the input
int toMonitor(struct node *current, char* scheduleCode, int currenttime, int progNum, char* time)
{
	int currentTime = currenttime;
	char* Time = time;
	if(current->commandLetter == 'S')
	{
		if(strcmp(current->commandTask, "start") == 0)
		{
			printf("TIME: %s, System Start\n", time);
			printf("TIME: %s, OS: Begin PCB Creation\n", time);
			printf("TIME: %s, OS: All processes initialized in New State\n", time);
			printf("TIME: %s, OS: All processes now set in Ready State\n", time);
			printf("TIME: %s, OS: Begin PCB Creation\n", time);
		}
		else if(strcmp(current->commandTask, "end") == 0)
		{
			printf("TIME: %s, System Stop\n", time);
		}
	}
	else if(current->commandLetter == 'A')
	{
		if(strcmp(current->commandTask, "start") == 0)
		{
			printf("TIME: %s, %s selects Process %d\n", time, scheduleCode, progNum);
			printf("TIME: %s, OS: Process %d set in Running State\n", time, progNum);
		}
		else if(strcmp(current->commandTask, "end") == 0)
		{
			printf("TIME: %s, OS: Process %d set in Exit State\n", time, progNum);
		}
	}
	else if(current->commandLetter == 'M')
	{
		if(strcmp(current->commandTask, "allocate") == 0)
		{
			accessTimer( LAP_TIMER, Time );
			printf("TIME: %s, Process %d: Memory management allocation start\n", Time, progNum);
			runTimer(5000);
		}
		else if(strcmp(current->commandTask, "access") == 0)
		{
			accessTimer( LAP_TIMER, Time );
			printf("TIME: %s, Process %d: Memory management access start\n", Time, progNum);
			//runTimer(currentTime);
			accessTimer( LAP_TIMER, Time );
			printf("TIME: %s, Process %d: Memory management access end\n", Time, progNum);
		}
	}
	else if(current->commandLetter == 'I')
	{
		accessTimer( LAP_TIMER, Time );
		printf("TIME: %s, Process %d: %s input start\n", Time, progNum, current->commandTask);
		runTimer(currentTime);
		accessTimer( LAP_TIMER, Time );
		printf("TIME: %s, Process %d: %s input end\n", Time, progNum, current->commandTask);
	}
	else if(current->commandLetter == 'O')
	{
		accessTimer( LAP_TIMER, Time );
		printf("TIME: %s, Process %d: %s output start\n", Time, progNum, current->commandTask);
		runTimer(currentTime);
		accessTimer( LAP_TIMER, Time );
		printf("TIME: %s, Process %d: %s output end\n", Time, progNum, current->commandTask);
	}
	else if(current->commandLetter == 'P')
	{
		accessTimer( LAP_TIMER, Time );
		printf("TIME: %s, Process %d: Run operation start\n", Time, progNum);
		runTimer(currentTime);
		accessTimer( LAP_TIMER, Time );
		printf("TIME: %s, Process %d: Run operation end\n", Time, progNum);
	}
	else
	{
		accessTimer( LAP_TIMER, Time );
		printf("TIME: %s, Process %d: Unknown Operation. Skipping...\n", Time, progNum);
	}
	accessTimer( LAP_TIMER, Time );
	return progNum;
}

// Same as toMonitor but has preemption
int toMonitorP(struct node *current, struct processInfo *process, char* scheduleCode, int currenttime, int progNum, char* time, int quantumTime, int IOCycleTime)
{
	char* Time = time;
	accessTimer( LAP_TIMER, Time );
	printf("TIME: %s, Process %d set in Running State\n", Time, progNum);
	
	int qTime = quantumTime;
	int currentTime = currenttime;
	int cycleTracker = 0;
	while(qTime > 0)
	{
		if(process->processTime <=0)
		{
			printf("TIME: %s, Process %d set in Exit State\n", Time, progNum);
			break;
		}
		accessTimer( LAP_TIMER, Time );
		if(current->commandLetter != 'M')
		{
			runTimer(IOCycleTime);
			currentTime -= IOCycleTime;
			cycleTracker += 1;
		}
		

		if(currentTime <= 0)
		{
			accessTimer( LAP_TIMER, Time );
			current->cycleTimeNum = 0;
			if(current->commandLetter == 'I')
			{
				printf("TIME: %s, Completed All Cyles for %s Input\n\n", Time, current->commandTask);
			}
			else if(current->commandLetter == 'O')
			{
				printf("TIME: %s, Completed All Cyles for %s Output\n\n", Time, current->commandTask);
			}
			else if(current->commandLetter == 'P')
			{
				printf("TIME: %s, Completed All Cyles for %s Operation\n\n", Time, current->commandTask);
			}
			current = current->next;
			currentTime = current->cycleTimeNum;
			currentTime *= IOCycleTime;
			if(current->commandLetter == 'I')
			{
				printf("TIME: %s, %s Input with %d cycles\n", Time, current->commandTask, current->cycleTimeNum);
			}
			else if(current->commandLetter == 'O')
			{
				printf("TIME: %s, %s Output with %d cycles\n", Time, current->commandTask, current->cycleTimeNum);
			}
			else if(current->commandLetter == 'P')
			{
				printf("TIME: %s, %s Operation with %d cycles\n", Time, current->commandTask, current->cycleTimeNum);
			}
			
		}
		if(current->commandLetter != 'M')
		{
			process->processTime -= 1;
		}
		
		qTime -= 1;
	}
	printf("TIME: %s, Process %d quantum time out\n", Time, process->processNum);
	if(current->commandLetter == 'I')
	{
		printf("TIME: %s, %s Input has %d cycles remaining\n\n", Time, current->commandTask, current->cycleTimeNum / IOCycleTime);
	}
	else if(current->commandLetter == 'O')
	{
		printf("TIME: %s, %s Output has %d cycles remaining\n\n", Time, current->commandTask, current->cycleTimeNum / IOCycleTime);
	}
	else if(current->commandLetter == 'P')
	{
		printf("TIME: %s, %s Operation has %d cycles remaining\n\n", Time, current->commandTask, current->cycleTimeNum / IOCycleTime);
	}
	return 1;
}

// Same as above but writes to file instead of printing to terminal
int toFile(struct node *current, char* logFile, char* scheduleCode, int currentTime, int progNum, char* time)
{
	char* Time = time;
	FILE *scheduleLog = fopen(logFile, "ab+");
	if(current->commandLetter == 'S')
	{
		if(strcmp(current->commandTask, "start") == 0)
		{
			FILE *scheduleLog = fopen(logFile, "w");
			fprintf(scheduleLog, "TIME: %s, System Start\n", time);
			fprintf(scheduleLog, "TIME: %s, OS: Begin PCB Creation\n", time);
			fprintf(scheduleLog, "TIME: %s, OS: All processes initialized in New State\n", time);
			fprintf(scheduleLog, "TIME: %s, OS: All processes now set in Ready State\n", time);
			fprintf(scheduleLog, "TIME: %s, OS: Begin PCB Creation\n", time);
		}
		else if(strcmp(current->commandTask, "end") == 0)
		{
			fprintf(scheduleLog, "TIME: %s, System Stop\n", time);
		}
	}
	else if(current->commandLetter == 'A')
	{
		if(strcmp(current->commandTask, "start") == 0)
		{
			fprintf(scheduleLog, "TIME: %s, %s selects Process %d\n", time, scheduleCode, progNum);
			fprintf(scheduleLog, "TIME: %s, OS: Process %d set in Running State\n", time, progNum);
		}
		else if(strcmp(current->commandTask, "end") == 0)
		{
			fprintf(scheduleLog, "TIME: %s, OS: Process %d set in Exit State\n", time, progNum);
		}
	}
	else if(current->commandLetter == 'M')
	{
		if(strcmp(current->commandTask, "allocate") == 0)
		{
			fprintf(scheduleLog, "TIME: %s, Process %d: Memory management allocation start\n", time, progNum);
		}
		else if(strcmp(current->commandTask, "access") == 0)
		{
			fprintf(scheduleLog, "TIME: %s, Process %d: Memory management access start\n", time, progNum);
			fprintf(scheduleLog, "TIME: %s, Process %d: Memory management access end\n", time, progNum);
		}
	}
	else if(current->commandLetter == 'I')
	{
		accessTimer( LAP_TIMER, Time );
		fprintf(scheduleLog, "TIME: %s, Process %d: %s input start\n", time, progNum, current->commandTask);
		runTimer(currentTime);
		accessTimer( LAP_TIMER, Time );
		fprintf(scheduleLog, "TIME: %s, Process %d: %s input end\n", time, progNum, current->commandTask);
	}
	else if(current->commandLetter == 'O')
	{
		accessTimer( LAP_TIMER, Time );
		fprintf(scheduleLog, "TIME: %s, Process %d: %s output start\n", time, progNum, current->commandTask);
		runTimer(currentTime);
		accessTimer( LAP_TIMER, Time );
		fprintf(scheduleLog, "TIME: %s, Process %d: %s output end\n", time, progNum, current->commandTask);
	}
	else if(current->commandLetter == 'P')
	{
		accessTimer( LAP_TIMER, Time );
		fprintf(scheduleLog, "TIME: %s, Process %d: Run operation start\n", time, progNum);
		runTimer(currentTime);
		accessTimer( LAP_TIMER, Time );
		fprintf(scheduleLog, "TIME: %s, Process %d: Run operation end\n", time, progNum);
	}
	else
	{
		fprintf(scheduleLog, "TIME: %s, Process %d: Unknown Operation. Skipping...\n", time, progNum);
	}
	return progNum;
}

// Same as toFile but with preemption
int toFileP(struct node *current, struct processInfo *process, char* logFile, char* scheduleCode, int currenttime, int progNum, char* time, int quantumTime, int IOCycleTime)
{
	FILE *scheduleLog = fopen(logFile, "ab+");
	char* Time = time;
	accessTimer( LAP_TIMER, Time );
	fprintf(scheduleLog, "TIME: %s, Process %d set in Running State\n", Time, progNum);
	
	int qTime = quantumTime;
	int currentTime = currenttime;
	int cycleTracker = 0;
	while(qTime > 0)
	{
		if(process->processTime <=0)
		{
			fprintf(scheduleLog, "TIME: %s, Process %d set in Exit State\n", Time, progNum);
			break;
		}
		accessTimer( LAP_TIMER, Time );
		if(current->commandLetter != 'M')
		{
			runTimer(IOCycleTime);
			currentTime -= IOCycleTime;
			cycleTracker += 1;
		}
		

		if(currentTime <= 0)
		{
			accessTimer( LAP_TIMER, Time );
			current->cycleTimeNum = 0;
			if(current->commandLetter == 'I')
			{
				fprintf(scheduleLog, "TIME: %s, Completed All Cyles for %s Input\n\n", Time, current->commandTask);
			}
			else if(current->commandLetter == 'O')
			{
				fprintf(scheduleLog, "TIME: %s, Completed All Cyles for %s Output\n\n", Time, current->commandTask);
			}
			else if(current->commandLetter == 'P')
			{
				fprintf(scheduleLog, "TIME: %s, Completed All Cyles for %s Operation\n\n", Time, current->commandTask);
			}
			current = current->next;
			currentTime = current->cycleTimeNum;
			currentTime *= IOCycleTime;
			if(current->commandLetter == 'I')
			{
				fprintf(scheduleLog, "TIME: %s, %s Input with %d cycles\n", Time, current->commandTask, current->cycleTimeNum);
			}
			else if(current->commandLetter == 'O')
			{
				fprintf(scheduleLog, "TIME: %s, %s Output with %d cycles\n", Time, current->commandTask, current->cycleTimeNum);
			}
			else if(current->commandLetter == 'P')
			{
				fprintf(scheduleLog, "TIME: %s, %s Operation with %d cycles\n", Time, current->commandTask, current->cycleTimeNum);
			}
			
		}
		if(current->commandLetter != 'M')
		{
			process->processTime -= 1;
		}
		
		qTime -= 1;
	}
	fprintf(scheduleLog, "TIME: %s, Process %d quantum time out\n", Time, process->processNum);
	if(current->commandLetter == 'I')
	{
		fprintf(scheduleLog, "TIME: %s, %s Input has %d cycles remaining\n\n", Time, current->commandTask, current->cycleTimeNum / IOCycleTime);
	}
	else if(current->commandLetter == 'O')
	{
		fprintf(scheduleLog, "TIME: %s, %s Output has %d cycles remaining\n\n", Time, current->commandTask, current->cycleTimeNum / IOCycleTime);
	}
	else if(current->commandLetter == 'P')
	{
		fprintf(scheduleLog, "TIME: %s, %s Operation has %d cycles remaining\n\n", Time, current->commandTask, current->cycleTimeNum / IOCycleTime);
	}
	return 1;
}

int opCount(struct node *current, int progNum)
{
	if(current->commandLetter == 'A')
	{
		if(strcmp(current->commandTask, "start") == 0)
		{
			progNum++;
		}
	}
	return progNum;
}

void *doSchedule(void *arguments)
{
	struct argStruct *args = arguments;
	if(strcmp(args->display, "Monitor") == 0)
	{
		if(strcmp(args->scheduleCode, "FCFS-P") == 0 || strcmp(args->scheduleCode, "SJF-P") == 0)
		{
			toMonitorP(args->current, args->process, args->scheduleCode, args->currentTime, args->progNum, args->time, args->qTime, args->IOCycle);
		}
		else if(strcmp(args->scheduleCode, "FCFS-N") == 0 || strcmp(args->scheduleCode, "SJF-N") == 0)
		{
			toMonitor(args->current, args->scheduleCode, args->currentTime, args->progNum, args->time);
		}
		
	}
	else if(strcmp(args->display, "Both") == 0)
	{
		if(strcmp(args->scheduleCode, "FCFS-P") == 0 || strcmp(args->scheduleCode, "SJF-P") == 0)
		{
			toMonitorP(args->current, args->process, args->scheduleCode, args->currentTime, args->progNum, args->time, args->qTime, args->IOCycle);
			toFile(args->current, args->logFile, args->scheduleCode, args->currentTime, args->progNum, args->time);
		}
		else if(strcmp(args->scheduleCode, "FCFS-N") == 0 || strcmp(args->scheduleCode, "SJF-N") == 0)
		{
			toMonitor(args->current, args->scheduleCode, args->currentTime, args->progNum, args->time);
			toFile(args->current, args->logFile, args->scheduleCode, args->currentTime, args->progNum, args->time);
		}
		
	}
	else if(strcmp(args->display, "File") == 0)
	{
		toFile(args->current, args->logFile, args->scheduleCode, args->currentTime, args->progNum, args->time);
	}
	
	return NULL;
}

void scheduleThreads(struct node *current, struct processInfo *process, char* scheduleCode, int currentTime, int progNum, char* logTo, char* logFile, char* timeString, int quantumTime, int IOCycleTime)
{
	// Creating a struct pf arguments to pass into the thread
	struct argStruct *args = NULL;
	args = malloc(sizeof(struct argStruct));

	args->current = current;
	args->scheduleCode = scheduleCode;
	args->currentTime = currentTime;
	args->progNum = progNum;
	args->display = logTo;
	args->logFile = logFile;
	args->time = timeString;
	args->qTime = quantumTime;
	args->process = process;
	args->IOCycle = IOCycleTime;

	pthread_t thread;
	pthread_create(&thread, NULL, doSchedule, args);
	pthread_join(thread, NULL);
}
