#ifndef _METASCANNER_H_
#define _METASCANNER_H_

// Holding all the metadata IO operations
struct node
{
	char data[30];

	char commandLetter;
	char commandTask[15];
	char cycleTime[15];
	int cycleTimeNum;

	float cycles;

	int empty;

	struct node *next;
};

// Holding individual process information including the five states (processState)
struct processInfo
{
	int processNum;
	int processTime;

	int empty;

	char processState[20];

	struct node *current;

	struct processInfo *next;
};

struct node metaDataScanner(char* filename, char* scheduleCode, char* logTo, char* logFile, int IOCycleTime, int quantumTime);

void dataSeparator(struct node *head);

struct processInfo processList(struct node *head);

void printList(struct node *head);

void startSchedule(struct node *head, struct processInfo *processes, char* scheduleCode, char* logTo, char* logFile, int IOCycleTime, int quantumTime);
#endif
