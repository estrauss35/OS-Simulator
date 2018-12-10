#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include "MetaScanner.h"

struct argStruct
{
	struct node *current;
	struct processInfo *process;
	char* scheduleCode;
	double currentTime;
	int progNum;
	char* display;
	char* logFile;
	char* time;
	int qTime;
	int IOCycle;
};

int toMonitor(struct node *current, char* scheduleCode, int currenttime, int progNum, char* time);

int toMonitorP(struct node *current, struct processInfo *process, char* scheduleCode, int currenttime, int progNum, char* time, int quantumTime, int IOCycleTime);

int toFile(struct node *current, char* logFile, char* scheduleCode, int currentTime, int progNum, char* time);

int toFileP(struct node *current, struct processInfo *process, char* logFile, char* scheduleCode, int currenttime, int progNum, char* time, int quantumTime, int IOCycleTime);

int opCount(struct node *current, int progNum);

void *doSchedule(void *varg);

void scheduleThreads(struct node *current, struct processInfo *process, char* scheduleCode, int currentTime, int progNum, char* logTo, char* logFile, char* timeString, int quantumTime, int IOCycleTime);

#endif
